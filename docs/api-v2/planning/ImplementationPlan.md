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

- Propose compute, packed CPU storage, and explicit GPU transfer types.
- Define point, direction, normal, rotation, and transform semantics.
- Define bounds, primitives, intersection results, and frustum facilities.
- Specify invariants, construction, error behavior, component offsets, array
  strides, matrix order, layouts, and costs.
- Specify contiguous and strided fused conversion paths for interleaved game
  and graphics data.
- Propose backend policy and header organization.

## 3. Design checkpoint

Present the full research and proposal before changing the core API.

## 4. Core implementation

After approval:

1. Introduce `mv::math` and PascalCase conventions.
2. Extract scalar and RTM primitive policies.
3. Prototype native-backed and fixed-layout compute vectors.
4. Implement compact storage and initial GPU transfer layouts.
5. Implement contiguous/strided fused load-operate-store kernels.
6. Implement semantic vector and rotation types.
7. Implement matrices and transform representations.
8. Implement primitives, bounds, queries, and frustum support.
9. Split core and extension headers.
10. Port and extend tests and benchmarks.
11. Compare v2 against the retained v1 baseline.

There will be no legacy compatibility surface.

## Deferred companion track

After the CPU functionality needed by the first Move 1.x releases is usable,
add an opt-in Slang companion for shader-side semantic types, geometry,
queries, culling, and transfer-layout reflection. It mirrors portable
contracts and shares conformance data; it does not generate or replace the
production C++ implementation. See
[`../design/SlangCompanion.md`](../design/SlangCompanion.md).
