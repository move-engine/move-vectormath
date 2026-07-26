# Unity math API review

## Scope

This review covers the Unity 6.5 `UnityEngine` scripting types and Unity
Mathematics 1.3.x. The former supplies the vocabulary familiar to a large
population of game developers; the latter provides shader-like data-oriented
types intended to work well with Burst and SIMD. Their coexistence is itself
instructive.

Unity is treated here as an adoption and discoverability reference, not as a
normative source of good API design. A familiar name is evidence that users
will search for it. It is not evidence that its contract, defaults, overloads,
or type model should be copied.

## Executive assessment

Unity succeeds at making common operations easy to find. Names such as
`MoveTowards`, `ProjectOnPlane`, `TransformPoint`, `Bounds`, and `Ray.GetPoint`
form a useful on-ramp for gameplay programmers. Move should preserve that
approachability where the terminology is honest.

Unity is much less convincing as a model for semantic precision. Its APIs
frequently rely on one general-purpose `Vector3`, contextual documentation,
silent fallback behavior, and distinctions whose names expose implementation
details instead of intent. The scene `Transform` type also combines hierarchy
state, mutation policy, and mathematical transformation in one object.

The design target is therefore **familiarity without imitation**:

- preserve searchable nouns and verbs where their meaning is sound;
- make differences involving scale, translation, normalization, units, and
  fallibility visible in the type or operation name;
- reject surprising defaults even when Unity users are accustomed to them;
- provide a short Unity-to-Move guide for intentional differences.

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
- `Transform` gives local/world point, vector, and direction conversion
  distinct, searchable method names.
- Unity Mathematics provides a compact `RigidTransform` of unit quaternion plus
  translation, an important alternative to a general matrix.
- Euler construction in Unity Mathematics exposes explicit order-specific
  factories such as `EulerZXY` and documents radians.

Unity's strongest usability pattern is vocabulary shaped around common intent:
move, rotate, look, project, transform point, and test bounds. Users do not
need to assemble these operations from low-level primitives. That vocabulary
is worth retaining after its contracts have been reviewed independently.

## Design liabilities and surprising contracts

### Equality and interpolation

`Vector3.operator==` performs approximate comparison, while `Vector3.Equals`
performs exact comparison. Code which changes between an operator, a generic
container, and an explicit method can therefore change meaning without making
the tolerance choice visible.

`UnityEngine.Vector3.Lerp` clamps its interpolation parameter. The similarly
named `Unity.Mathematics.math.lerp` permits extrapolation. Each behavior can be
useful, but the shared base name hides a meaningful behavioral difference
between Unity's two math surfaces.

Move should use exact value equality for `operator==`, an explicit
`IsNearlyEqual` operation for tolerance-based comparison, an unclamped
mathematical `Lerp`, and an explicitly named `LerpClamped`.

### Transform helper taxonomy

The three familiar `Transform` helpers have materially different contracts:

- `TransformPoint` applies translation, rotation, and scale;
- `TransformVector` applies rotation and scale but not translation, so it can
  change length;
- `TransformDirection` applies orientation while ignoring translation and
  scale, preserving length.

Those distinctions are documented, but the names alone do not tell a user
whether scale is applied. There is no corresponding `TransformNormal`; treating
a normal like either a direction or a vector is incorrect under general
nonuniform scale.

The matrix surface adds another layer of ambiguity. `MultiplyPoint` supports a
general projective matrix, while `MultiplyPoint3x4` is the faster affine
operation. The latter name exposes the implementation shape rather than the
semantic precondition. Current documentation also describes
`Matrix4x4.MultiplyVector` in terms of the "rotation part", whereas
`Transform.TransformVector` explicitly includes scale. Whether this difference
is intended behavior or documentation shorthand, the user cannot infer the
contract from the common word "vector".

Move should retain `TransformPoint`, `TransformVector`,
`TransformDirection`, and `TransformNormal` as searchable concepts, but make
their valid combinations type-directed:

- `TransformPoint(AffineTransform3, Point3)` applies the full affine transform;
- `TransformVector(AffineTransform3, Vec3)` applies the linear part;
- `TransformDirection(RigidTransform3, Direction3)` applies rotation and
  preserves the direction invariant;
- `TransformNormal(AffineTransform3, Normal3)` applies the inverse transpose
  and renormalizes;
- projective point transformation uses an explicitly fallible name such as
  `TryProjectPoint`, not `MultiplyPoint`.

An affine transform should not offer a rotation-only direction helper that
silently discards scale or shear. A caller that wants that policy should state
it by extracting or supplying a rotation.

### Scene graph behavior mixed with pure math

Unity's `Transform` is both a scene component and a mathematical interface.
For example, `SetParent` defaults to preserving world-space values and modifies
local position, rotation, and scale to do so. That is a reasonable scene-graph
operation, but it is not a property of a lightweight transform value and its
boolean/default policy is easy to overlook.

`lossyScale` exposes a deeper representation issue: a rotated child under a
nonuniformly scaled parent can acquire shear that cannot be represented
accurately by a three-component scale. Unity returns an approximation. This is
an important warning against presenting position/rotation/scale as a closed,
general transform algebra.

Move's math layer should contain explicit value types such as
`RigidTransform3`, authoring-oriented `TrsTransform3`, and closed
`AffineTransform3`. Scene hierarchy mutation belongs in the engine layer. If a
future scene API supports reparenting, policy should be an explicit enum such
as `PreserveWorld` or `PreserveLocal`, without a defaulted boolean.

### Rotations, units, and representation

`Quaternion.eulerAngles` uses degrees and a documented Z-X-Y application order.
Reading the property can produce components very different from those
previously written because Euler representations are non-unique; Unity warns
against incremental read-modify-write use. Unity Mathematics generally uses
radians and offers order-specific factories.

Move should treat Euler angles as an explicit construction/decomposition
format, not stable quaternion state. Angle wrappers and order-specific names
should make units and rotation order visible.

### Silent fallback and quantity ambiguity

`Vector3.normalized` returns zero when the input is too small. This avoids
non-finite output but silently converts a requested direction into a value that
is not a direction. Move should expose `TryNormalize` and, when desired,
`NormalizedOrZero`; constructing `Direction3` or `Normal3` must preserve its
invariant or report failure.

`Bounds.Expand(amount)` increases the size by twice `amount` on each axis
because the amount is added to both sides. Move should distinguish operations
such as `InflatedBy(margin)` from `WithSizeIncreasedBy(delta)` rather than make
the caller remember how an ambiguous quantity is interpreted.

## Semantics and safety

The API does not structurally distinguish points, displacement vectors, unit
directions, and normals. A `Vector3` can represent any of them. That is
convenient for initial learning, but it pushes invalid combinations and
normalization assumptions into runtime conventions.

`Quaternion` is intended to represent rotation, but public components and
general construction do not make unit length a compile-time invariant.
Likewise, `Ray.direction` is a `Vector3`; its unit-length expectations depend on
the operation.

The engine has both degree-oriented gameplay APIs and radian-oriented
Unity Mathematics APIs. This is practical within each layer but makes crossing
the boundary an easy source of unit mistakes.

## Coordinate and transform conventions

Unity's transform and matrix APIs are familiar but require documentation
knowledge:

- `Matrix4x4` is documented as column-major.
- Unity engine coordinates use Y up and Z forward for common scene semantics.
- Projection and graphics-backend details are partly abstracted by the engine.
- Gameplay `Transform` is usually preferred over manually manipulating
  matrices, which makes its implicit scene and hierarchy policies especially
  consequential.

V2 should not depend on users learning conventions from scattered pages. The
equivalent choices should be centralized in one conventions header/document,
with explicit projection variants where platform differences matter.

## Unity-to-Move approachability map

The following keeps familiar search terms while making intentional differences
clear:

| Unity concept | Proposed Move surface | Policy |
| --- | --- | --- |
| `Vector3` | `Vec3f`, `Point3f`, `Direction3f`, `Normal3f` | Keep ordinary vectors easy; use semantic types where validity or transform behavior differs. |
| `Quaternion` | `Rotation3f` for rotations, `Quatf` for quaternion algebra | Do not imply every quaternion is a valid rotation. |
| scene `Transform` | `RigidTransform3f`, `TrsTransform3f`, `AffineTransform3f` | Keep hierarchy state and mutation outside the pure math value. |
| `TransformPoint` | `TransformPoint(transform, point)` | Preserve the familiar verb with typed semantics. |
| `TransformVector` | `TransformVector(transform, vector)` | Apply the full linear part; no translation. |
| `TransformDirection` | rigid/rotation overloads for `Direction3f` | Do not silently discard affine scale or shear. |
| no normal-specific helper | `TransformNormal(affine, normal)` | Use inverse-transpose semantics and preserve normalization. |
| `Vector3.Lerp` | `Lerp`, `LerpClamped` | Make clamping an explicit choice. |
| approximate `operator==` | exact `operator==`, `IsNearlyEqual` | Make tolerance visible at the call site. |
| `.normalized` | `TryNormalize`, `NormalizedOrZero` | Make failure or fallback explicit. |
| `Bounds` | `Aabb3f` | Prefer the precise geometric term; document `Bounds` as a migration search term. |
| `Bounds.Expand` | `InflatedBy`, `WithSizeIncreasedBy` | Name which quantity the argument represents. |
| `Ray.GetPoint` | `Ray3f::PointAt` | Preserve the intent while using the library's naming convention. |
| `Matrix4x4.MultiplyPoint3x4` | `TransformPoint(AffineTransform3f, Point3f)` | Express the affine precondition semantically. |
| `Matrix4x4.MultiplyPoint` | `TryProjectPoint(Mat4f, Point3f)` | Expose homogeneous division and failure. |

Approachability should come from documentation, examples, search aliases in
the generated reference, and predictable vocabulary. It should not require
compatibility aliases that preserve weaker contracts.

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

- PascalCase and honest, intent-oriented gameplay operations.
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
- Retain familiar transform verbs, but constrain them by semantic type and
  make scale, normalization, and projection behavior explicit.
- Provide a Unity migration page with side-by-side task examples and an
  "intentional differences" section.

## Avoid

- treating popularity or familiarity as proof of a sound contract;
- approximate equality hidden behind `operator==`;
- clamping, normalization fallback, or hierarchy preservation hidden behind a
  generic name or default argument;
- conflating a scene node with a pure transform value;
- presenting TRS as closed under arbitrary composition;
- rotation-only helpers on affine transforms that silently discard scale or
  shear;
- implementation-shaped names such as `MultiplyPoint3x4` when the real
  distinction is affine versus projective;
- relying on documentation alone to preserve quaternion/direction invariants;
- mixing degree and radian scalars without type distinction;
- letting an engine-global transform object substitute for lightweight
  `RigidTransform3` and `AffineTransform3` values;
- a geometry utility grab bag whose return types vary by scripting convenience.
