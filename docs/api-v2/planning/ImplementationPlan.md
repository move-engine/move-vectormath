# Implementation plan

## 0. Permanent baseline

- Integrate and publish the completed v1 audit and benchmark work.
- Pin the benchmark to the permanent library commit.
- Verify every benchmark executable builds.
- Create and publish `feat/math-api-v2`.

## 1. Comparative prepass

- Review the current Move API.
- Review Unity, Unreal Engine, Source, Garry's Mod, and Godot.
- Review DirectXMath and DirectXTK collision/geometry facilities.
- Apply one common game/graphics usability rubric.
- Produce a feature matrix, comparative analysis, and recommendations.

## 2. Semantic design

- Propose compute and packed storage types.
- Define point, direction, normal, rotation, and transform semantics.
- Define bounds, primitives, intersection results, and frustum facilities.
- Specify invariants, construction, error behavior, layouts, and costs.
- Propose backend policy and header organization.

## 3. Design checkpoint

Present the full research and proposal before changing the core API.

## 4. Core implementation

After approval:

1. Introduce `mv::math` and PascalCase conventions.
2. Extract scalar and RTM primitive policies.
3. Implement compute vectors and packed storage.
4. Implement semantic vector and rotation types.
5. Implement matrices and transform representations.
6. Implement primitives, bounds, queries, and frustum support.
7. Split core and extension headers.
8. Port and extend tests and benchmarks.
9. Compare v2 against the retained v1 baseline.

There will be no legacy compatibility surface.
