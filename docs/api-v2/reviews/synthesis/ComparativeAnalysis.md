# Comparative analysis

## What game and graphics users actually need

Across the reviewed APIs, the recurring workload is more stable than the
surface naming:

1. update positions, velocities, and orientations;
2. interpolate camera/animation state;
3. convert between local, world, view, clip, and screen spaces;
4. construct compact rigid/affine transforms;
5. maintain and transform bounds;
6. cast rays/segments and compute closest points;
7. cull volumes against frusta;
8. pack data for GPU, networking, or storage;
9. repeat those operations over arrays without redundant work.

Move v1 covers the low-level arithmetic in steps 1–4 but lacks most of the
semantic and geometry vocabulary that makes the remainder straightforward.

## The facade should be thin, not small in capability

A thin facade means that public types and common operations add little
implementation machinery over optimized primitives. It does not mean omitting
useful geometry.

The reviewed APIs suggest three layers:

```text
semantic/game API
    Point, Direction, Rotation, Transform, Aabb, Ray, Frustum
                 ↓ shared algorithms
core value API
    Vec, Mat, Quat representation and small primitive vocabulary
                 ↓ selected backend primitives
backend/storage API
    scalar, RTM/SIMD, packed formats, loads/stores
```

Unity Mathematics and DirectXMath make the bottom two layers visible. Unity
Engine, Godot, and Unreal emphasize the top layer. Move can provide all three
without duplicating algorithms by keeping the dependency direction one-way and
splitting headers by capability.

## Semantic types are a performance feature

Semantic types are often treated only as safety wrappers. In this domain they
also remove work:

- `Direction3` proves length is one at API boundaries, so projection,
  reflection, ray distance, basis, and plane routines need not normalize or
  divide by squared length repeatedly.
- `Rotation3` proves a quaternion is valid for rotation, so inverse is
  conjugate and vector rotation can use the specialized unit-quaternion path.
- `RigidTransform3` proves there is no scale/shear, so inverse and composition
  avoid a general matrix inverse/multiply.
- `Aabb3` with ordered min/max removes repeated canonicalization.
- `Obb3` with nonnegative half-extents and a valid rotation removes validation
  inside every query.
- `PreparedRay3` caches reciprocal direction/sign data when testing many
  bounds.

The type system cannot prove a runtime float has exact mathematical unit length
at compile time. It can prove that callers obtained the value through a
constructor/operation that enforces the library's unit-length tolerance and
that they cannot mutate components afterward. That is the useful guarantee.

## Safe and fast must describe contracts

The comparisons expose three common failure modes:

- one safe operation silently returns a fallback;
- one unchecked operation assumes undocumented inputs;
- every operation revalidates inputs even when validity is already known.

Move should use:

- invariant input types for common structural preconditions;
- `Try...` construction when runtime data may be invalid;
- deliberate fallback construction when a caller supplies a fallback;
- conspicuously named `Unchecked` or internal constructors for proven data;
- separate `Intersects` fast predicates and `Intersect` detail queries;
- debug validation that can be enabled without changing release semantics.

This follows Unreal's useful safe/unsafe distinction while moving frequent
preconditions into types.

## Result types should match the question

The reviewed APIs use booleans, output parameters, sentinel vectors, optional
variants, arrays, Lua multiple returns, and rich engine trace structures.
Move should support three levels:

- predicate: `Intersects(a, b) -> bool`;
- classification: `Classify(a, b) -> Containment`;
- detailed query: `Intersect(a, b) -> optional<Hit>`.

Detailed results should be specialized where the information differs. A
triangle hit has barycentrics and face orientation; an AABB ray hit has entry
and exit parameters and an inside-start state. A universal oversized hit type
would waste space and obscure meaning.

## Coordinate conventions must be API

Godot's separate scene-forward and model-front constants are the strongest
example of convention clarity. Unreal's +X-forward/+Z-up constants are also
prominent. Move must document and test:

- handedness;
- right/up/forward axes;
- row- versus column-vector convention;
- matrix element/storage ordering;
- transform composition order;
- quaternion multiplication meaning;
- angle units and Euler order;
- clip-space depth and Y direction;
- near/far extraction, including reverse-Z and infinite-far projections.

Projection constructors should name relevant policy rather than assuming one
graphics API. Asset, camera, and render conventions may need explicit adapters.

## Header-only design needs stricter boundaries than engine APIs

Unity and Godot hide implementation in engine binaries. Unreal and Source can
accept large engine headers within their own build ecosystems. Move is consumed
directly as C++ headers, so its organization must be closer to DirectXMath:

- core scalars and vectors;
- rotations;
- matrices;
- transforms;
- geometry primitives;
- query families;
- frustum/culling;
- packed formats;
- backend adapters;
- formatting and serialization;
- umbrella header.

Core vector code must not include collision, formatting, serialization, or
both backend implementations.

## Flexibility without public backend complexity

The Move v1 design tried to avoid duplication while allowing optimized vector
representations. That goal remains correct. The problem is exposing its
mechanism through public inheritance and template parameters.

V2 can keep one implementation of high-level operations by defining a compact
backend primitive contract. The selected backend supplies load/store,
arithmetic, shuffle, dot/cross, compare/select, and reciprocal/sqrt primitives.
Public `Vec3<T>` delegates to that contract but does not name the backend in its
type parameters. Semantic algorithms build on public/core primitives once.

Scalar and RTM tests should compile as separate targets. A consumer selects one
backend consistently for the target; it should not include and instantiate
both in every translation unit.

## Precision strategy

Unreal's large-world work and Godot's build-wide precision choice show that
double support matters. DirectXCollision and SimpleMath show the simplicity and
speed of a float-first surface.

Move should provide:

- primary float aliases for rendering/gameplay;
- corresponding double aliases for world/authoring use;
- templates internally/publicly where component type is meaningful;
- explicit narrowing;
- no promise that every packed format or SIMD backend supports every scalar
  equally;
- query algorithms that preserve the component type unless a documented
  promoted result is necessary.

## Overall position

The target should combine:

- DirectXMath's storage/compute and modularity discipline;
- Unity/Godot's intent-oriented vocabulary;
- Unreal's production geometry breadth and prepared/unsafe paths;
- Source/GMod's game-loop cost awareness;
- a stronger semantic type system than any reviewed API.

That is a coherent niche: a standalone, SIMD-capable C++ game/graphics math
library that remains approachable at the call site and makes common invariants
both safer and cheaper.

