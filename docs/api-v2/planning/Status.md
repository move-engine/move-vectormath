# Status

## Current phase

Phase A architectural proof and evidence review.

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

## Ready for review

- Phase A representation and code-generation findings
- Preliminary storage/working-set benchmark findings
- The provisional native-backed `Vec3f` choice

## Pending evidence

- CI results for MSVC and AppleClang/ARM
- controlled performance-governor benchmark rerun
- shader compiler/reflection validation
- final `Vec2f` representation choice
- authorization to proceed from the architecture proof into the full semantic,
  transform, geometry, query, and frustum implementation
