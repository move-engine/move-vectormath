# Vectormath Review

Scope: header inspection of the current workspace plus a few direct `g++` probes against the local RTM dependency cache. I reviewed correctness first, then API/build stability, then performance-sensitive paths.

## Findings

### 1. `vec4` is currently unusable with the pinned RTM version
Severity: high

[`packages/move/math/include/move/math/rtm/base_vec4.hpp:331`](packages/move/math/include/move/math/rtm/base_vec4.hpp#L331) and [`packages/move/math/include/move/math/rtm/base_vec4.hpp:481`](packages/move/math/include/move/math/rtm/base_vec4.hpp#L481) call `rtm::vector_normalize`, but RTM only exposes `vector_normalize3` here. A minimal probe including [`packages/move/math/include/move/vectormath.hpp`](packages/move/math/include/move/vectormath.hpp) fails immediately for this reason, which means the umbrella header does not build in its current state.

Impact:
- Any code that includes [`packages/move/math/include/move/math/vec4.hpp`](packages/move/math/include/move/math/vec4.hpp) transitively fails to compile.
- That also blocks [`packages/move/math/include/move/math/quat.hpp`](packages/move/math/include/move/math/quat.hpp) and the matrix headers because they include `vec4`.

### 2. Matrix transform helpers are mathematically wrong
Severity: high

[`packages/move/math/include/move/math/mat3x3.hpp:153`](packages/move/math/include/move/math/mat3x3.hpp#L153) and [`packages/move/math/include/move/math/mat4x4.hpp:150`](packages/move/math/include/move/math/mat4x4.hpp#L150) implement `transform_vector`/`transform_point` by component-wise multiplying the input with only the matrix `x` axis. That discards every other row/column contribution. On an identity matrix, `transform_point({x, y, z})` would collapse to `{x, 0, 0}` instead of returning the original point.

[`packages/move/math/include/move/math/mat4x4.hpp:168`](packages/move/math/include/move/math/mat4x4.hpp#L168) has a second bug: `transform_vector4` returns `fast_vec3_t::from_rtm(res)` even though the function returns `fast_vec4_t`, so `w` is dropped as well.

These helpers should be using RTM's `matrix_mul_vector3`/`matrix_mul_vector` style APIs instead of `vector_mul` on a single axis.

### 3. `mat3x3::store_array` reads a nonexistent fourth axis
Severity: high

[`packages/move/math/include/move/math/mat3x3.hpp:103`](packages/move/math/include/move/math/mat3x3.hpp#L103) loops `i < 4` and calls `matrix_get_axis(_value, (axis3)i)`. `axis3` only has three valid axes. The fourth iteration is undefined behavior before the code even truncates back to 3x3 output.

### 4. Quaternion inverse is wrong for non-unit quaternions
Severity: high

[`packages/move/math/include/move/math/quat.hpp:216`](packages/move/math/include/move/math/quat.hpp#L216) and [`packages/move/math/include/move/math/quat.hpp:266`](packages/move/math/include/move/math/quat.hpp#L266) compute inverse as `normalize(conjugate(q))`. That is only correct for unit quaternions. For general quaternions the inverse is `conjugate(q) / |q|^2`.

There is already a correct helper in [`packages/move/math/include/move/math/rtm/rtm_ext.hpp:483`](packages/move/math/include/move/math/rtm/rtm_ext.hpp#L483), but it is not used.

### 5. `vec3::refract` returns the zero vector for ordinary refraction cases
Severity: high

[`packages/move/math/include/move/math/scalar/base_vec3.hpp:468`](packages/move/math/include/move/math/scalar/base_vec3.hpp#L468) and [`packages/move/math/include/move/math/rtm/base_vec3.hpp:467`](packages/move/math/include/move/math/rtm/base_vec3.hpp#L467) document `ior` as an index of refraction, but the implementation behaves like it expects an already-divided eta ratio.

A direct probe with a normalized incident vector `(0, -1, -1)`, normal `(0, 1, 0)`, and `ior = 1.5f` returned `0 0 0` in both scalar and RTM paths, even though that is a normal air-to-glass style case and should refract, not total-internally-reflect.

This also conflicts with the existing expectation in [`packages/move/math/tests/vec3.cpp:429`](packages/move/math/tests/vec3.cpp#L429), where the tests assume a non-zero refracted vector for `ior = 1.2`.

### 6. `quat::load_array` does not match the RTM API
Severity: high

[`packages/move/math/include/move/math/quat.hpp:83`](packages/move/math/include/move/math/quat.hpp#L83) calls `quat_load(_value, in)`, but RTM's `quat_load` returns a quaternion from a pointer and does not take an output argument. A compile probe that instantiated `load_array` hit this signature mismatch immediately.

### 7. RTM `vec3` deserialization reads past a 3-element buffer
Severity: high

[`packages/move/math/include/move/math/rtm/base_vec3.hpp:284`](packages/move/math/include/move/math/rtm/base_vec3.hpp#L284) allocates `T data[3]` and then loads it with `rtm::vector_load(data)`, which is a 4-lane load. That is an out-of-bounds read on the stack buffer during loading.

The matching public wrapper serialization path in [`packages/move/math/include/move/math/vec3.hpp:244`](packages/move/math/include/move/math/vec3.hpp#L244) inherits this bug.

### 8. `vec2` has compile-time API holes
Severity: high

[`packages/move/math/include/move/math/scalar/base_vec2.hpp:250`](packages/move/math/include/move/math/scalar/base_vec2.hpp#L250) calls `*this /= length()` but `base_vec2` never defines `operator/=`. Instantiating `normalize()` fails to compile.

[`packages/move/math/include/move/math/vec2.hpp:126`](packages/move/math/include/move/math/vec2.hpp#L126) forwards serialization to `base_t::serialize`, but [`packages/move/math/include/move/math/scalar/base_vec2.hpp`](packages/move/math/include/move/math/scalar/base_vec2.hpp) has no `serialize` member. Instantiating `vec2::serialize()` also fails to compile.

### 9. The public umbrella header is incomplete
Severity: medium

[`packages/move/math/include/move/math/all.hpp:3`](packages/move/math/include/move/math/all.hpp#L3) only includes `common`, `macros`, `vec2`, `vec3`, and `vec4`. It omits [`packages/move/math/include/move/math/quat.hpp`](packages/move/math/include/move/math/quat.hpp), [`packages/move/math/include/move/math/mat3x3.hpp`](packages/move/math/include/move/math/mat3x3.hpp), and [`packages/move/math/include/move/math/mat4x4.hpp`](packages/move/math/include/move/math/mat4x4.hpp).

That makes [`packages/move/math/include/move/vectormath.hpp`](packages/move/math/include/move/vectormath.hpp) an incomplete public entry point.

### 10. The advertised "fast" `vec2` types are still scalar
Severity: medium

[`packages/move/math/include/move/math/vec2.hpp:10`](packages/move/math/include/move/math/vec2.hpp#L10) maps `Default` to `Scalar`, and [`packages/move/math/include/move/math/vec2.hpp:15`](packages/move/math/include/move/math/vec2.hpp#L15) resolves both branches of `base_vec2_t` to `scalar::base_vec2<T>`.

That means [`packages/move/math/include/move/math/vec2.hpp:133`](packages/move/math/include/move/math/vec2.hpp#L133) through [`packages/move/math/include/move/math/vec2.hpp:134`](packages/move/math/include/move/math/vec2.hpp#L134) (`fast_float2`, `fast_double2`) are not accelerated at all. A probe printing `fast_float2::acceleration` and `storage_float2::acceleration` produced `1 1` for both.

### 11. `vec3` RTM conversions leave an avoidable hot-path performance penalty
Severity: medium

[`packages/move/math/include/move/math/vec3.hpp:177`](packages/move/math/include/move/math/vec3.hpp#L177) and [`packages/move/math/include/move/math/vec3.hpp:184`](packages/move/math/include/move/math/vec3.hpp#L184) still have `TODO: Fast mode for RTM vec3` and always bounce through scalar lane extraction/reconstruction even when the source and destination are already RTM vectors.

This affects quaternion rotation, matrix multiplication helpers, and any code that frequently crosses the wrapper/RTM boundary.

### 12. `byte*` aliases are signed, not unsigned
Severity: low

[`packages/move/math/include/move/math/vec2.hpp:153`](packages/move/math/include/move/math/vec2.hpp#L153), [`packages/move/math/include/move/math/vec3.hpp:270`](packages/move/math/include/move/math/vec3.hpp#L270), and [`packages/move/math/include/move/math/vec4.hpp:467`](packages/move/math/include/move/math/vec4.hpp#L467) define both `sbyte*` and `byte*` aliases as `int8_t`. That makes the `byte` aliases signed, which is surprising and likely wrong for callers expecting `uint8_t`.

## Test and coverage gaps

- [`packages/move/math/tests/mat4x4.cpp`](packages/move/math/tests/mat4x4.cpp) validates `operator*`-based transforms, but it does not exercise `transform_point`, `transform_vector`, or `transform_vector4`. Those helpers currently contain one of the largest correctness bugs in the library.
- [`packages/move/math/tests/vec4.cpp:468`](packages/move/math/tests/vec4.cpp#L468) still has explicit `TODO` markers for `reflect` and `refract`.
- I did not find any serialization-focused coverage for vectors, quaternions, or matrices, which is why the `vec2`/`vec3` serialization defects were able to persist.
- There also appears to be no compile-only smoke test for the umbrella header, which would have caught the current `vec4` build break.

## Validation notes

I used small standalone probes to confirm the highest-risk items:

- Including [`packages/move/math/include/move/vectormath.hpp`](packages/move/math/include/move/vectormath.hpp) against the local RTM checkout fails because `rtm::vector_normalize` is not available.
- Calling `move::math::float3::refract` and `move::math::storage_float3::refract` with a normalized `(0, -1, -1)` incident vector, normal `(0, 1, 0)`, and `ior = 1.5f` returned `0 0 0`.
- Instantiating `move::math::storage_float2::normalize()` and `move::math::float2::serialize()` both fail to compile for the reasons described above.

## Priority order

1. Fix the build blockers and API holes: `vec4`, `quat::load_array`, `vec2::normalize`, `vec2::serialize`.
2. Fix the incorrect math results: matrix transform helpers, quaternion inverse, `vec3::refract`, `mat3x3::store_array`.
3. Add targeted tests for the currently uncovered helpers and serialization paths.
4. Then address the performance debt: real RTM-backed `vec2`, and fast-path `vec3` RTM conversions.
