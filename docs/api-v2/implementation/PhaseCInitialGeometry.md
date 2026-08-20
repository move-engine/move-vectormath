# Phase C initial CPU geometry slice

## Purpose

The first Phase C vertical slice supplies a small, production-useful set of
CPU geometry and ray queries for early Move 1.x integration. It tests the
designed primitive, invariant, result, and query-tier conventions without
waiting for the complete primitive cross-product.

This slice deliberately takes priority over the deferred Slang companion. C++
remains the authoritative implementation.

Algorithm sources, exact upstream revisions, and Move-specific deviations are
recorded in [`AlgorithmProvenance.md`](AlgorithmProvenance.md).

## Implemented surface

| Area | Types and facilities |
| --- | --- |
| Linear geometry | `Ray3f`, `Plane3f`, `Triangle3f` |
| Bounds | `Sphere3f`, `Aabb3f` |
| Repeated queries | `PreparedRay3f` reciprocal direction and parallel mask |
| Linear queries | ray/plane and ray/triangle predicates and detailed hits |
| Bounds queries | ray/sphere and ray/AABB predicates and detailed hits |
| Pair predicates | sphere/sphere, sphere/AABB, and AABB/AABB |
| Result types | distinct plane, triangle, sphere, and AABB hit values |

`Intersects` uses a predicate solution path and does not construct detailed hit
points, normals, or barycentrics. `Intersect` reuses the same scalar solution
and adds only the requested result data.

## Behavioral contracts

- A ray is created through `TryFromOriginDirection`; its finite origin and
  normalized direction are construction invariants, so `PointAt(t)` uses
  physical distance. It does not silently clamp a negative caller value.
- Touching primitive boundaries count as intersection.
- `StartsInside` means strictly inside. A ray beginning on a boundary reports
  a distance-zero boundary hit instead.
- A parallel or coplanar ray has no unique ray/plane point hit.
- Triangles are created through `TryFromPoints`, which rejects non-finite
  vertices while retaining degenerate triangles as valid values.
- Triangle back faces are included by default. `BackFaceMode::Cull` is explicit.
- Triangle barycentric XYZ weights correspond to the first, second, and third
  vertices respectively. Degenerate triangles do not produce a hit.
- Sphere and AABB ray intervals are clipped to the ray domain. A strictly
  interior ray reports entry distance zero.
- A zero-radius sphere is valid. Its geometric normal at the center is absent
  rather than fabricated.
- AABB minimum and maximum are finite and ordered. One private sentinel is the
  canonical empty state; empty bounds remain closed under merge/intersection.
- Empty-derived values use fallible accessors (`TryCenter`, `TrySize`,
  `TryHalfExtents`, `TryCorners`, and `TryClosestPoint`).
- Prepared rays treat components whose reciprocal exceeds the finite float
  range as parallel for representable-distance slab queries, avoiding
  zero-times-infinity NaNs on slab boundaries.
- Non-finite ray origins and triangle vertices are rejected at construction.
  Queries rely on those type invariants instead of repeating component scans;
  no query silently repairs invalid values.

## Hot-query organization

`Intersects(PreparedRay3f, Aabb3f)` has a predicate-only Williams slab kernel.
The detailed `Intersect` path separately tracks entry/exit axes, signs, and
inside state. This deliberate kernel split prevents boolean traversal from
paying for detailed-hit bookkeeping while retaining one checked construction
boundary.

## Layout

| Type | Size | Alignment |
| --- | ---: | ---: |
| `Ray3f` | 32 | 16 |
| `Plane3f` | 16 | 16 |
| `Triangle3f` | 48 | 16 |
| `Sphere3f` | 16 | 16 |
| `Aabb3f` | 32 | 16 |

All five primitives are trivially copyable and standard-layout. Plane
coefficients and sphere center/radius fit one four-float slot; semantic access
does not add a tag or dispatch field.

## Verification

Local verification completed with:

- GCC 16 strict Release RTM and forced-scalar builds;
- Clang 22 RTM and forced-scalar builds under AddressSanitizer and
  UndefinedBehaviorSanitizer;
- all 19 legacy, standalone, Phase A, Phase B, and Phase C tests;
- a separate downstream CMake consumer using `Ray3f` and `Aabb3f`;
- GCC and Clang compilation of focused ray, query, and Phase C umbrella
  fixtures;
- 1,024 deterministic varied ray/AABB cases checked against an independent
  double-precision slab reference;
- dedicated code-generation fixtures for prepared ray/AABB batches and
  ray/triangle batches.
- the hosted GCC, Clang, AppleClang/ARM, and MSVC matrix;
- the separate Linux/Windows SSE4.2, AVX, and AVX2 benchmark build and parity
  jobs, plus its diagnostic performance-report job.

Warmed local GCC 16 focused-header measurements used C++20 and `-O2`:

| Fixture | Time | Peak memory | Preprocessed lines |
| --- | ---: | ---: | ---: |
| Ray only | 0.73 s | 140.9 MiB | 88,237 |
| Ray/AABB query | 0.85 s | 155.4 MiB | 90,485 |
| Phase C umbrella | 0.82 s | 157.7 MiB | 91,902 |

These are local directional measurements, not stable regression gates.

The invariant/predicate-kernel follow-up used the separate nanobench suite at
the 4,096-element working set. Hardware-counter results show that the former
hotspots no longer contain semantic-facade overhead:

| Query | Backend | Move instructions/op | Raw `Vec3f` instructions/op |
| --- | --- | ---: | ---: |
| prepared ray/AABB predicate | SSE4.2 | 62.28 | 59.28 |
| prepared ray/AABB predicate | AVX2 | 53.92 | 51.92 |
| ray/triangle predicate | SSE4.2 | 59.44 | 62.30 |
| ray/triangle predicate | AVX2 | 48.59 | 49.30 |

The remaining prepared ray/AABB difference is the canonical-empty AABB test:
Move supports an empty value while the raw benchmark representation does not.
Absolute timings were collected under a variable-frequency powersave governor;
the instruction counts are the comparison evidence.

## Remaining Phase C work

- Add the remaining point/bounds and primitive-pair closest-point and distance
  operations; line, ray, segment, plane, and triangle point queries are now
  implemented.
- Add capsule and OBB values, transforms, and queries.
- Add ray/segment variants and broader primitive-pair coverage.
- Add packed AABB storage and batch traversal evidence.
- Decide the final float-only versus templated/double shape after the public
  float API and algorithm contracts settle.
- Replace proof umbrellas with the final `Core`, `Geometry`, `Queries`, and
  `Spatial` header organization before the Move 1.x integration point.
