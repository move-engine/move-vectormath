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

- `Vec2f` remains an open decision: 16-byte storage wins small compute loops,
  while 8-byte storage wins the measured large working set.
- Compact 12-byte and compute 16-byte arrays are both first-class choices;
  working-set behavior determines which is faster.
- The facade should retain focused headers. The focused `Vec3` proof adds only
  a small parse/memory increment over raw RTM, while the forced-scalar path
  avoids parsing RTM.
- `Vec3f` size and alignment remain guarded implementation contracts rather
  than a stable cross-version ABI until MSVC and AppleClang generated code is
  inspected.
