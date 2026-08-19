# API-v2 implementation

This directory records implementation work and the evidence used to accept or
reject architectural choices.

- [`PhaseAArchitectureProof.md`](PhaseAArchitectureProof.md) describes the
  first backend, representation, storage, GPU-layout, semantic-type, codegen,
  compile-cost, and runtime proof.
- [`PhaseBSemanticTransforms.md`](PhaseBSemanticTransforms.md) records the
  point, normal, rigid-transform, affine-semantic, compile-cost, shader, and
  generated-code evidence for the next vertical slice.
- [`PhaseCInitialGeometry.md`](PhaseCInitialGeometry.md) records the initial
  CPU ray, plane, triangle, sphere, AABB, typed-query, prepared-ray, layout,
  compile-cost, and correctness evidence.

The Phase A architecture and initial Phase B semantic-transform slice are
accepted, but their code remains a proof surface rather than a complete
replacement for v1. CPU geometry and spatial queries are the next vertical
slice. The initial geometry slice is implemented and awaiting its hosted CI
and runtime benchmark evidence. Broader geometry, frustum facilities, final
public header organization, and the deferred Slang companion remain subsequent
work.
