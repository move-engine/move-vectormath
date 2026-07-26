# Review methodology

Each API is evaluated using the same game/graphics-oriented rubric.

## Scope

1. Naming, consistency, discoverability, and common game-loop ergonomics
2. Vectors, points, directions, normals, rotations, matrices, and transforms
3. Coordinate system, handedness, matrix layout, and depth-range conventions
4. Packed storage, compute representations, SIMD, and batch processing
5. Normalization, finite-value handling, tolerances, and invalid input
6. Angles, interpolation, splines, projections, and coordinate conversion
7. Rays, lines, segments, planes, triangles, spheres, capsules, AABBs, and OBBs
8. Intersection, containment, closest-point, distance, and sweep queries
9. Frustum construction, extraction, classification, and culling
10. Serialization, reflection, editor, and scripting ergonomics
11. Header granularity, generic complexity, and likely compilation cost
12. Features worth adopting, avoiding, or measuring

## Evidence

Reviews prioritize current official documentation and first-party source.
Versioned documentation is preferred. Observations inferred from implementation
details are identified as inferences rather than documented API guarantees.

The current Move API is inspected at the permanent v1 baseline commit recorded
in [`../baselines/README.md`](../baselines/README.md). External APIs are
reviewed as they exist during this prepass; exact versions and links are
recorded in [`Sources.md`](Sources.md).

## Interpretation

The comparison does not assume that the largest API is the best API. Features
are evaluated for:

- semantic clarity;
- frequency in real game and graphics code;
- ability to prevent misuse;
- predictable runtime and storage cost;
- ability to produce specialized code without duplicating the public API;
- cost imposed on translation units that do not use the feature.
