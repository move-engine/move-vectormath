# Source SDK math API review

## Scope

This review inspects Valve's current public `source-sdk-2013` repository,
especially `mathlib/vector.h`, `mathlib/mathlib.h`, `mathlib/vmatrix.h`, and
`cmodel.h`. It represents an older C++ game-engine style that is unusually
explicit about cost and hot-loop behavior.

## API shape

`Vector` is a concrete three-float type with public `x/y/z`, array access, many
member operations, and a parallel family of free functions/macros such as
`VectorAdd` and `VectorMA`. `VectorAligned` supplies a 16-byte-aligned form.
Matrices, quaternions, angles, planes, and collision structures are spread
across mathlib and engine headers.

The surface mixes object-oriented C++, C-like output-parameter functions,
macros, platform-specific optimizations, and debug validation.

## Game-loop usability strengths

The API reflects actual game code:

- `Length`, `LengthSqr`, 2D variants, `DistTo`, and squared distance;
- in-place normalization returning original length;
- `WithinAABox`;
- min/max, dot, cross, and multiply-add;
- `PointWithinViewAngle`, designed to avoid a square root;
- validity/invalidation helpers;
- aligned vectors for SIMD boundaries;
- optional disabling of "slow operations" through
  `VECTOR_NO_SLOW_OPERATIONS`.

The methods make cost visible. Squared operations are prominent, and
performance-sensitive alternatives exist for common gameplay questions.

## Ray and swept-volume semantics

`Ray_t` is more than an infinite mathematical ray:

- `m_Start` and `m_Delta` describe a finite start-to-end sweep;
- `m_Extents` turns it into an axis-aligned box sweep;
- `m_StartOffset` centers asymmetric extents;
- `m_IsRay` and `m_IsSwept` precompute useful classifications;
- `InvDelta()` computes reciprocal delta with a finite sentinel for zero
  components.

This is a strong engine query object but a poor name for a general math type.
It conflates ray, segment, and swept AABB because those cases share a trace
implementation. Move should preserve the optimization opportunity while using
separate semantic types:

- `Ray3` for an infinite origin plus unit direction;
- `Segment3` for finite endpoints/parameter range;
- `SweptAabb3` or an engine adapter for hull traces;
- prepared internal/query data derived from any of the above.

## Semantics and safety

`Vector` may be default-uninitialized outside paranoia/debug configurations.
It can be invalidated to NaNs and checked with `IsValid`. These tools help find
engine bugs but do not provide a safe value-type default.

Points, vectors, directions, and normals all share `Vector`. Unit assumptions
are conventional. `AsVector2D` uses representation reinterpretation, and
pointer/base access exposes layout. This enables old code patterns but is too
fragile for the primary v2 API.

Source's mutation-heavy/output-parameter APIs avoid temporaries in an era and
compiler model where that mattered more. Modern C++ value returns should be the
default unless measurement shows a real loss; explicit output spans/batch APIs
are more useful than per-operation out parameters.

## Coordinate and transform conventions

Source convention knowledge is distributed throughout engine code and tools.
`QAngle`, vectors, matrix types, and trace APIs expose game-oriented concepts,
but the foundational types do not create a single explicit convention boundary.
That historical flexibility also produces conversions and naming that new
users must learn socially.

Move should make axes, handedness, angle units, matrix/vector multiplication,
and clip-space policy explicit and test them in one place.

## Geometry and queries

Source contains extensive engine collision/trace functionality, but much of it
is coupled to BSP, entity filtering, contents masks, collision models, and
physics. The reusable lesson is the shape of query data:

- precompute whether a query is swept;
- share ray and hull broad-phase machinery internally;
- return a rich trace containing fraction, hit position/normal, surface, start
  solid/all solid, and object information at the engine layer;
- use squared and reciprocal forms to avoid repeated expensive operations.

The standalone Move layer should return geometry-only hit information and let
engine adapters add materials/entities/masks.

## Performance and compilation lessons

Source is strongly cost-aware but header organization is historically broad.
Large headers, macros, inline implementation, platform branches, and legacy
compatibility increase compile and maintenance cost. It demonstrates that
performance vocabulary is valuable, while also showing why implementation and
platform machinery should not dominate public type identity.

`MulAdd` is worth retaining as an intentional primitive because it maps well to
both scalar and SIMD/FMA implementations and is common in integration loops.
Cost-specific helpers such as view-cone testing should live in a focused
gameplay/geometry algorithms module rather than bloating `Vec3`.

## Adopt

- prominent squared-distance/length operations;
- in-place normalize returning original length or explicit success/result;
- multiply-add and component-wise min/max primitives;
- query preparation and reciprocal direction for repeated bounds tests;
- explicit finite/debug validation hooks;
- aligned compute and packed storage forms;
- rich trace-result concepts at the appropriate integration layer.

## Adapt rather than copy

- Separate ray, segment, and swept volume at the public semantic level, sharing
  an internal prepared query representation.
- Use explicit `NoInit` only where measured, keeping safe default construction.
- Use spans/batch algorithms instead of C-style pointer/output overload
  proliferation.
- Keep engine collision filters and material/entity data outside pure math.

## Avoid

- type punning between vector dimensions;
- public layout/pointer escape hatches as the ordinary API;
- macros duplicating normal operators and functions;
- sentinel `FLT_MAX` reciprocal semantics without a named prepared type;
- one `Ray` structure that actually represents three different query kinds;
- compile switches that remove ordinary value-return operations from the
  public type.

