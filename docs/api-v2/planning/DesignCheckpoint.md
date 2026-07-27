# API v2 design checkpoint

## Outcome of the prepass

The recommended direction is a breaking `mv::math` PascalCase API that combines:

- a stable, thin public facade over one selected scalar/SIMD backend;
- explicit packed storage and compute values;
- explicit GPU transfer layouts rather than a universal GPU vector type;
- contiguous and strided fused processing for interleaved data;
- zero-overhead semantic types for points, unit directions, normals, and
  rotations;
- rigid/affine transform types that expose their actual capabilities;
- modular game/graphics geometry, queries, and frustum culling;
- predicate, classification, and detailed-result query tiers;
- prepared and batch paths for repeated hot-loop work.

No reviewed competitor combines all of these. In particular, none enforces
point/direction/normal/rotation roles through the type system, while the
existing benchmark evidence shows Move's wrapper approach can compile to the
same instruction count as its backend.

The surface should remain approachable to users arriving from Unity and other
engines, but those APIs are not treated as design authorities. Move retains
honest, searchable vocabulary while making equality tolerance, interpolation
clamping, normalization failure, transform capability, and scene mutation
policy explicit. A Unity on-ramp is documentation, not a second compatibility
facade.

## Recommended answers to open decisions

1. Use `mv::math`, interpreting the namespace migration as `move` → `mv` while
   retaining the math capability namespace.
2. Retain +X right, +Y up, +Z forward, row-major addressing, row vectors, and
   `value * matrix` as core defaults. Require explicit handedness/clip policy
   for view/projection APIs.
3. Keep general `Quat` for algebraic quaternion work and make invariant
   `Rotation3` the primary orientation API.
4. Defer `SimilarityTransform3` until a workload demonstrates its value;
   reserve the concept and implement rigid, TRS-authoring, and affine first.
5. Use component accessors on compute values; give packed storage aggregates
   public PascalCase fields.
6. Use `std::optional` for the first fallible APIs. Introduce a small
   status-bearing result only where tooling needs error detail.
7. Keep predicate queries minimal and specialized detailed hits ergonomic.
   Benchmark whether detailed hit points should be stored or derived.
8. Keep unchecked invariant construction internal initially. Add a public path
   only when measured code cannot express the precondition through semantic
   types.
9. Apply the familiarity-without-imitation policy: preserve familiar names
   where accurate, constrain transform overloads by semantic type, and document
   intentional Unity differences without legacy aliases.
10. Preserve v1's compute/storage choice while separating it from backend
    identity: 16-byte compute values, 12-byte compact storage, and explicitly
    contracted GPU transfer layouts.
11. Make the initial proof cover direct-upload compatibility and fused
    strided conversion, not only single-value `Load`/`Store`.

## Recommended implementation authorization

Approve only Phase A first:

- create the backend primitive contract;
- implement core vectors and packed vector storage;
- implement representative 12-byte and 16-byte GPU float3 transfer layouts;
- prototype native-backed versus fixed-layout `Vec3f`;
- prototype 8-byte versus 16-byte `Vec2f` independently;
- implement one contiguous and one interleaved/strided fused transform path;
- implement angles, `Direction3`, raw `Quat`, and `Rotation3`;
- compile both scalar and RTM targets;
- measure layout, compile cost, generated code, and runtime kernels;
- create the benchmark v2 branch when these types compile.

This phase tests the riskiest architectural claim before committing to the full
geometry surface: hiding the backend should preserve specialized code and
reduce public/compilation complexity without duplicating algorithms.

## Deferred until the proof succeeds

- point/normal and transform implementation;
- geometry primitives and spatial queries;
- frustum/culling;
- packed GPU formats beyond basic storage;
- additional encoded GPU formats and SoA/AoSoA containers;
- formatting, serialization, and third-party adapters.

The designs for these are documented now so the core proof does not choose
representations that block them.

## Current repository state

This document records the pre-implementation checkpoint. Phase A was
subsequently authorized and its proof implementation and measurements are in
[`../implementation/PhaseAArchitectureProof.md`](../implementation/PhaseAArchitectureProof.md).

## Post-Phase-A checkpoint

Phase A passed the supported GCC, Clang, MSVC, and AppleClang/ARM builds,
sanitizer and coverage jobs, source hygiene, and Linux/Windows benchmark
parity. Its evidence supports the thin-facade architecture:

- retain private selected-native `Vec3f` storage without exposing backend
  identity;
- reject the fixed scalar-array `Vec3f` alternative;
- retain explicit compute, compact storage, and GPU transfer boundaries;
- keep focused headers and expand backend primitives only for shared
  algorithms;
- keep `Vec2f` and stable ABI decisions open pending their targeted evidence.

The architecture proof is accepted and Phase B spatial semantics and
transforms are authorized. Geometry, intersections, and frustum work should
follow only after the semantic-transform slice has equivalent behavioral,
layout, compilation, and code-generation evidence.
