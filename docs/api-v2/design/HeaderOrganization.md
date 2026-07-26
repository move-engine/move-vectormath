# Header and source organization proposal

## Public include tree

```text
packages/move/math/include/mv/math/
  Config.hpp
  Concepts.hpp
  Constants.hpp
  Tolerance.hpp
  Angles.hpp
  Vec2.hpp
  Vec3.hpp
  Vec4.hpp
  Quat.hpp
  Rotation3.hpp
  Mat3.hpp
  Mat4.hpp

  semantic/
    Point2.hpp
    Point3.hpp
    Direction2.hpp
    Direction3.hpp
    Normal3.hpp

  transforms/
    RigidTransform2.hpp
    RigidTransform3.hpp
    SimilarityTransform3.hpp
    TrsTransform3.hpp
    AffineTransform3.hpp
    Projection.hpp

  geometry/
    Line3.hpp
    Ray3.hpp
    Segment3.hpp
    Plane3.hpp
    Triangle3.hpp
    Sphere3.hpp
    Capsule3.hpp
    Aabb3.hpp
    Obb3.hpp

  queries/
    Classification.hpp
    HitResults.hpp
    LinearQueries.hpp
    BoundsQueries.hpp
    ClosestPoint.hpp
    PreparedRay3.hpp

  culling/
    Frustum3.hpp
    PreparedFrustum3.hpp
    BoxSphereBounds3.hpp

  packed/
    Vectors.hpp
    Matrices.hpp
    Bounds.hpp
    Directions.hpp

  gpu/
    Layouts.hpp
    Vectors.hpp
    Matrices.hpp
    Encodings.hpp

  batch/
    StridedSpan.hpp
    Transforms.hpp
    Queries.hpp
    Culling.hpp

  graphics/
    ClipConvention.hpp
    Project.hpp
    Viewport.hpp

  interop/
    Rtm.hpp
    DirectXMath.hpp

  format/
    Format.hpp

  serialization/
    Cereal.hpp

  All.hpp
```

Exact aggregation may be reduced after prototype compile measurements; the
important rule is one-way dependencies and opt-in capabilities.

## Internal tree

```text
packages/move/math/include/mv/math/detail/
  BackendSelect.hpp
  NativeTypes.hpp
  PrimitiveOps.hpp
  scalar/
    NativeTypes.hpp
    PrimitiveOps.hpp
  rtm/
    NativeTypes.hpp
    PrimitiveOps.hpp
    MatrixOps.hpp
    QuaternionOps.hpp
  algorithms/
    VectorAlgorithms.hpp
    MatrixAlgorithms.hpp
    GeometryAlgorithms.hpp
```

Only `BackendSelect.hpp` includes one backend implementation. Public headers do
not include scalar and RTM implementations simultaneously.

## Dependency direction

```text
Config/Concepts/Constants
          ↓
Vec/Angles/Quat/Mat
          ↓
Semantic types and Rotation
          ↓
Transforms
          ↓
Geometry primitives
          ↓
Queries and Culling
          ↓
Batch/Graphics/Adapters
```

Formatting, serialization, and interop depend on public types; public types do
not depend on them.

## Umbrella headers

Provide:

- `Core.hpp`: scalars, angles, vectors, quaternion/rotation, matrices;
- `Spatial.hpp`: semantic types, transforms, geometry, queries, culling;
- `All.hpp`: every first-party math facility except third-party adapters.

The smallest documented examples should include focused headers. Umbrella
headers are convenience, not the benchmark baseline.

## Compilation boundaries

Each proposed module gets a header self-sufficiency test. Compile benchmarks
cover:

- `Vec3.hpp` alone;
- rotation;
- matrix;
- semantic types;
- transforms;
- one geometry primitive;
- one query family;
- frustum/culling;
- packed formats;
- GPU transfer layouts;
- contiguous and strided batch views;
- each adapter;
- `Core.hpp`, `Spatial.hpp`, and `All.hpp`.

Track preprocessed lines and peak front-end memory in CI or a reproducible
report target.

## Tests

```text
packages/move/math/tests/v2/
  core/
  semantic/
  transforms/
  geometry/
  queries/
  culling/
  packed/
  gpu/
  batch/
  properties/
  codegen/
  compile/
```

Scalar and RTM behavioral tests are separate target configurations. Property
tests cover invariants and cross-backend parity. Code-generation checks focus
on a small stable kernel corpus rather than brittle whole-function byte
snapshots.

## Benchmark repository

Create the benchmark v2 branch only when the first compilable v2 types land.
Organize benchmarks by capability, with libraries as competitors within each
capability:

```text
benchmarks/
  core/vector/
  core/rotation/
  transforms/
  geometry/ray_triangle/
  geometry/ray_aabb/
  culling/frustum_aabb/
  workloads/
  storage/
  gpu_transfer/
  compilation/
```

This answers “which library is fastest for this capability?” directly and
matches nanobench's grouping/relative-baseline strengths.
