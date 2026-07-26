# Data layout and GPU interop proposal

## Design principle

Semantic meaning, byte representation, and execution backend are independent
axes.

- `Vec3f` versus `Point3f` expresses mathematical meaning.
- compute versus packed versus GPU-transfer form expresses byte layout.
- scalar versus RTM/SIMD is an internal implementation choice.

The API must not force users to select a backend type merely to obtain compact
storage or a shader-compatible layout.

## Representation families

### Compute

The normal arithmetic types are SIMD-friendly:

| Type family | Target size/alignment | Role |
| --- | --- | --- |
| `Vec2f`, `Point2f`, `Direction2f` | prototype 8/8 versus 16/16 | CPU computation |
| `Vec3f`, `Point3f`, `Direction3f`, `Normal3f` | 16/16 | CPU computation |
| `Vec4f`, `Quatf`, `Rotation3f` | 16/16 | CPU computation |
| `Mat3f` | prototype dependent | CPU computation |
| `Mat4f` | 64/16 | CPU computation |

For three-component float compute values, Phase A should attempt to guarantee:

- XYZ component bytes at offsets 0, 4, and 8;
- size and alignment of 16;
- a fourth storage slot that has no public semantic meaning;
- trivial copyability;
- direct use by the selected SIMD backend without per-operation repacking.

The padding slot is always initialized but its value may be unspecified after
arithmetic where the destination layout ignores it. It does not participate in
equality, hashing, or serialization. APIs needing deterministic bytes use a
canonicalizing store. It is never interpreted as homogeneous W.

Two-component vectors should not be padded to 16 bytes merely for conceptual
uniformity. Phase A must compare an 8-byte value loaded into SIMD operations
against a persistently 16-byte representation. UVs, screen coordinates, and 2D
particle data make the cache/storage cost practically important.

### Compact CPU and raw transfer storage

```cpp
struct PackedVec3f
{
    float X;
    float Y;
    float Z;
};
```

Required contract:

- size 12, alignment 4;
- XYZ at offsets 0, 4, and 8;
- standard-layout and trivially copyable;
- no hidden padding or backend state;
- suitable for compact CPU arrays and explicitly 12-byte graphics formats.

Equivalent packed point/direction/normal types can remain distinct to preserve
semantic validation at load boundaries:

```cpp
PackedPoint3f Store(Point3f);
PackedDirection3f Store(Direction3f);

Point3f Load(PackedPoint3f);
std::optional<Direction3f> TryLoad(PackedDirection3f);
```

Trusted bulk paths may use internal checked-once or unchecked construction.
GPU readback does not automatically satisfy direction/normal invariants.

### Composite storage

Frequently persisted spatial structures receive packed counterparts when the
savings are material:

| Compute value | Compact candidate | Typical use |
| --- | ---: | --- |
| `Aabb3f` | `PackedAabb3f`, 24 bytes | large CPU culling/BVH arrays |
| `RigidTransform3f` | 28-byte payload candidate | animation/instance storage |
| `Mat3f` | `PackedMat3f`, 36 bytes | files and CPU storage |
| `AffineTransform3f` | 48-byte payload candidate | instance/animation storage |

These sizes are payload candidates, not promises that the same bytes form a
valid shader structure. For example, a GPU AABB represented as two standalone
float3 slots commonly occupies 32 bytes. GPU transfer counterparts follow
their declared shader layout.

Packed composite types are added only for common boundaries or measured
working-set wins. They reuse leaf `Load`/`Store` operations and do not duplicate
the entire geometry/query API.

### GPU layout types

GPU layouts live in an opt-in header/namespace and state their physical
contract. Candidate building blocks include:

```cpp
namespace mv::math::gpu
{
    struct Float3Packed;       // size 12, alignment 4
    struct alignas(16) Float3Slot16; // size/stride 16

    template <MatrixMajor Major>
    struct Mat3f16;            // three 16-byte vector slots

    template <MatrixMajor Major>
    struct Mat4f16;            // four 16-byte vector slots
}
```

`Float3Packed` and `PackedVec3f` may be aliases only if their contracts are
identical and the name remains unambiguous at call sites. A broad name such as
`GpuVec3f` is avoided because GPU APIs do not share one float3 layout.

Shader structure packing is contextual. For example, a float3 followed by a
scalar can use one 16-byte slot:

```cpp
struct alignas(16) GpuPositionRadius
{
    PackedVec3f Position;
    float Radius;
};

static_assert(offsetof(GpuPositionRadius, Position) == 0);
static_assert(offsetof(GpuPositionRadius, Radius) == 12);
static_assert(sizeof(GpuPositionRadius) == 16);
```

Arrays of standalone shader `vec3` values commonly require a distinct
16-byte-stride element type. The library must not use the 12-byte payload type
for that case unless the selected shader layout explicitly permits it.

## Matrices and transforms

Matrix transfer types state:

- scalar format;
- row/column major byte order;
- vector stride;
- total size and alignment;
- whether the shader declaration needs `row_major`, `column_major`, or an
  explicit transpose.

A compact 3x3 scalar matrix is 36 bytes, while common shader-block layouts
store three padded vector slots for 48 bytes. These are different types.

Affine 3x4 transfer types should be supplied in the orientation actually
consumed by shaders. Names and traits must distinguish a row-major three-row
form from a column-major four-column form even when both contain 12 floats.

`Store` functions perform transpose/reordering only when required:

```cpp
gpu::Mat4f16<MatrixMajor::Column> gpuMatrix =
    StoreGpu<MatrixMajor::Column>(cpuMatrix);
```

If `Mat4f` already has an identical documented layout, a checked byte view can
avoid the copy.

## Views and conversion

### Single values

Single-value transitions are explicit:

```cpp
Vec3f value = Load(packed);
Store(packed, value);
gpu::Float3Slot16 upload = StoreGpu16(value);
```

Loads/stores map directly to backend load3/store3 or aligned operations. They
must not route through temporary scalar arrays.

### Contiguous batches

```cpp
TransformPoints(
    std::span<const PackedPoint3f> input,
    RigidTransform3f transform,
    std::span<gpu::Float3Slot16> output);
```

The kernel loads once, performs the complete operation chain in compute form,
and stores once. A temporary compute array is not required.

### Strided and interleaved batches

The batch module provides a lightweight `StridedSpan<T>` or equivalent field
view:

```cpp
auto positions = MakeFieldView(vertices, &Vertex::Position);
auto normals = MakeFieldView(vertices, &Vertex::Normal);

TransformPoints(positions, transform, outputPositions);
TransformNormals(normals, transform, outputNormals);
```

Its iterator/reference design must allow vectorization and expose stride
without virtual dispatch or allocation. Separate input and output layouts
support vertex conversion and upload staging in one pass.

### Direct byte views

Zero-copy views are opt-in and trait constrained:

```cpp
auto bytes = AsGpuBytes<GpuLayout::Float3Array16>(
    std::span<const Vec3f>(positions));
```

The overload exists only when the source type's published size, alignment,
component offsets, array stride, and scalar representation satisfy the target
layout. Compatibility is never inferred from equal `sizeof` alone.

The engine's buffer-structure layer should validate complete structures against
shader reflection or generated schema metadata.

## Choosing persistent CPU storage

Document the choice by dominant use:

| Workload | Preferred starting representation |
| --- | --- |
| frequent vector arithmetic, modest collections | compute values |
| large, bandwidth/cache-bound collections | packed or SoA |
| static/interleaved vertices | declared vertex-transfer structure |
| frequently uploaded 16-byte shader arrays | compatible 16-byte transfer or compute values |
| mixed CPU/GPU particle updates | benchmark packed, compute, and SoA paths |
| invariant values read back from GPU | transfer storage followed by validated load |

Neither compact nor compute storage is labeled universally “fast.” The
selected representation should reflect the number of operations per load and
the working-set/cache cost.

## Ergonomics

Packed and transfer values provide:

- named construction;
- public scalar fields where compatible with layout guarantees;
- component access;
- exact equality;
- `Load`, `Store`, and batch operations.

They do not duplicate the full vector math surface. Arithmetic happens after
one explicit load or inside a fused batch algorithm. This keeps costs visible
without forcing users to write intrinsics or manual packing loops.

Common data paths should remain short:

```cpp
// CPU-hot
positions[i] += velocities[i] * dt;

// Compact persistent storage, one operation chain
Vec3f p = Load(packedPositions[i]);
p = Integrate(p, Load(packedVelocities[i]), dt);
Store(packedPositions[i], p);

// Bulk/interleaved
IntegrateParticles(positionView, velocityView, dt);

// Zero-copy upload when proven compatible
Upload(AsGpuBytes<GpuLayout::Float3Array16>(positions));
```

## Layout traits

Every storage/transfer type publishes or supports compile-time traits for:

- scalar/component type;
- component count and offsets;
- size and alignment;
- array stride;
- matrix major order and vector stride;
- standard-layout/trivial-copy status;
- compatible GPU layout profiles;
- whether padding bytes are meaningful, canonical, or ignored.

Traits are descriptive. They do not make backend representations part of
public type identity.

## Phase A proof requirements

Prototype and compare at least two `Vec3f` representations:

1. selected backend-native storage;
2. fixed 16-byte public storage loaded into backend registers.

Choose only after measuring:

- chained arithmetic and transforms;
- function-call ABI behavior;
- load/store instruction count;
- trivial copyability and component offsets;
- direct 16-byte-stride GPU upload compatibility;
- scalar and RTM builds;
- vector header compile time.

Independently compare 8-byte and 16-byte `Vec2f` candidates; there is no reason
to make every dimension follow the three-vector decision.

Also implement the first 12-byte packed and 16-byte GPU-slot types plus
contiguous/strided conversion kernels. This makes storage/GPU requirements part
of the architectural proof rather than a deferred adapter concern.

## Non-goals

- one type promised to match every graphics API layout;
- automatic runtime shader reflection inside the math library;
- implicit packed/compute conversion;
- public backend template parameters;
- a full arithmetic API duplicated on every transfer representation;
- unconditional SoA containers before workloads justify their shape.
