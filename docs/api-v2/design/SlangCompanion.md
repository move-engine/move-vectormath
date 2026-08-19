# Slang companion library

## Decision

Move Vectormath may provide an opt-in Slang companion library, but Slang is
not the implementation language or source of truth for the CPU library. The
C++ `mv::math` API, its scalar/RTM backends, and its CPU performance evidence
remain primary.

The companion is deferred until the CPU API required by the first Move 1.x
releases is usable. CPU geometry, queries, matrices, and culling take priority
over shader-side API parity.

## Intended role

The future companion should mirror the portable mathematical contracts that
are useful in shaders:

- semantic points, directions, normals, rotations, and transforms;
- rays, planes, triangles, spheres, AABBs, OBBs, and frusta;
- intersection, classification, and culling conventions;
- prepared ray/frustum representations where they are useful on GPUs;
- exact buffer-transfer structures corresponding to `mv::math::gpu` layouts.

It should use Slang modules and built-in shader vector/matrix types. It must
not expose RTM, reproduce CPU SIMD policy, or imply that a shader compute type
has the same ABI as a C++ compute type.

## Conformance model

Observable behavior should be shared through test data and documented
contracts rather than through code generation into the production C++ API.
Candidate conformance layers are:

1. common golden inputs and expected results;
2. C++ execution as the authoritative CPU implementation;
3. Slang CPU compilation as a reference/parity runner where supported;
4. selected GPU execution tests for layout- or floating-point-sensitive paths;
5. Slang reflection of field offsets, sizes, alignments, strides, and matrix
   order against the C++ transfer-layout assertions.

Slang's CPU target is useful for conformance, but it is not a replacement for
the optimized C++ implementation and is not a stable public dependency of the
core math package.

## Proposed module organization

```text
shaders/mv/math/
  Core.slang
  Semantics.slang
  Transforms.slang
  GpuLayouts.slang
  Geometry.slang
  Intersections.slang
  Culling.slang
  Spatial.slang
```

The module boundaries should track the focused C++ headers without requiring
one language to mimic incidental implementation details of the other.

## Scheduling

Do not begin the companion implementation before the initial Move 1.x CPU
surface is accepted. When it begins, add parity alongside stable CPU
capabilities rather than delaying CPU geometry or culling to achieve complete
cross-language coverage in one pass.

