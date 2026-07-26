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

The reviewed APIs suggest four concerns with one-way dependencies:

```text
semantic/game API
    Point, Direction, Rotation, Transform, Aabb, Ray, Frustum
                 ↓ shared algorithms
core compute API
    SIMD-friendly Vec, Mat, Quat and small primitive vocabulary
                 ↓ selected backend primitives
backend API
    scalar or RTM/SIMD primitive implementation

storage/transfer API ── explicit load/store ──► core compute API
    compact CPU, GPU layouts, encoded formats, strided views
```

Unity Mathematics and DirectXMath make the lower layers visible. Unity Engine,
Godot, and Unreal emphasize the top layer. Move can provide all four concerns
without duplicating algorithms by keeping backend choice out of public
representation identity and splitting headers by capability.

## Familiarity is evidence, not authority

Popular engine APIs reveal which terms users will search for and which tasks
deserve direct support. They do not establish that every contract is sound.
Unity is especially important as an on-ramp and especially risky as a design
template:

- its gameplay vocabulary is highly discoverable;
- a single `Vector3` erases point/direction/normal distinctions;
- approximate `operator==` differs from exact `Equals`;
- `Lerp` clamps in `UnityEngine` but not in Unity Mathematics;
- normalization can silently return zero;
- transform helpers differ in their treatment of scale;
- scene hierarchy behavior and pure transform math share one `Transform`;
- TRS and `lossyScale` expose representational caveats only at runtime;
- matrix helper names expose 3x4 versus 4x4 mechanics rather than affine versus
  projective intent.

Move should retain honest, familiar terms such as `Dot`, `Cross`,
`MoveTowards`, `TransformPoint`, `Bounds` as a documentation search term, and
ray point evaluation. It should deliberately differ where semantics improve:
exact equality, named tolerance, explicit clamping/fallback, angle wrappers,
semantic spatial types, and capability-specific transform values.

Approachability is supplied by short names, task examples, predictable
overloads, search metadata, and a Unity migration guide—not by a duplicate
compatibility facade. This keeps the API coherent and avoids adding headers,
templates, and maintenance work solely for familiarity.

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

## Practical storage and GPU paths

V1's fast-versus-storage motivation should be preserved. A 16-byte compute
three-vector can remain SIMD-resident across an operation chain, while a
12-byte three-vector can reduce cache and transfer bandwidth. Neither is
universally faster; the crossover depends on working-set size and operations
per load.

GPU transfer adds a third representation concern. A compact 12-byte float3 is
valid for some vertex, structured, Metal packed, or scalar-layout uses, while
Vulkan/WGSL float3 arrays commonly require 16-byte alignment and stride. HLSL
constant buffers pack within 16-byte registers, and matrix major order/vector
stride remain separate choices. Therefore:

- `PackedVec3f` means dense 12-byte storage, not “works in every shader”;
- GPU transfer types state payload size, alignment, stride, and matrix order;
- a compute span can be uploaded without conversion only when layout traits
  prove compatibility;
- interleaved vertex/particle/instance fields use strided views;
- batch kernels fuse load, computation, and output layout conversion;
- SoA/AoSoA remains an optimization selected from measured workloads.

This provides the ergonomic choice v1 intended without restoring
`Acceleration` to public type identity. The complete review and proposal are
in
[`StorageComputeAndGpuInterop.md`](StorageComputeAndGpuInterop.md) and
[`DataLayoutAndGpuInterop.md`](../../design/DataLayoutAndGpuInterop.md).

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
- familiar Unity/Godot vocabulary after filtering it through explicit
  contracts;
- Unreal's production geometry breadth and prepared/unsafe paths;
- Source/GMod's game-loop cost awareness;
- explicit CPU-storage and GPU-transfer layout contracts;
- a stronger semantic type system than any reviewed API.

That is a coherent niche: a standalone, SIMD-capable C++ game/graphics math
library that remains approachable at the call site and makes common invariants
both safer and cheaper.
