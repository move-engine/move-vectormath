# API-v2 implementation

This directory records implementation work and the evidence used to accept or
reject architectural choices.

- [`PhaseAArchitectureProof.md`](PhaseAArchitectureProof.md) describes the
  first backend, representation, storage, GPU-layout, semantic-type, codegen,
  compile-cost, and runtime proof.

The Phase A code is intentionally a proof surface rather than a complete
replacement for v1. Geometry, matrices, production transforms, and the final
public header organization remain deferred until the representation findings
have been reviewed.
