# Legacy correctness-contract audit

This audit maps the behavior protected by the legacy `move::math` tests to the
replacement `mv::math` surface. It is a capability audit, not a request for
source-compatible spellings. Move 1.x has no legacy-compatibility requirement.

## Replacement coverage

| Legacy area | Replacement evidence | Decision |
| --- | --- | --- |
| `vec2`, `vec3`, and `vec4` construction, arithmetic, compound assignment, indexing, axes, dot/cross, length/distance, Min/Max, clamp, interpolation, reflection, refraction, and approximate comparison | `tests/representation.cpp`, `tests/semantic_transforms.cpp` | Replaced. Invariant-bearing normalization and refraction use `Direction3`/`Normal3` and explicit failure/status results. |
| Scalar and RTM vector implementations | The same Catch2 behavior sources run as RTM and `MV_MATH_FORCE_SCALAR` targets | Replaced without exposing backend identity in public type names. |
| Scalar constants, saturation, inverse lerp, and smoothing | `tests/scalar.cpp` | Replaced. Ordinary standard-library operations are not wrapped. |
| Quaternion identity, algebra, normalization, inverse, axis-angle, Euler construction, look direction, interpolation, and rotation equivalence | `tests/semantic_transforms.cpp` | Replaced by `Quat<T>` for general algebra and invariant-bearing `Rotation3<T>` for rotations. |
| `mat3x3` and `mat4x4` construction, access, multiplication, transpose, determinant, inverse, rotations, scales, view, and projection | `tests/matrices.cpp` | Replaced. Singular inverse and invalid view/projection inputs are explicitly fallible. |
| Packed CPU storage and GPU transfer layouts | `tests/representation.cpp`, shader-layout tests | Replaced by explicit packed/load/store and GPU-layout facilities. |
| Umbrella and dependency-free consumer smoke coverage | `tests/consumer/main.cpp`, `tests/xmake/header_consumer.cpp`, `tests/standalone.cpp` | Replaced; all three consume only `mv::math`. |

## Intentional removals

| Legacy behavior | Reason |
| --- | --- |
| Public acceleration selectors, `fast_*` aliases, and direct RTM conversion | Backend selection is an implementation detail. Explicit packed and GPU transfer types cover representation boundaries. |
| Implicit mutable-pointer conversion and serialization hooks on compute vectors | A compute value is not a portable storage or wire format. Callers must choose a packed, GPU, or application-owned representation explicitly. |
| Zero-vector normalization returning zero | A normalized zero vector is not a valid direction. `Direction3::TryFrom` and `Normal3::TryFrom` report failure. |
| Identity fallback for invalid look-rotation input | Silent identity changes the meaning of a transform. `Rotation3::TryLookTowards` reports failure. |
| Infallible matrix inverse and implicit projection/view conventions | Failure, handedness, clip depth, and depth direction are explicit in `mv::math`. |
| Quaternion logarithm and exponential | No Move 1.x workload currently requires them. They remain workload-driven future general-quaternion operations rather than blocking the cutover. |
| Broad arbitrary swizzles | The retained named swizzles cover demonstrated use without multiplying template instantiations and compile cost. More are workload-driven. |

## Deletion result

The direct old/new benchmark evidence is archived, and the live benchmark
repository has no legacy dependency. The legacy-only tests, installed headers,
and compatibility aliases were removed together. Strict GCC, Clang ASan/UBSan,
RTM, forced-scalar, CMake consumer, XMake consumer, and example checks passed
locally; the hosted compiler/OS matrix is the final publication check.

The C++20 module wrapper remains intentionally sequenced after these deletion
steps so headers are the sole implementation source of truth.
