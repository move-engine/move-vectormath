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
- [`PhaseCClosestPoint.md`](PhaseCClosestPoint.md) records the line, segment,
  closest-point, distance, degeneracy, provenance, and verification slice.
- [`PhaseCCapsule.md`](PhaseCCapsule.md) records the compact sphere-swept
  segment representation and its first proximity and overlap queries.
- [`PhaseCGenericMathAndGraphicsMatrices.md`](PhaseCGenericMathAndGraphicsMatrices.md)
  records vector parity, float/double geometry and queries, integer bounds,
  Mat4, projection/view policy, benchmark evidence, and the inlining audit.
- [`AlgorithmProvenance.md`](AlgorithmProvenance.md) records the exact papers,
  compatible-licensed implementations, deliberate deviations, and historical
  source status of Codex-generated algorithms.

The Phase A architecture and initial Phase B semantic-transform slice are
accepted, but their code remains a proof surface rather than a complete
replacement for v1. CPU geometry and spatial queries are the next vertical
slice. The initial geometry slice has passed local and hosted compiler, OS,
parity, and diagnostic benchmark verification. Broader geometry, frustum
facilities, final public header organization, and the deferred Slang companion
remain subsequent work.
