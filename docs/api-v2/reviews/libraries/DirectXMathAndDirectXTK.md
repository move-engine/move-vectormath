# DirectXMath and DirectXTK API review

## Scope

This review covers the June 2026 DirectXMath source/documentation,
`DirectXCollision`, and the March 2026 DirectXTK SimpleMath documentation and
header.

Together they show a useful layering:

- DirectXMath is an all-inline SIMD primitive API.
- `XMFLOAT*` and packed types are storage representations.
- `XMVECTOR`/`XMMATRIX` are opaque compute values with platform-aware calling
  conventions.
- DirectXCollision adds bounding volumes and queries.
- DirectXTK SimpleMath adds an ergonomic value facade over the primitives.

## DirectXMath architecture

DirectXMath explicitly instructs users to load storage values into
`XMVECTOR`/`XMMATRIX`, compute, and store results. `XMVECTOR` is opaque and
optimally aligned/mapped to a hardware register. Parameter aliases such as
`FXMVECTOR`, `GXMVECTOR`, and `HXMVECTOR` encode platform calling-convention
choices.

This is the clearest comparison point for Move's storage/compute concern. It
proves the architecture is valuable, but also illustrates why the raw backend
API is not an ideal gameplay facade:

- names are dense and prefix-heavy;
- four-lane compute values stand in for points, directions, planes,
  quaternions, and colors;
- users manually choose load/store functions;
- behavior and calling conventions are optimized for low-level use.

Move's facade should preserve opacity and optimized representation without
exposing backend-specific parameter aliases or forcing manual load/store for
ordinary local code.

## SimpleMath facade

SimpleMath provides PascalCase `Vector2`, `Vector3`, `Vector4`, `Quaternion`,
`Matrix`, `Plane`, `Ray`, and bounding-volume wrappers. `Vector3` derives from
the 12-byte `XMFLOAT3`, exposes public components, loads to `XMVECTOR` for
operations, and supplies:

- length/dot/cross/normalization;
- distance, min/max, clamp, and interpolation;
- barycentric, Catmull-Rom, Hermite, reflect, and refract;
- vector and normal transforms;
- array transforms;
- direction constants.

`Matrix` adds decomposition, inverse/transpose, billboard, world, look-at,
projection, rotation, and scale construction. The documentation states a
right-handed, Y-up, observer-facing +Z convention for the XNA-like facade and
row-major matrix addressing.

SimpleMath is much easier to use but pays compilation and load/store costs that
depend on compiler optimization and operation chaining. Its measured compile
cost sits between raw DirectXMath and Move v1. Move should offer a facade this
thin while allowing a compute value to remain resident across a chain of
operations.

## DirectXCollision coverage

DirectXCollision provides:

- `BoundingSphere`;
- center/extents `BoundingBox`;
- center/extents/unit-quaternion `BoundingOrientedBox`;
- origin/orientation/slopes/near/far `BoundingFrustum`;
- `ContainmentType` (`DISJOINT`, `INTERSECTS`, `CONTAINS`);
- `PlaneIntersectionType` (`FRONT`, `INTERSECTING`, `BACK`);
- triangle/ray, triangle/triangle, triangle/plane, and six-plane containment
  functions.

Each volume supports a broad cross-product of containment and intersection
tests with points, triangles, spheres, AABBs, OBBs, frusta, planes, and rays.
Volumes can be constructed from points, transformed, and queried for corners.
The frustum can be constructed from a projection matrix and can emit its six
planes.

This capability matrix is a strong minimum target for Move's bounds/frustum
module.

## Result and invariant limitations

DirectXCollision generally returns:

- boolean overlap;
- a containment/plane classification enum;
- ray distance through an output parameter.

It does not return a common rich hit structure with point, normal,
barycentrics, entry/exit, or inside-start status. The triangle-ray test returns
distance but not barycentric coordinates. Move should retain the lightweight
boolean/classification fast path and add typed detail queries where the
algorithm already computes useful values.

OBB orientation is documented as a unit quaternion but stored as `XMFLOAT4`;
the type does not enforce the invariant. Ray direction is an `XMVECTOR` and
relies on documented normalization assumptions. Move can improve both through
semantic types.

## Flexibility and performance lessons

DirectXMath's most important positive patterns are:

- explicit packed/storage versus compute representations;
- platform-opaque compute types;
- no virtual dispatch;
- specialized calling conventions;
- separate packed-format and collision headers;
- batch array transform functions;
- small POD collision volumes.

Its limitations for Move's goals are:

- single-precision emphasis in collision and SimpleMath;
- DirectX-flavored naming/conventions;
- function-family breadth that is difficult to discover;
- weak semantic distinction among four-lane values;
- output-parameter result style;
- a facade that inherits storage types and converts on each operation boundary.

## Adopt

- explicit storage/compute boundary and opaque backend representation;
- modular core, packed, collision, and convenience layers;
- POD/trivially-copyable bounding volumes;
- AABB, OBB, sphere, and frustum cross-query coverage;
- three-state containment and plane classification;
- frustum construction from projection plus direct plane/corner access;
- batch transforms and packed normal/color formats;
- center/extents OBB with invariant rotation.

## Adapt rather than copy

- Keep `Vec3f` as a compute-friendly value without inheriting a packed storage
  struct; provide explicit `PackedVec3f`.
- Use `Rotation3f` rather than an arbitrary four-float OBB orientation.
- Use `Direction3f` for ray direction.
- Return values normally in modern C++, reserving output spans for batches.
- Provide `Test`/classification fast paths plus richer `Intersect` results.
- Support float and double where algorithms and backends permit.
- Isolate RTM/DirectXMath adapters so selecting a backend does not change
  public identity.

## Avoid

- exposing backend calling-convention aliases in the public facade;
- making every semantic value a raw four-lane vector;
- inheritance from storage structs;
- bool-plus-distance as the only ray-query result;
- one giant header for primitive math, collision, packed formats, and
  formatting;
- assuming a DirectX coordinate/projection convention without naming it.

