# Math API v2

This directory contains the research, design, baselines, and implementation
planning for a breaking revision of Move Vectormath.

The target surface uses the `mv` namespace, PascalCase identifiers, explicit
storage and compute boundaries, and semantically meaningful game/graphics
types. The revision does not include a compatibility layer for the existing
`move::math` API.

## Contents

- [`reviews/`](reviews/README.md) contains the current-API review, external API
  comparisons, sources, and comparative synthesis.
- [`design/`](design/README.md) contains proposed types, invariants,
  architecture, and public API decisions.
- [`baselines/`](baselines/README.md) records the v1 type, compilation, and
  runtime measurements against which v2 will be evaluated.
- [`planning/`](planning/README.md) tracks scope, decisions, sequencing, and
  current status.

Research and design are deliberately separated from implementation. Core v2
code will begin only after the comparative prepass and design checkpoint have
been reviewed.
