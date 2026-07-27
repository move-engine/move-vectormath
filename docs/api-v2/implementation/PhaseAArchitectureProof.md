# Phase A architecture proof

## Scope

Phase A tests the highest-risk part of the API-v2 design before expanding the
geometry surface:

- a stable `mv::math` public identity over a selected scalar or RTM backend;
- native-backed versus fixed-layout three-vector compute storage;
- compact CPU storage versus SIMD-resident compute storage;
- explicit 12-byte and 16-byte GPU transfer contracts;
- contiguous and strided fused load-transform-store paths;
- zero-overhead invariant direction and rotation types;
- focused-header compile cost, generated code, and working-set behavior.

This is not yet the complete v2 API and does not establish a stable ABI.

## Implemented proof surface

The implementation lives under `packages/move/math/include/mv/math/`.

| Area | Proof types/facilities |
| --- | --- |
| Backend | `SelectedVectorOps`, scalar primitives, RTM float primitives |
| Core | `Vec2`, `Vec3`, `Vec4`, `Quat`, strong degrees/radians |
| Semantics | invariant `Direction3f`, invariant `Rotation3f` |
| Storage | `PackedVec2f`, `PackedVec3f`, `PackedVec4f` |
| GPU transfer | packed float3, 16-byte float3 slot, row/column matrix layouts |
| Composite GPU layout | position/radius and two-slot AABB examples |
| Transform proof | `AffineTransform3f`, vector and point transforms |
| Bulk processing | contiguous and `StridedSpan` transform kernels |
| Compatibility | trait-constrained `AsGpuBytes` |

`experimental/RepresentationProof.hpp` contains benchmark-only alternatives.
It is deliberately excluded from the `PhaseA.hpp` convenience header.

## Layout results

GCC and Clang, in scalar and RTM configurations, verify:

| Type | Size | Alignment | Other contract |
| --- | ---: | ---: | --- |
| `Vec2f` | 8 | 8 | trivial, standard-layout |
| `Vec3f` | 16 | 16 | trivial, standard-layout, XYZ in first 12 bytes |
| `Vec4f` | 16 | 16 | trivial, standard-layout |
| `Direction3f` | 16 | 16 | same representation cost as `Vec3f` |
| `Rotation3f` | 16 | 16 | same representation cost as `Quatf` |
| `PackedVec3f` | 12 | 4 | XYZ offsets 0/4/8 |
| `gpu::Float3Slot16` | 16 | 16 | XYZ offsets 0/4/8, canonical zero padding |
| `gpu::PositionRadius16` | 16 | 16 | radius offset 12 |
| `gpu::Aabb3fSlots16` | 32 | 16 | minimum/maximum offsets 0/16 |
| `gpu::Mat3f16` | 48 | 16 | three 16-byte vector slots |

Shader-side HLSL, GLSL `std430`, and WGSL fixtures mirror the composite
layouts. The current machine has no DXC, glslang, Tint, or Naga executable, so
host assertions were run but shader compilation/reflection remains a CI
follow-up.

## Representation and generated-code result

The public `Vec3f` stores the selected backend's native value privately. The
backend type is not a public template parameter, return type, base class, or
conversion.

This choice is evidence-driven:

- GCC generates identical inner loops for public `Vec3f` and the raw-native
  proof: 13 instructions per element for packed load, add, scale, and packed
  store.
- Clang also generates identical public and raw-native loops.
- GCC's fixed `float[4]` alternative requires 26 instructions per element and
  substantial stack/bit-assembly traffic in the same kernel.
- Clang optimizes the fixed form much more successfully, showing that it is
  compiler-sensitive rather than a portable zero-cost representation.
- Packed-to-GPU and strided transforms remain fused; they do not allocate or
  materialize an intermediate compute array.

The initial conclusion is to retain private selected-native storage for
`Vec3f`, guarded by layout and codegen tests on every supported target. A
fixed scalar array is rejected for the current GCC implementation. This is a
provisional implementation decision, not an ABI promise.

## Compile-cost result

Focused compile fixtures live in `tests/compile/`. The following warmed local
GCC 16 measurements used C++20 and `-O2`; absolute times are machine-specific.

| Fixture | Time | Peak memory | Preprocessed lines |
| --- | ---: | ---: | ---: |
| raw RTM vector | 0.70 s | 135.1 MiB | 85,951 |
| Phase A `Vec3` | 0.72 s | 137.0 MiB | 86,396 |
| Phase A rotation | 0.71 s | 142.1 MiB | 88,341 |
| Phase A umbrella | 0.74 s | 143.8 MiB | 90,147 |
| scalar Phase A `Vec3` | 0.22 s | 66.2 MiB | 26,636 |
| scalar Phase A umbrella | 0.37 s | 88.7 MiB | 46,466 |

The focused RTM facade adds about 445 preprocessed lines and 1.9 MiB peak
memory over raw RTM in this fixture, a much thinner increment than v1. The
scalar configuration does not parse RTM at all. Clang 22 showed the same shape:
0.74 seconds for raw RTM, 0.76 for `Vec3`, 0.78 for rotation, and 0.83 for the
umbrella.

These fixtures must remain focused; `PhaseA.hpp` is a convenience header, not
the expected include for a vector-only consumer.

## Preliminary runtime result

The separate vectormathbench Phase A executable measures SSE4.2, AVX, and AVX2
builds at 256, 4,096, 65,536, and 1,048,576 elements. Representative AVX
results on an AMD Ryzen 9 5900HS are:

| Kernel | 4,096 elements | 1,048,576 elements |
| --- | ---: | ---: |
| public native-backed `Vec3` | 0.76 ns/element | 1.67 ns/element |
| raw-native proof | 0.76 ns/element | 1.67 ns/element |
| fixed scalar-array `Vec3` | 7.07 ns/element | 7.11 ns/element |
| compact 8-byte `Vec2` | 0.61 ns/element | 0.63 ns/element |
| aligned 16-byte `Vec2` | 0.30 ns/element | 1.10 ns/element |
| compute-16 particle update | 0.33 ns/element | 1.68 ns/element |
| packed-12 particle update | 0.66 ns/element | 1.30 ns/element |
| packed transform to GPU-16 | 1.02 ns/element | 1.71 ns/element |
| strided packed transform | 1.06 ns/element | 4.23 ns/element |

The results show the intended crossover:

- persistent 16-byte values win compute-resident, cache-sized loops;
- compact values can win once memory footprint dominates;
- a 16-byte `Vec2` halves small-set instruction count but loses for a large
  array because it doubles traffic;
- public `Vec3f` has no measured facade penalty relative to the raw backend;
- AVX2 reduces the transform kernels by two instructions per element, while
  the core `Vec3` result is effectively ISA-independent.

The host used a powersave governor with active frequency scaling. Instruction
counts and broad crossovers are useful, but the exact large-working-set timing
is preliminary and some one-million-element samples were unstable. A pinned,
performance-governor run is required before finalizing `Vec2f` or publishing
portable performance claims.

## Validation completed

- GCC 16 Release, RTM and forced-scalar, strict warnings as errors
- Clang 22 Release, RTM and forced-scalar, strict warnings as errors
- behavioral/layout tests for vector, storage, transfer, semantic, transform,
  contiguous, and strided paths
- codegen object targets for public, experimental, scalar, and RTM paths
- SSE4.2, AVX, and AVX2 Phase A benchmark builds and local runs
- GitHub workflow coverage for Phase A tests in the existing GCC, Clang,
  AppleClang/ARM, MSVC, sanitizer, and coverage jobs

## Checkpoint decision

Phase A is accepted as the architecture for the next implementation slice.
Private selected-native `Vec3f` storage, focused headers, explicit compact and
GPU representations, invariant semantic wrappers, and fused strided
processing proceed into Phase B. This accepts an implementation direction, not
a stable ABI.

## Evidence still required

- Do not finalize 8-byte versus 16-byte `Vec2f` before a controlled benchmark
  rerun and representative 2D workloads.
- Do not declare a stable `Vec3f` ABI until MSVC and AppleClang/ARM generated
  code has been inspected; their Phase A builds and behavioral tests now pass
  CI.
- Add actual shader compilation and reflection validation where toolchains are
  available.
- Expand backend primitives only when a shared algorithm needs them.
- Complete the point/normal and transform evidence slice before beginning the
  full geometry, query, or frustum implementation.
