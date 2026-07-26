# Backend architecture proposal

## Objective

Retain v1's central motivation—one public/high-level implementation with
specialized scalar or SIMD primitives—without exposing backend inheritance or
backend wrapper types in the public API.

## Layering

```text
mv::math semantic and geometry types
  Point, Direction, Rotation, Transform, Ray, Aabb, Frustum
                         │
                         ▼
mv::math core value types and shared algorithms
  Vec, Quat, Mat, interpolation, transform primitives
                         │
                         ▼
mv::math::detail selected primitive operations
  load/store, arithmetic, shuffle, dot, cross, compare/select, sqrt
                         │
             ┌───────────┴───────────┐
             ▼                       ▼
       scalar backend          RTM/SIMD backend
```

Only the bottom primitive vocabulary is duplicated. Geometry and semantic
algorithms are written once.

## Stable public identity

The public template depends on component type, not backend:

```cpp
template <typename T>
class Vec3;

using Vec3f = Vec3<float>;
using Vec3d = Vec3<double>;
```

Conceptually, implementation looks like:

```cpp
template <typename T>
class Vec3
{
public:
    Vec3 operator+(Vec3 rhs) const noexcept
    {
        return FromNative(detail::Ops<T>::Add(Native_, rhs.Native_));
    }

private:
    detail::NativeVec<T> Native_;
};
```

`detail::NativeVec<T>` and `detail::Ops<T>` come from exactly one selected
backend. Neither is a public template argument, base class, conversion
operator, or return type.

The implementation may instead use a fixed opaque 16/32-byte storage block and
load into native registers inside operations if code-generation experiments
show that this improves ABI stability without adding instructions. That choice
is an implementation prototype decision, not public API.

## Backend selection

V2 initially supports target-wide selection:

- RTM/SIMD by default when supported;
- scalar fallback;
- explicit scalar target for testing and constrained platforms.

Only the selected backend header is included by core value headers. Scalar and
RTM implementations are compiled in separate test/benchmark targets rather
than both being parsed and instantiated by every consumer.

Per-value backend selection is not proposed for the normal API. It creates
multiple identities for the same conceptual value, complicates conversions,
and was a major source of v1 template weight. Advanced backend experiments can
use internal test types or a future opt-in policy layer without changing
`Vec3f`.

Because this is header-only, all translation units in a target must use the
same backend configuration. CMake interface definitions and a configuration
signature/static assertion should make mismatches fail early.

## Primitive contract

The first prototype should keep the backend vocabulary deliberately small:

- native zero/splat/set and lane access;
- aligned/unaligned and two/three/four-component load/store;
- add, subtract, multiply, divide, fused multiply-add where available;
- min, max, abs, negation;
- reciprocal, square root, reciprocal square root;
- dot2/dot3/dot4 and cross3;
- comparisons, masks, select, finite checks;
- shuffle/swizzle primitives required by measured algorithms;
- matrix multiply/transpose/inverse primitives where a backend implementation
  materially wins;
- unit-quaternion multiply, vector rotate, and conversion primitives.

Do not mirror the entire RTM API. A small vocabulary limits backend work and
keeps shared algorithm code portable.

## Shared algorithms

Operations that can be expressed efficiently from primitives are implemented
once:

```cpp
template <typename Ops>
auto Reflect(auto incident, auto unitNormal)
{
    return Ops::Sub(
        incident,
        Ops::Mul(unitNormal, Ops::Splat(2 * Ops::Dot3(incident, unitNormal))));
}
```

In actual code, templates/concepts should be kept inside `detail` and avoid
creating large overload sets in user translation units. Common concrete
float/double paths can use non-generic inline functions when that compiles
faster.

## Semantic wrappers

`Point3<T>`, `Direction3<T>`, and `Normal3<T>` contain a `Vec3<T>` with no
additional fields. Their methods delegate to the same core operations.
Private/invariant constructors should not inhibit trivial copyability.

The intended properties are:

- same size/alignment as `Vec3<T>`;
- no runtime tag;
- no virtual dispatch;
- no repeated normalization for operations accepting `Direction3`;
- optimized code identical to the equivalent validated raw-vector kernel.

## Storage boundary

Packed and GPU-transfer data are separate from compute identity:

```cpp
PackedVec3f stored;             // 12-byte transport/storage value
Vec3f value = Load(stored);     // explicit compute boundary
Store(stored, value);
```

This preserves v1's useful fast/storage distinction without making
acceleration policy part of public type identity. `Vec3f` is the
SIMD-friendly compute value; `PackedVec3f` is the dense CPU/raw-transfer
value. Neither is universally faster—the right persistent representation
depends on operations per load and working-set pressure.

GPU layouts require an additional explicit contract. A 12-byte packed float3
is not interchangeable with a shader float3 array that requires 16-byte
alignment/stride. Opt-in GPU transfer types and compatibility traits cover
these cases; there is no universal `GpuVec3f`.

Bulk forms operate on contiguous or strided views and permit backend
vectorization:

```cpp
TransformPoints(
    std::span<const PackedPoint3f> input,
    RigidTransform3f transform,
    std::span<gpu::Float3Slot16> output);

TransformPoints(
    StridedSpan<const PackedPoint3f> input,
    RigidTransform3f transform,
    StridedSpan<PackedPoint3f> output);
```

An implicit packed-to-compute conversion is not proposed because it hides a
load and can cause repeated conversions in loops. Fused batch kernels load
once, perform the complete operation, and store once without an intermediate
compute array.

When a compute/storage span exactly satisfies a declared GPU layout, a
trait-constrained byte view permits zero-copy upload. Compatibility includes
component offsets, alignment, array stride, matrix order, and padding policy;
equal `sizeof` is not sufficient.

The detailed contract is in
[`DataLayoutAndGpuInterop.md`](DataLayoutAndGpuInterop.md).

## Interoperability

Backend escape hatches live in opt-in headers:

```cpp
#include <mv/math/interop/rtm.hpp>

rtm::vector4f native = mv::math::interop::rtm::ToNative(value);
Vec3f value = mv::math::interop::rtm::FromNativeVec3(native);
```

The core header does not need to promise the native type or homogeneous W lane.
Point and direction adapters choose W explicitly.

## Validation

Every backend must pass:

- the same behavioral/property tests;
- size, alignment, component-offset, triviality, and standard-layout
  assertions;
- generated-code comparisons for primitive and representative composed
  operations;
- compile-time and preprocessed-line measurements;
- runtime counter benchmarks;
- scalar-versus-SIMD numerical parity within documented tolerance.

The architecture proof also measures 12-byte packed, 16-byte compute, and
16-byte GPU-slot arrays across cache-sized working sets, plus strided
interleaved data and fused layout conversion.

The existing ray benchmarks showing identical Move/raw-RTM instruction counts
are the minimum zero-overhead standard to preserve.
