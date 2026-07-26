# Garry's Mod Lua math API review

## Scope

This review uses the Facepunch Garry's Mod wiki for `Vector`, `Angle`,
ray/primitive utilities, local/world conversion, and line/hull traces. The API
is a scripting view over Source-derived engine facilities, so it is valuable
for understanding what gameplay authors need at a higher abstraction level.

## API shape

`Vector` and `Angle` are mutable userdata-like values with public/indexed
components, operators, and PascalCase methods. Global `util` functions expose
geometry and engine traces. Inputs and results use Lua tables or multiple
return values where C++ would use typed structures.

The surface emphasizes immediacy: a script can compute aim direction, turn it
into an angle, convert local/world coordinates, project to screen, and trace
into the world with very little setup.

## Game-loop usability strengths

`Vector` directly supports:

- dot/cross and 2D/3D length/distance, including squared variants;
- copied and in-place normalization;
- tolerance equality and zero checks;
- vector-to-angle and angle-with-up conversion;
- in-place rotation;
- point-within-AABB testing;
- conversion to screen coordinates;
- mutable `Add`, `Sub`, `Mul`, and related operations documented as ways to
  avoid allocation and garbage collection.

The wiki explains performance consequences in gameplay language. It explicitly
steers users toward squared length/distance and in-place mutation in hot loops.
That kind of documentation is worth emulating even when C++ value returns have
different cost behavior.

## Geometry and trace usability

The `util` namespace exposes a wide practical set:

- ray/plane, ray/sphere, and ray/OBB intersection;
- box/box, box/sphere, OBB/OBB, sphere/sphere, cone, and ray/ray tests;
- line and hull traces against the world;
- local/world position and angle conversion.

`util.IntersectRayWithOBB` is especially instructive. Despite its name,
`rayDelta` is a finite direction-times-distance segment. A hit returns position,
normal, and fraction; a miss returns `nil` values. It solves a common gameplay
task in one call and provides the information scripts usually need.

`util.TraceLine` takes a configuration table and returns a rich trace-result
table, including geometry and engine-object information. The configuration can
express masks and filters. This is flexible and discoverable in Lua, though not
an appropriate type-erased model for a foundational C++ math library.

## Semantics and safety

As in Source, `Vector` represents points, displacements, directions, normals,
colors, and arbitrary triples. APIs communicate semantic roles through
parameter names only. `Angle` is semantically useful but remains a compound
Euler representation with engine-specific ordering/convention.

The ray/segment naming mismatch is a concrete warning: users readily conflate
an infinite ray, a delta-bounded trace, and a segment unless the type system
and method names distinguish them.

Multiple return values plus `nil` are idiomatic Lua. In C++, Move should use a
typed optional result containing at least parameter/distance, point, normal,
and optional barycentric/face information.

## Performance and compilation lessons

Lua allocation/GC advice does not directly imply that C++ should favor mutable
methods. It does reinforce two principles:

- documentation should distinguish in-place and value-return operations;
- batch/high-frequency APIs should make allocation and recomputation explicit.

The scripting API also shows the value of a thin convenience layer over more
specialized primitives. Move can provide ergonomic one-shot calls implemented
over prepared/backend kernels without duplicating every algorithm.

## Adopt

- gameplay-oriented conversions and query names;
- squared variants and clear hot-loop guidance;
- hit results with position, normal, and normalized segment fraction;
- local/world and screen/world conversion helpers in an optional graphics
  module;
- pure geometry queries separate from engine traces;
- both one-shot and prepared/batch query paths.

## Adapt rather than copy

- Name finite delta-based queries `Segment` or `Cast`, not `Ray`.
- Replace Lua multiple returns/nil with `std::optional<Hit>` or a compact
  `QueryResult`.
- Replace configuration tables with typed query/filter structures at engine
  integration boundaries.
- Prefer immutable C++ value operations, with explicit in-place methods only
  where they are clearer or measurably useful.

## Avoid

- a universal `Vector` semantic role;
- query names whose mathematical domain does not match their actual extent;
- mixing pure intersection math with entity/material/filter behavior;
- arbitrary tables or tuples for core C++ query results;
- engine-context-dependent operations such as `ToScreen` in the foundational
  vector header.

