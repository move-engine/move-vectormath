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

## Process

- Comparative research precedes core implementation.
- Per-API reports and their sources are committed.
- Semantic and geometry proposals receive a design checkpoint.
- Compilation cost, layouts, code generation, and runtime performance are
  evaluated alongside usability.
