# Math API v2

This directory contains the research, design, baselines, implementation
evidence, and planning for a breaking revision of Move Vectormath.

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
- [`implementation/`](implementation/README.md) records implementation work
  and the evidence used to evaluate architectural choices.
- [`planning/`](planning/README.md) tracks scope, decisions, sequencing, and
  current status.

Research, design, baselines, and implementation evidence remain separate so
that provisional prototypes are not mistaken for accepted API or ABI.
