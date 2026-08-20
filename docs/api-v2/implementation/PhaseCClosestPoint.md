# Phase C linear primitives and closest-point slice

## Purpose

This slice establishes the linear and closest-point foundation needed by
capsules, OBBs, segment casts, distance queries, and later narrow-phase work.
It remains CPU-first; the deferred Slang companion is unaffected.

## Implemented surface

- `Line3f` stores an origin and unit `Direction3f`; its signed parameter is
  physical distance.
- `Segment3f` stores endpoints and exposes displacement, length, fallible
  direction, degeneracy, and unclamped `PointAtFraction`.
- `ClosestPoints(point, primitive)` returns a primitive-specific result for
  lines, rays, segments, planes, and triangles.
- Result fields distinguish signed line distance, ray distance, segment
  fraction, plane signed distance, and triangle barycentrics.
- `ClosestPoint`, `DistanceSquared`, and `Distance` provide concise overloads
  when the detailed parameter data is unnecessary.
- Point/AABB queries preserve the empty-box contract through
  `TryClosestPoints`, `TryClosestPoint`, `TryDistanceSquared`, and
  `TryDistance`.
- Point/sphere queries use solid bounding-volume semantics: points already in
  the sphere have zero distance and remain unchanged.
- Segment/segment queries return both closest points, both endpoint fractions,
  and squared distance, including when either segment degenerates to a point.

All operations are allocation-free. `Line3f` and `Segment3f` are 32-byte,
16-byte-aligned, trivially copyable values. The RTM and scalar backends share
the public algorithm and differ only in their vector primitives.

## Behavioral contracts

- A line accepts negative and positive distances.
- A ray clamps its closest parameter to zero.
- A segment clamps only closest-query fractions; `PointAtFraction` itself does
  not silently clamp.
- A zero-length segment is valid and resolves to its start at fraction zero.
- Triangle barycentrics use first/second/third vertex order and sum to one.
- Degenerate triangles reduce to the closest of edges 01, 02, and 12, with
  that order breaking equal-distance ties.
- Squared-distance overloads avoid a square root; `Distance` is intentionally
  the derived convenience operation.

The direct mathematical operations expect finite points and primitives.
`IsFinite` is available on the new primitives; a checked/fallible closest-query
layer remains a separate API decision rather than adding hidden validation cost
to these operations.

## Provenance

The short source names and important Move-specific differences are stated next
to each implementation. Exact revisions, licenses, and classifications are in
[`AlgorithmProvenance.md`](AlgorithmProvenance.md):

- Geometric Tools revision `d29e7758ae2615e5e37da3eb573b7bf90ee94e9b`,
  Boost Software License 1.0;
- David Eberly, “Distance Between Point and Triangle in 3D,” created 1999 and
  revised 2020, CC BY 4.0;
- Christer Ericson, *Real-Time Collision Detection*, 2005, sections 5.1.1,
  5.1.2, 5.1.3, 5.1.4, 5.1.5, and 5.1.9;
- David Eberly, “Robust Computation of Distance Between Line Segments,” created
  2018 and revised 2023, CC BY 4.0.

No upstream source was copied verbatim. The triangle query uses the documented
Voronoi-region formulation and adds an explicit lower-dimensional fallback.

## Verification

- fixed line, ray, segment, plane, triangle face, edge, vertex, and degeneracy
  cases;
- identical behavior tests under RTM and forced-scalar builds;
- layout and trivial-copy assertions for the new values and result types;
- a generated loop fixture for batched point/segment closest points;
- a focused-header compile fixture;
- 1,024 deterministic random point/triangle comparisons against a separately
  written double-precision projection-plus-exhaustive-edge oracle.
- 2,048 deterministic random segment-pair comparisons against an independent
  double-precision exhaustive-critical-point oracle.

The separate benchmark repository also verifies 4,096 point/segment cases
against a raw `Vec3f` implementation, GLM 0.9.9.8, and DirectXMath dec2022 for
each SSE4.2, AVX, and AVX2 executable before measuring them.

### Local diagnostic timings

One local powersave-governor run produced the following point/segment
nanoseconds per operation. The machine was not performance-tuned, so these are
directional rather than release claims.

| ISA / count | Move | raw `Vec3f` | GLM | DirectXMath |
| --- | ---: | ---: | ---: | ---: |
| SSE4.2 / 256 | 2.61 | 2.68 | 5.03 | 4.36 |
| SSE4.2 / 4,096 | 2.85 | 2.86 | 5.74 | 5.00 |
| SSE4.2 / 65,536 | 2.92 | 2.88 | 5.70 | 4.99 |
| AVX / 256 | 2.43 | 2.44 | 5.20 | 4.53 |
| AVX / 4,096 | 2.44 | 2.47 | 5.54 | 4.58 |
| AVX / 65,536 | 2.72 | 2.47 | 5.41 | 4.60 |
| AVX2 / 256 | 2.25 | 2.28 | 4.14 | 4.16 |
| AVX2 / 4,096 | 2.32 | 2.37 | 4.90 | 4.45 |
| AVX2 / 65,536 | 2.66 | 2.65 | 4.97 | 4.80 |

Move and its raw baseline execute the same 46, 39, and 38 instructions per
operation under SSE4.2, AVX, and AVX2 respectively. Their timings are generally
within noise except for one large-set AVX sample that should be rerun on a
controlled host. GLM's implementation normalizes the segment direction with a
square root; Move and DirectXMath use a squared-length projection. This run
therefore supports the facade-cost goal without treating shared-runner or
powersave timing as a gate.

## Remaining related work

- implement closest segment/triangle pairs;
- add segment casts against the existing plane, triangle, sphere, and AABB;
- repeat the diagnostic runtime measurements under a controlled performance
  governor before treating small differences as stable.
