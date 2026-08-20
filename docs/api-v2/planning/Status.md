# Status

## Current phase

Phase A and Phase B are accepted. Phase C now includes generic float/double
semantic transforms and geometry/query kernels, discrete integer bounds,
practical vector parity, complete transform conversion and TRS facilities,
`Mat4<T>`, OBBs, frustum culling, and explicit projection/view/viewport
operations. The generic-math, graphics-matrix, transform, culling, graphics,
and packaging checkpoints have passed hosted CI.

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
- Implemented `Line3f` and `Segment3f` with explicit physical-distance versus
  fraction parameter conventions
- Implemented detailed and concise point/line, point/ray, point/segment,
  point/plane, and point/triangle closest-point and distance queries
- Added deterministic lower-dimensional triangle handling and an independent
  1,024-case double-precision triangle closest-point oracle
- Added point/AABB and solid-sphere closest/distance conveniences with explicit
  empty-box and contained-point semantics
- Added detailed segment/segment closest-point queries, degenerate handling,
  and a 2,048-case independent double-precision critical-point oracle
- Added compact 32-byte `Capsule3f`, point/capsule proximity and containment,
  and sphere/capsule and capsule/capsule overlap predicates
- Completed practical vector parity for arithmetic scalar families, including
  approximate comparison, explicit indexing/mutation policy, constrained
  swizzles, and status-bearing direction/normal refraction
- Generalized semantic transforms, continuous geometry primitives, prepared
  rays, intersection results, and query kernels across `float` and `double`
- Added fixed-width signed and unsigned integer AABB aliases while constraining
  continuous metrics and construction to floating-point bounds
- Implemented and correctness-tested `Mat4<T>` construction, homogeneous
  transform, composition, transpose, determinant, and fallible inverse
- Added explicit handedness, clip-depth, and forward/reverse-depth projection
  construction, including finite/infinite perspective and off-center
  orthographic matrices
- Added fallible left- and right-handed look-at view construction with explicit
  coincident-eye/target and parallel-up failure
- Extended the separate migration benchmark suite with Mat4, perspective,
  orthographic, and view parity and performance rows
- Added quaternion algebra, explicit-order Euler construction, look-towards,
  and shortest-arc rotation interpolation
- Added affine composition/inversion, rigid/affine/TRS matrix conversions, and
  typed reflection-aware TRS decomposition with explicit shear failure
- Resolved reflected-normal policy with distinct half-space covector and
  ordered-surface-winding operations
- Added float/double OBBs and ray, sphere, AABB, and full 15-axis OBB queries
- Added explicit clip-depth/reverse-Z frustum extraction, infinite-far plane
  masks, prepared point/sphere/AABB/OBB culling, and hierarchical plane masks
- Added NDC and viewport project/unproject plus near-plane and perspective
  viewport-ray construction
- Added first-class XMake consumption with RTM and forced-scalar smoke tests,
  the canonical `mv::math` CMake target, and current public usage documentation
- Verified the transform, culling, graphics, CMake, and XMake packaging
  checkpoint across the hosted compiler and operating-system matrix
- Migrated the current matrix, geometry/query, culling/graphics,
  representation, and semantic-transform behavior suites to Catch2 while
  retaining separate RTM and forced-scalar execution
- Added C++20-backed PascalCase constants, saturation, fallible overflow-safe
  inverse lerp, and explicitly clamped smoothing helpers while documenting
  standard-library guidance for ordinary scalar math

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
- Audit the remaining legacy-only correctness contracts and migrate all
  benchmark capabilities before beginning the `mv.math` module wrapper
