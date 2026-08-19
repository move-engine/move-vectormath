# Status

## Current phase

Phase A and the initial Phase B semantic-transform slice are accepted. The
first Phase C CPU geometry/query slice for Move 1.x is implemented and verified
locally and on the hosted compiler/OS matrix.

## Completed

- Published integrated Move Vectormath baseline on `main`
- Published integrated vectormathbench baseline on `master`
- Pinned vectormathbench to the permanent Move baseline
- Verified clean-cache SSE4.2, AVX, and AVX2 benchmark builds
- Created and published `feat/math-api-v2`
- Established the organized research, design, baseline, and planning structure
- Completed the current Move API review
- Recorded v1 layout, compilation, and runtime baselines
- Completed the Unity, Unreal, Source, Garry's Mod, Godot, and DirectX reviews
- Completed the comparative feature matrix, analysis, and recommendations
- Completed the Unity familiarity-without-imitation review pass and propagated
  its semantic-transparency rules into the v2 design
- Completed the practical CPU storage, SIMD residency, GPU layout, and
  interleaved-data review pass
- Completed the semantic, transform, geometry, query, frustum, invariant,
  backend, header, and proposed-call-site designs
- Received Phase A implementation authorization
- Implemented the initial scalar/RTM backend primitive layer
- Implemented the core vector, packed storage, GPU transfer, semantic
  direction/rotation, affine-transform, and fused strided proof surface
- Verified GCC and Clang strict RTM and scalar builds
- Added layout, behavior, codegen, compile-cost, and shader-layout fixtures
- Added and ran SSE4.2, AVX, and AVX2 working-set benchmarks in the separate
  benchmark repository
- Added Phase A coverage to the library's compiler, sanitizer, and coverage CI
  jobs
- Verified the Phase A surface on GCC, Clang, MSVC, and AppleClang/ARM, plus
  sanitizer, coverage, source-hygiene, Linux benchmark, and Windows benchmark
  jobs
- Accepted private selected-native storage as the `Vec3f` implementation
  direction while keeping its ABI provisional
- Authorized the Phase B semantic-transform vertical slice
- Implemented the initial `Point3f`, `Normal3f`, and `RigidTransform3f`
  semantic-transform slice
- Added named rigid composition/inverse and explicit affine point, direction,
  and inverse-transpose normal operations
- Verified identical local GCC generated loops for semantic and raw rigid
  point transformation
- Added HLSL and GLSL SPIR-V compilation, validation, and reflected layout
  checks
- Added representative sprite, UI, point-transform, and direction-invariant
  benchmark workloads
- Verified the full Phase B compiler/ISA CI matrix and the benchmark build,
  parity, and diagnostic performance workflow on Linux and Windows
- Accepted the initial Phase B semantic-transform evidence as sufficient to
  begin CPU geometry
- Documented an opt-in future Slang companion and explicitly deferred its
  implementation behind the CPU surface needed by Move 1.x
- Established a source-first provenance gate for Codex-generated algorithms
  and retrospectively audited the legacy Codex changes and Phase A/B/C
  implementation against original papers and exact compatible-licensed
  library revisions
- Implemented `Ray3f`, `Plane3f`, `Triangle3f`, `Sphere3f`, `Aabb3f`, and
  `PreparedRay3f`
- Implemented distinct predicate and detailed ray/plane, ray/triangle,
  ray/sphere, and ray/AABB query paths with typed results
- Added sphere/sphere, sphere/AABB, and AABB/AABB predicates
- Added Phase C scalar/RTM behavior, layout, focused-compile, generated-code,
  sanitizer, downstream-consumer, and independent ray/AABB reference coverage
- Verified the Phase C library matrix and the separate SSE4.2, AVX, and AVX2
  geometry benchmark build, parity, and diagnostic performance workflow on
  Linux and Windows

## Accepted evidence

- The public `Vec3f` facade and raw RTM proof generate equivalent
  representative loops under GCC and Clang.
- The fixed scalar-array `Vec3f` proof is rejected because its GCC code
  generation is materially worse.
- Compact storage, compute-resident values, and explicit GPU transfer layouts
  remain separate first-class representations.
- Focused headers keep the facade's incremental compile cost small.
- Hosted-runner timings are useful diagnostic artifacts but are not regression
  gates.

## Pending evidence

- Controlled performance-governor `Vec2f` benchmark runs using representative
  2D game and graphics workloads
- WGSL compiler/reflection validation for the retained transfer fixture
- Final `Vec2f` representation choice
- Generated-code inspection on MSVC and AppleClang/ARM before making a stable
  `Vec3f` ABI commitment
- Controlled Phase B benchmark evidence on stable hardware; hosted reports
  remain diagnostic rather than regression gates
- Resolve inverse-transpose direction versus oriented-surface/adjugate
  semantics for `Normal3f` under negative-determinant transforms
