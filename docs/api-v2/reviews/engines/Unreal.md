# Unreal Engine math API review

## Scope

This review uses the Unreal Engine 5.8 Core, Engine, and GeometryCore API
documentation. Unreal offers the broadest game-focused surface in the
comparison and shows both the benefits and costs of a mature engine math API.

## API shape

Core types use PascalCase Unreal naming: `TVector`, `TQuat`, `TMatrix`,
`TTransform`, `TBox`, `TBoxSphereBounds`, and aliases such as `FVector`.
`FMath` contains a very large collection of scalar, interpolation, geometry,
random, and query functions. Additional types and algorithms are distributed
through Engine and GeometryCore.

`TVector` is a component-type template with public `X/Y/Z`. In 5.8, float and
double specializations/aliases coexist; the default `FVector` ecosystem has
moved toward double precision for large-world support.

## Game-loop usability strengths

The API is saturated with real production use cases:

- vector length, squared length, 2D length, safe/unsafe normalization,
  clamping, projection, grid snap, heading, plane projection, basis generation,
  finite checks, and conversion to orientation;
- explicit degree and radian rotation helpers;
- `TTransform` storing quaternion, translation, and scale;
- bounding box, oriented box, sphere, combined box/sphere bounds, convex
  volume, and frustum helpers;
- ray/plane, line/box, line/sphere, segment/plane, triangle, closest-point, and
  many other geometry operations;
- local performance-oriented overloads, such as line/box intersection with
  caller-provided reciprocal direction;
- safe and unchecked variants where cost and preconditions differ;
- explicit zero-initialization and no-initialization construction paths.

The API often gives users both a convenient path and a lower-level path that
avoids recomputing known data.

## Semantics and safety

Unreal documents assumptions carefully but generally encodes them by naming
rather than types:

- `GetSafeNormal` versus `GetUnsafeNormal`;
- `ProjectOnToNormal` assumes its argument is unit length;
- `RotateAngleAxis` assumes a unit axis;
- ray/plane helpers may be explicitly unchecked;
- `Normalize` returns whether normalization succeeded.

This is better than one ambiguous operation, but still permits callers to pass
invalid normals and axes. A Move `Direction3` or `UnitVec3` could turn several
of these documented assumptions into overload constraints and remove repeated
checks.

Unreal's `TTransform` is semantically richer than a matrix but includes
non-uniform scale. Composition order and negative scale remain subtle, and
general matrix fallback is sometimes necessary. Move should distinguish rigid
and affine transforms rather than naming one scaled representation simply
`Transform`.

## Coordinate and numeric conventions

Unreal prominently exposes its directional constants:

- +X forward;
- +Y right;
- +Z up.

It also makes degree/radian distinctions visible in several method names.
Large-world support motivates float/double variants and is an important
flexibility lesson: Move should not hardcode every semantic/geometry type to
float even if float aliases are primary.

## Geometry and query design

Notable facilities include:

- center/extents AABB and OBB forms;
- OBB orientation as unit axes or quaternion depending on subsystem;
- `TBoxSphereBounds`, which carries AABB and sphere information for cheap
  staged culling;
- `FConvexVolume` frustum/plane classification with a fully-contained output;
- `GetViewFrustumBounds` from view-projection data;
- broad primitive containment/intersection coverage;
- output parameters for hit points/distances and boolean success.

The combined bounds type is especially useful in renderer-oriented code:
different stages can choose a cheap sphere or tighter box without separately
maintaining two objects.

The performance overload that accepts reciprocal ray direction demonstrates a
general pattern Move should adopt as an explicit `PreparedRay3` or
`RayBoxQuery3`: cache derived data when one ray is tested against many boxes,
while keeping the ordinary one-shot call simple.

## API and compilation costs

Unreal's breadth comes with significant surface complexity:

- `FMath` is a grab bag with many overlapping names and aliases;
- functionality is split across Core, Engine, and GeometryCore;
- vectors contain engine serialization, text, networking, diagnostics, and
  clustering helpers alongside math;
- users encounter multiple generations of OBB/bounds APIs;
- heavy engine macros/reflection and broad headers are acceptable inside Unreal
  but inappropriate for a small standalone library.

Move should borrow the capability model, not the monolithic type/header model.
Formatting, serialization, networking, and editor integration should be
adapters.

## Adopt

- PascalCase and explicit coordinate constants.
- safe and preconditioned operation variants.
- caller-selectable initialization for measured hot paths.
- float/double semantic types.
- rigid and affine transform values.
- AABB, OBB, sphere, capsule, plane, triangle, ray, line, and segment support.
- containment classification, not only boolean overlap.
- combined box/sphere bounds as an optional renderer-oriented type.
- prepared reciprocal-direction ray queries.
- view-frustum construction and plane-mask culling.
- finite/NaN diagnostics as opt-in validation.

## Adapt rather than copy

- Use `Direction3`/`Normal3` parameters instead of merely documenting unit
  preconditions.
- Prefer result structs or `std::optional<Hit>` to bool plus multiple out
  parameters.
- Split algorithms by capability rather than building one `FMath` equivalent.
- Keep serialization, strings, networking, and diagnostics outside core vector
  types.
- Use explicit `RigidTransform3` and `AffineTransform3`; avoid one transform
  type that hides difficult scale semantics.

## Avoid

- redundant synonyms such as several names for squared length/distance;
- default-uninitialized construction without an explicit tag;
- multiple unrelated OBB representations in the same primary layer;
- accumulating engine integration responsibilities in foundational math types;
- unchecked functions whose preconditions are not obvious in name or type.

