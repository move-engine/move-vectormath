# Accepted decisions

## Public identity

- The new namespace is `mv`, with math facilities under `mv::math`.
- Public identifiers use PascalCase.
- The v2 surface is intentionally breaking.
- No `move::math` or snake_case compatibility layer will be added.

## Architecture

- Optimized SIMD implementations remain a first-class requirement.
- High-level algorithms should be expressed once over a small backend
  primitive vocabulary.
- Backend selection must not require virtual dispatch.
- Packed storage and optimized compute representations must have visible costs.
- Public templates must not expose incidental RTM wrapper implementation types.
- Semantic role, byte representation, and backend selection are independent
  design axes.
- GPU transfer types name exact layout contracts; no type is declared
  universally GPU-blittable.
- Packed/interleaved data must support fused and strided processing without an
  intermediate compute array.
- A future Slang companion may mirror portable math and geometry contracts,
  but C++ remains the primary CPU implementation and source of truth.
- Slang implementation work is deferred until the initial Move 1.x CPU math
  surface is usable; it must not delay CPU geometry, queries, or culling.
- The CPU library remains C++20-compatible. Runtime geometric invariants are
  encoded by fallible construction and invariant-bearing value types rather
  than depending on C++26 contracts.
- Hot queries do not revalidate invariants already established by public
  construction. Separate prepared types remain appropriate when they carry
  cached computation, and predicate kernels may differ from detailed kernels
  when generated-code evidence justifies the distinction.
- Header-defined class members are implicitly `inline`; public function
  templates need no additional ODR annotation. Force-inline attributes are
  reserved for hot leaf operations whose generated-code tests demonstrate a
  missed inline and a measurable regression. Legacy's widely used
  `MVM_INLINE` macro was ordinary `inline`; its unused `MVM_FORCE_INLINE`
  definition did not impose a stronger optimizer contract.

## Approachability

- Familiar engine vocabulary is a discoverability input, not a normative
  design authority.
- Unity users receive a task-oriented on-ramp, not a compatibility facade.
- Familiar names are retained when their contracts are semantically honest.
- Clamping, tolerance, fallback, normalization, transform capability,
  projective failure, and local/world mutation policy must be explicit.
- Scene hierarchy state and mutation do not belong to pure transform values.

## Process

- Comparative research precedes core implementation.
- Per-API reports and their sources are committed.
- Semantic and geometry proposals receive a design checkpoint.
- Compilation cost, layouts, code generation, and runtime performance are
  evaluated alongside usability.

## Accepted Phase A architecture

These decisions authorize Phase B but are not stable ABI commitments:

- `Vec3f` privately stores the selected backend-native value. GCC and Clang
  generate the same representative inner loop as the raw-native proof.
- The fixed scalar-array `Vec3f` alternative is rejected for the current GCC
  implementation because it doubles instruction count in the measured chain.
- Native storage is an implementation technique, not public backend identity:
  RTM types remain absent from public template arguments, return types, base
  classes, and conversions.
- Compact CPU storage, compute values, and exact GPU transfer layouts remain
  distinct first-class representations.
- The scalar backend remains independently compilable without parsing RTM.
- Phase A passing GCC, Clang, MSVC, AppleClang/ARM, sanitizers, coverage, and
  benchmark parity is sufficient to proceed with semantic vectors and
  transforms.

## Decisions still provisional

- `Vec2f` remains compact and uses scalar per-value primitives. Direct
  component-chain benchmarks showed that expanding each 8-byte value into an
  RTM register inhibited loop vectorization; scalar primitives let the compiler
  vectorize across adjacent values instead.
- Semantic wrappers are scalar-parametric where their contracts are coherent:
  `Point3<T>` supports arithmetic scalars, while `Direction3<T>`, `Normal3<T>`,
  and `Rotation3<T>` require floating-point scalars. Aliases do not imply
  invalid integer normalization or rotation APIs.
- Compact 12-byte and compute 16-byte arrays are both first-class choices;
  working-set behavior determines which is faster.
- The facade should retain focused headers. The focused `Vec3` proof adds only
  a small parse/memory increment over raw RTM, while the forced-scalar path
  avoids parsing RTM.
- `Vec3f` size and alignment remain guarded implementation contracts rather
  than a stable cross-version ABI until MSVC and AppleClang generated code is
  inspected.

## Accepted Phase B checkpoint

- `Point3f`, `Direction3f`, and `Normal3f` preserve their semantic contracts
  without storage overhead over `Vec3f`.
- `RigidTransform3f` provides explicit point, vector, direction, and normal
  operations plus named application-order composition and inverse.
- Affine direction and inverse-transpose normal operations expose failure
  rather than silently repairing singular or non-finite transforms.
- The representative semantic rigid-point loop matches the raw-vector loop
  under the measured GCC configuration.
- The supported compiler, scalar/RTM, sanitizer, layout, and downstream
  consumer evidence is sufficient to begin the CPU geometry vertical slice.
