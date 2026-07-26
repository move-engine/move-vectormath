# Unity math API review

## Scope

This review covers the Unity 6.5 `UnityEngine` scripting types and Unity
Mathematics 1.3.1. The former optimizes gameplay discoverability; the latter
provides shader-like data-oriented types intended to work well with Burst and
SIMD. Their coexistence is itself instructive.

## API shape

`UnityEngine.Vector2/3/4`, `Quaternion`, `Matrix4x4`, `Transform`, `Bounds`,
`Ray`, and `Plane` are small PascalCase value/object types with public
properties and method-heavy discoverability. `GeometryUtility` groups
frustum/plane operations. Physics queries live outside the pure math types and
return engine-specific hit structures.

Unity Mathematics uses lowercase `float2/3/4`, `floatNxM`, `quaternion`,
`RigidTransform`, and a broad `math` function surface. Its naming deliberately
resembles shader languages. It provides a more composable low-level vocabulary
but fewer semantically rich types.

## Game-loop usability strengths

- `Vector3` covers frequent gameplay operations directly: `MoveTowards`,
  `RotateTowards`, `SmoothDamp`, `Project`, `ProjectOnPlane`, `Reflect`,
  `Angle`, `SignedAngle`, `ClampMagnitude`, `Lerp`, and `Slerp`.
- `Bounds` makes center/extents, min/max, size, closest point, containment,
  expansion, squared distance, AABB intersection, and ray intersection easy to
  discover.
- `Ray` has an origin, direction, and `GetPoint(distance)`, which reads
  naturally in gameplay code.
- `Plane` provides signed-side tests, distance, closest point, flipping, and
  ray casting.
- `GeometryUtility` offers frustum-plane extraction and AABB testing without
  requiring users to write extraction math.
- `Transform` makes local/world point, vector, and direction conversion
  explicit in method names.
- Unity Mathematics provides a compact `RigidTransform` of unit quaternion plus
  translation, an important alternative to a general matrix.
- Euler construction in Unity Mathematics exposes explicit order-specific
  factories such as `EulerZXY` and documents radians.

Unity's strongest usability pattern is vocabulary shaped around common intent:
move, rotate, look, project, transform point, and test bounds. Users do not
need to assemble these operations from low-level primitives.

## Semantics and safety

The API does not structurally distinguish points, displacement vectors, unit
directions, and normals. A `Vector3` can represent any of them. Normalization
returns zero for inputs too small to normalize, which is safe but silently
loses direction and forces downstream operations to keep guarding.

`Quaternion` is intended to represent rotation, but public components and
general construction do not make unit length a compile-time invariant.
Likewise, `Ray.direction` is a `Vector3`; its unit-length expectations depend on
the operation.

The engine has both degree-oriented gameplay APIs and radian-oriented
Unity Mathematics APIs. This is practical within each layer but makes crossing
the boundary an easy source of unit mistakes.

## Coordinate and transform conventions

Unity's transform and matrix APIs are approachable but require documentation
knowledge:

- `Matrix4x4` is documented as column-major.
- Unity engine coordinates use Y up and Z forward for common scene semantics.
- Projection and graphics-backend details are partly abstracted by the engine.
- Gameplay `Transform` is usually preferred over manually manipulating
  matrices.

V2 should not depend on users learning conventions from scattered pages. The
equivalent choices should be centralized in one conventions header/document,
with explicit projection variants where platform differences matter.

## Geometry and queries

Unity provides practical baseline geometry:

- AABB through `Bounds`;
- infinite rays and planes;
- plane/AABB frustum tests;
- engine physics ray, sphere, capsule, and box casts with rich hit data.

The pure scripting math surface does not provide a general OBB value type,
triangle value type, typed pure-math intersection family, or reusable prepared
query. Physics fills many gameplay needs, but a standalone C++ math library
cannot defer those facilities to an engine.

`Bounds.IntersectRay` can return a boolean or an output distance. This is easy
to call, but the overload/result model scales poorly when normal,
entry/exit distance, or inside-start status is needed.

## Performance and compilation lessons

Unity demonstrates a useful split:

- ergonomic engine types for broad usage;
- data-oriented, shader-shaped types for high-throughput Burst code.

Move does not need two incompatible naming systems, but it should support the
same two concerns through explicit storage/compute types and modular
high-level facilities. A thin core should not include bounds, physics-like
queries, formatting, or engine integration automatically.

Unity Mathematics also demonstrates that `RigidTransform` deserves a native
representation instead of forcing every transform into a 4x4 matrix.

## Adopt

- PascalCase, intent-oriented gameplay operations.
- `MoveTowards`, magnitude limiting, projection, signed angle, and smooth
  interpolation facilities.
- A discoverable AABB with center/extents and min/max views.
- `Ray::PointAt(distance)` semantics.
- explicit local/world point, vector, direction, and normal transforms.
- compact rigid transform.
- explicit Euler order and angle units.
- direct frustum extraction and AABB classification.

## Adapt rather than copy

- Replace raw vector ray directions with invariant `Direction3`.
- Return typed query results instead of bool-plus-out-parameter overloads.
- Keep pure geometry separate from physics/world queries.
- Use one consistent PascalCase surface rather than duplicating
  UnityEngine-style and shader-style APIs.
- Express points and directions as distinct types where doing so eliminates
  ambiguity without making ordinary vector arithmetic cumbersome.

## Avoid

- relying on documentation alone to preserve quaternion/direction invariants;
- mixing degree and radian scalars without type distinction;
- letting an engine-global transform object substitute for lightweight
  `RigidTransform3` and `AffineTransform3` values;
- a geometry utility grab bag whose return types vary by scripting convenience.

