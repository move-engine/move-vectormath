# V1 runtime-performance baseline

## Measurement caveats

The retained local nanobench runs were made while the host reported frequency
scaling and a powersave governor. Nanosecond estimates are therefore useful for
finding large effects but are not a stable publication-quality baseline.
Hardware counter comparisons and generated-code inspection are more reliable
for the current run.

Raw local results are intentionally excluded from version control. The
benchmark suite commit and dependency pins are recorded in
[`README.md`](README.md).

## Current findings

- Move and raw RTM retired exactly the same instruction counts in representative
  ray/triangle (17,493) and ray/AABB (8,972) batch kernels. The wrapper can
  disappear completely.
- Move normalization was roughly 42–59% slower than Sony Vectormath across the
  tested ISA builds. Move was close to raw RTM in cycles but used four extra
  instructions for its safe zero-vector contract.
- Camera-basis construction was roughly 16–25% slower than Sony Vectormath and
  about 9–11% slower than raw RTM.
- Quaternion vector rotation was roughly 39–42% slower than Sony Vectormath and
  7–14% slower than raw RTM in cycles, with two additional instructions.
- The apparent Move win over raw RTM in one matrix/vector case used the same
  primitive and is likely harness/compiler code-shape noise rather than a
  library capability advantage.
- The model-matrix comparison was not equivalent: Move constructed a real
  Euler quaternion while the RTM case used `quat_set`.

## V2 benchmark requirements

- Every library case must perform semantically equivalent work.
- Inputs must vary enough to prevent constant folding and result reuse.
- Results must be consumed outside the timed kernel.
- Report distributions, counters, generated code, compiler, flags, ISA,
  governor, and CPU model.
- Separate safe and preconditioned operations where contracts differ.
- Add representative game-loop workloads: transform hierarchies, camera basis,
  animation interpolation, ray casts, broad-phase bounds, frustum culling,
  particle integration, and packed/compute conversion.
- Add compilation benchmarks alongside runtime benchmarks.

