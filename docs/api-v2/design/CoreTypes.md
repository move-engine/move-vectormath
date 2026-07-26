# Core type proposal

## Compute vectors

Primary templates:

```cpp
template <Arithmetic T> class Vec2;
template <Arithmetic T> class Vec3;
template <Arithmetic T> class Vec4;
```

Common aliases:

```cpp
Vec2f Vec3f Vec4f
Vec2d Vec3d Vec4d
Vec2i Vec3i Vec4i
Vec2u Vec3u Vec4u
```

Floating vectors are compute values. `Vec3f` targets a 16-byte,
SIMD-friendly representation; `Vec3d` may use a wider representation.
Integer vectors support discrete component arithmetic and measurements but do
not offer nonsensical normalization/interpolation that truncates silently.

All value types should be:

- trivially copyable;
- standard-layout where compatible with private invariant design;
- zero-initialized by default;
- explicitly constructible with `NoInit` only in measured paths;
- free of serialization, stream, and backend conversion members.

Components are accessed through `X()`, `Y()`, `Z()`, `W()`, and checked/debug
indexed access. Mutating setters may be provided for ordinary vectors. Public
writable fields are not proposed for compute types because they constrain
representation and interact poorly with a hidden SIMD lane.

## Operators and comparisons

Provide ordinary component arithmetic, scalar multiply/divide, unary negation,
and compound assignment. Avoid implicit numeric narrowing.

`operator==` is exact. Approximate equality is explicit:

```cpp
IsNearlyEqual(a, b, tolerance);
```

Relational vector operators are omitted unless a compelling use appears.
Component-wise comparisons should return a named mask type or use
`AllLess`, `AnyLess`, and similar predicates; pretending they form a total
ordering is error-prone.

## Core vector functions

The core set includes:

- `Dot`, `Cross`;
- `Length`, `LengthSquared`, `Distance`, `DistanceSquared`;
- `Min`, `Max`, `Clamp`, `Abs`, `Sign`;
- `Lerp`, `LerpClamped`, `SmoothStep`;
- `Project`, `ProjectOnPlane`, `Reflect`, `Refract`;
- `Angle`, `SignedAngle`;
- `ClampedLength`, `MoveTowards`;
- `IsFinite`, `IsNearlyZero`, `IsNearlyEqual`;
- swizzles proven useful in benchmarked code.

Less common splines, random generation, packing, and gameplay helpers belong in
focused headers.

## Packed storage

Compact storage types are simple aggregates with documented fields/layout:

```cpp
struct PackedVec3f { float X, Y, Z; };
struct PackedVec4f { float X, Y, Z, W; };
struct PackedMat4f { float Elements[16]; };
```

`Packed` means dense CPU/raw-transfer representation; it does not mean
universally compatible with shader blocks. In particular, `PackedVec3f` has a
12-byte array stride, while common GPU float3 arrays use a 16-byte stride.
Opt-in GPU layout types cover those contracts.

Targets:

| Type | Proposed size | Proposed alignment |
|---|---:|---:|
| `Vec2f` | 8 or 16 prototype | 8 or 16 prototype |
| `PackedVec2f` | 8 | 4 |
| `Vec3f` | 16 | 16 |
| `PackedVec3f` | 12 | 4 |
| `Vec4f` | 16 | 16 |
| `Quatf`/`Rotation3f` | 16 | 16 |
| `Mat4f` | 64 | 16 |
| `PackedMat4f` | 64 | 4 |

These are targets to validate during the architecture proof, not yet an ABI
commitment. Component offsets and array stride are part of the eventual
contract. Storage/compute conversion is explicit and has contiguous,
strided/interleaved, and fused bulk overloads.

Phase A should attempt to make `Vec3f` byte-compatible with a common
16-byte-stride shader float3 array: size/alignment 16 with XYZ at byte offsets
0, 4, and 8. This is accepted only if triviality and generated code remain
competitive with native RTM storage. The fourth slot has no vector or
homogeneous semantic meaning, is always initialized, and is ignored by value
equality. A canonicalizing store is used when deterministic transfer bytes are
required.

`Vec2f` is a separate decision: retaining an 8-byte representation can matter
for UV, screen-space, and 2D particle arrays, while a 16-byte representation
may reduce backend load/repack work. Both must be measured rather than deriving
the answer from `Vec3f`.

GPU matrix transfer types separately state matrix major order and vector
stride. A 36-byte compact `PackedMat3f` and a 48-byte shader-block mat3 are
different representations.

## Angles

Strong angle wrappers prevent degree/radian mistakes:

```cpp
template <Floating T> class Radians;
template <Floating T> class Degrees;

using Radiansf = Radians<float>;
using Degreesf = Degrees<float>;
```

Conversion is explicit. Trigonometric functions accept radians, while literals
and named factories make intent concise:

```cpp
using namespace mv::math::literals;
auto yaw = 90.0_deg;
auto radians = ToRadians(yaw);
```

## Quaternion and rotation

`Quat<T>` remains an algebraic four-component quaternion for users who need
general quaternion logarithm, exponential, or non-unit arithmetic.

`Rotation3<T>` is the primary orientation type and maintains a unit-quaternion
invariant. Conversions between them are explicit/fallible where normalization
can fail.

This separation retains v1's general quaternion functionality without forcing
every rotation operation to accept invalid rotation data.

## Matrices

Proposed core types:

- `Mat3<T>` for a general 3x3 matrix;
- `Mat4<T>` for a general 4x4 matrix;
- optionally `AffineMat3x4<T>` as a compact affine matrix if benchmarks justify
  it.

Default v2 convention retains row-major addressing, row vectors, and
`value * matrix` transformation to avoid gratuitous behavioral churn.
Storage order is documented independently from multiplication semantics.

Construction that depends on handedness or clip depth must be explicit.
Matrix APIs provide:

- row/column access with unambiguous names;
- transpose, determinant, inverse with fallible singular handling;
- transform of vectors/points through semantic overloads;
- rotation/scale/translation and projection construction;
- decomposition returning a typed result rather than several ambiguous output
  parameters.

## Initialization

Default construction remains safe:

- vectors/points: zero;
- matrices/rotations/transforms: identity;
- geometry primitives: valid degenerate or documented canonical values.

An explicit `NoInit` tag is available only on plain compute/storage types where
it does not violate an invariant:

```cpp
Vec3f value(NoInit);
```

Invariant types such as `Direction3` and `Rotation3` cannot be uninitialized
through their public API.
