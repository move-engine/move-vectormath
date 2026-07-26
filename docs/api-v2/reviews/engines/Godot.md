# Godot math API review

## Scope

This review uses the stable Godot documentation for `Vector3`, `Quaternion`,
`Basis`, `Transform3D`, `AABB`, `Geometry3D`, and `Camera3D`.

## API shape

Godot exposes compact built-in value types to its scripting languages.
`Vector3` is a generic coordinate/triplet type with public components and a
large method set. `Basis` represents a 3x3 rotation/scale/shear basis;
`Transform3D` combines it with an origin. `Geometry3D` is a static namespace for
primitive construction and queries.

The engine can be compiled with float or double `real_t`, providing a global
precision policy rather than a distinct type at each call site.

## Game-loop usability strengths

Godot's vector surface is broad and practical:

- direction, distance, squared distance, dot/cross;
- finite, approximately zero, and normalized checks;
- length limiting and move-toward;
- project, reflect, slide, bounce, rotate, and signed angle;
- linear, spherical, cubic, time-aware cubic, and Bézier interpolation;
- snapping, component extrema, and octahedral normal encoding.

`AABB` is particularly complete:

- position/size representation with center, end, endpoints, support point, and
  volume;
- expansion, growth, merge, intersection, containment, and enclosure;
- point, plane, ray, and segment tests;
- finite/volume/surface checks;
- explicit handling guidance for negative sizes.

`Geometry3D` includes closest points, barycentric coordinates, convex hull
helpers, polygon clipping, ray/triangle, segment/triangle, segment/sphere,
segment/cylinder, and segment/convex tests.

`Camera3D` exposes frustum planes, point-in-frustum, behind-camera,
screen-to-ray, and world/screen projection helpers at the level gameplay code
needs.

## Coordinate clarity

Godot does an excellent job documenting direction constants:

- scene/camera forward is -Z;
- imported model front is +Z;
- separate `FORWARD` and `MODEL_FRONT` families make this mismatch visible.

This is a model for Move documentation. A generic library may not choose both
engine and asset conventions, but it should name every convention and make
conversion points explicit.

Angles returned by `Vector3` are documented in radians. Axis-angle operations
state their normalized-axis expectations.

## Semantics and safety

`Quaternion` is documented as a unit quaternion for rotation, but public
components and constructors allow non-unit values. Documentation warns that
the value must be normalized and that conversion from a non-orthonormal
`Basis` can fail/fall back. This is precisely the gap a `Rotation3` invariant
type can close.

`Transform3D` is semantically clearer than a 4x4 matrix, but its `Basis` may
contain rotation, scale, and shear. Inverse choices therefore include
orthonormal and affine variants. Move should encode rigid versus affine
capability in the type rather than relying on the caller to select the correct
inverse.

The `Vector3` documentation explains many preconditions, but points,
directions, and normals remain the same type. `is_normalized()` diagnoses an
invariant; it does not preserve one.

## Geometry result design

Godot's script-friendly results sometimes use `Variant`/`null` or packed arrays
whose element positions carry meaning. This is convenient across scripting
bindings but weak for C++:

- a ray/triangle hit should expose a named distance/parameter and barycentrics;
- a closest-points query should return a named pair/result;
- classification should use an enum rather than an array/sentinel convention.

Godot's capability coverage is worth adopting, but Move should provide strongly
typed result structures.

## Performance and compilation lessons

The monolithic built-in type experience is ideal for an engine scripting
language, where engine binaries absorb implementation cost. A header-only C++
library must modularize the same features.

Global float/double precision is simple for an engine build, but a library
benefits from explicit `f`/`d` aliases so large-world and renderer code can
coexist. Batch normal encoding/decoding and culling functions should be
optional modules.

## Adopt

- the breadth and naming of `Vector3` gameplay operations;
- explicit scene-forward versus asset-forward documentation;
- comprehensive AABB operations;
- closest-point, distance, barycentric, segment, and primitive queries;
- `Basis` terminology distinguishing orthogonal, normalized, uniform, and
  orthonormal;
- screen/world ray projection and frustum facilities in a graphics module;
- float/double build and alias support;
- octahedral direction/normal packing as an optional storage module.

## Adapt rather than copy

- Use `Direction3`, `Normal3`, and `Rotation3` invariant types.
- Split `RigidTransform3` from general `AffineTransform3`.
- Return typed query results instead of variants or position-dependent arrays.
- Keep rich operations in focused headers while preserving easy umbrella
  inclusion.
- Provide both min/max and position/size AABB construction, but select one
  canonical invariant representation.

## Avoid

- documenting a quaternion as unit length while allowing all operations to
  invalidate it silently;
- ambiguous inverse methods on transforms whose basis capabilities differ;
- packed arrays as geometry result protocols;
- allowing negative AABB sizes without normalization or explicit unchecked
  construction;
- burying coordinate-space differences in prose instead of type/function
  names.

