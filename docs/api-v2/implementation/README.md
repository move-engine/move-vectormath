# API-v2 implementation

This directory records implementation work and the evidence used to accept or
reject architectural choices.

- [`PhaseAArchitectureProof.md`](PhaseAArchitectureProof.md) describes the
  first backend, representation, storage, GPU-layout, semantic-type, codegen,
  compile-cost, and runtime proof.
- [`PhaseBSemanticTransforms.md`](PhaseBSemanticTransforms.md) records the
  point, normal, rigid-transform, affine-semantic, compile-cost, shader, and
  generated-code evidence for the next vertical slice.

The Phase A architecture has been accepted as the basis for Phase B, but its
code remains a proof surface rather than a complete replacement for v1.
Production semantic transforms are the next vertical slice. Geometry,
intersections, frustum facilities, and the final public header organization
remain deferred until that slice has equivalent evidence.
