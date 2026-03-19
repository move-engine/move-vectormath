# Comprehensive Review: move-vectormath

## Executive Summary

This is a C++ vectormath library providing `vec2`, `vec3`, `vec4`, `mat3x3`, `mat4x4`, and `quat` types with a dual-backend architecture: a scalar fallback and an RTM (Realtime Math) SIMD-accelerated backend. The library is header-only, well-structured, and covers the most important operations for a game engine math library. However, there are several bugs, performance concerns, and design issues worth addressing.

---

## Critical Bugs (Incorrect Behavior)

### 1. Missing `return` in `scalar::base_vec3::operator-=`

**File:** `packages/move/math/include/move/math/scalar/base_vec3.hpp`, lines 144–149

```cpp
MVM_INLINE base_vec3& operator-=(const base_vec3& other)
{
    x -= other.x;
    y -= other.y;
    z -= other.z;
    // BUG: missing "return *this;"
}
```

This is **undefined behavior** — a non-void function falls off the end without returning. On most compilers/platforms this will appear to "work" by accident (the return value register happens to hold `*this`), but it is formally UB and could cause miscompilation with aggressive optimizations. Every other compound assignment operator in the same file correctly returns `*this`. This specific operator is additionally shadowed by the `vec3` wrapper (which does `*this = *this - other`), so the wrapper variant is fine, but any code using `scalar::base_vec3` directly would trigger UB.

**Severity:** Critical / UB

---

### 2. `quat::inverse()` is mathematically wrong for non-unit quaternions

**File:** `packages/move/math/include/move/math/quat.hpp`, lines 274–278

```cpp
MVM_INLINE_NODISCARD quat inverse() const
{
    using namespace rtm;
    return quat::from_rtm(quat_normalize(quat_conjugate(_value)));
}
```

The true inverse of a quaternion is `conjugate(q) / length_squared(q)`, not `normalize(conjugate(q))`. The current implementation returns a unit quaternion that does not satisfy `q * q.inverse() == identity` unless `q` is already unit-length. The correct implementation exists in `rtm_ext.hpp` as `quat_inverse()` but is not used.

The `invert_in_place()` method has the same problem.

**Severity:** High — silently wrong results for non-unit quaternions

---

### 3. `mat4x4::transform_point`, `transform_vector`, and `transform_vector4` are broken

**File:** `packages/move/math/include/move/math/mat4x4.hpp`, lines 177–196

```cpp
MVM_INLINE_NODISCARD fast_vec3_t
transform_point(const fast_vec3_t& rhs) const
{
    rtm_vec4_t mul = rtm::vector_set_w(rhs.to_rtm(), component_type(1));
    rtm_vec4_t res = rtm::vector_mul(
        mul, rtm::matrix_get_axis(_value, rtm::axis4::x));
    return fast_vec3_t::from_rtm(res);
}
```

These methods use `vector_mul` (component-wise multiplication) against only the **first row** of the matrix. A matrix-vector transform should use `matrix_mul_vector` (which performs dot products of the vector against each row). The result is a component-wise multiply with just the x-axis of the matrix, which is completely wrong.

The same bug exists in `mat3x3::transform_vector`.

Note: The free function `operator*(vec4, mat4x4)` correctly uses `matrix_mul_vector`, so users of that API are unaffected.

**Severity:** Critical — completely incorrect results

---

### 4. `base_vec4::refract` does not handle total internal reflection

**File:** `packages/move/math/include/move/math/scalar/base_vec4.hpp`, lines 547–578

The scalar `base_vec4::refract()` computes `sqrt(1 - ior*ior*(1 - dot*dot))` without checking whether the argument to `sqrt` is negative (total internal reflection). When the discriminant is negative, `std::sqrt` returns `NaN`, which silently propagates. The `base_vec3` (both scalar and RTM) variants correctly check for this and return a zero vector, but `base_vec4` (both scalar and RTM) do not.

**Severity:** Medium — produces NaN silently instead of a defined result

---

### 5. `vec4::angle_between_vectors` (RTM) calls `dot` with raw `rtm_vec4_t` instead of `base_vec4`

**File:** `packages/move/math/include/move/math/rtm/base_vec4.hpp`, lines 614–619

```cpp
auto v1norm = rtm::vector_normalize(v1._value);
auto v2norm = rtm::vector_normalize(v2._value);
return math::acos(dot(v1norm, v2norm));
```

Here `v1norm` and `v2norm` are raw `rtm_vec4_t` types, but `dot()` is a static member expecting `const base_vec4&`. This likely compiles because `base_vec4` has an implicit constructor from `rtm_vec4_t`, creating temporary `base_vec4` objects — an unnecessary performance hit. Should use `rtm::vector_dot(v1norm, v2norm)` directly, or wrap them explicitly.

**Severity:** Low (compiles, correct result, but unnecessary temporaries)

---

## Performance Issues

### 6. `vec3` RTM backend stores a `vector4f` but extracts components scalar-by-scalar for `to_rtm()`

**File:** `packages/move/math/include/move/math/vec3.hpp`, lines 167–178

```cpp
MVM_INLINE_NODISCARD rtm_t to_rtm() const
{
    return rtm::vector_set(base_t::get_x(), base_t::get_y(),
                           base_t::get_z(), 1);
}
```

When the base type is already `simd_rtm::base_vec3`, the underlying `_value` is already an `rtm::vector4f/d`. Calling `get_x()`, `get_y()`, `get_z()` extracts each scalar via `rtm::vector_get_x/y/z`, then `vector_set` packs them back into a vector. This round-trip defeats the purpose of SIMD. The RTM base class already has `to_rtm()` method that returns `_value` directly.

The same applies to `from_rtm()` at line 175.

**Severity:** Medium — this is a hot path in all matrix/quaternion multiplication involving vec3

---

### 7. Compound assignment operators in `vec3`/`vec4` wrappers create unnecessary temporaries

**Files:** `vec3.hpp`, `vec4.hpp` — all `operator+=`, `-=`, `*=`, `/=`

```cpp
MVM_INLINE vec3& operator+=(const vec3& other)
{
    *this = *this + other;
    return *this;
}
```

These construct a new temporary and assign back. The base classes (`scalar::base_vec3`, `simd_rtm::base_vec3`) already have efficient in-place compound assignment operators. The wrapper's versions shadow the base class operators and force a construction + copy instead. With `MVM_FORCE_INLINE` this may optimize away on some compilers, but it's not guaranteed, especially for debug builds or when the compiler hits an inlining budget.

**Severity:** Low-Medium — depends on compiler optimization level

---

### 8. `approx_equal` for vectors stores to arrays instead of comparing in-register

**Files:** `vec3.hpp`, `vec4.hpp`, `mat4x4.hpp`, `mat3x3.hpp`, `quat.hpp`

```cpp
template <typename T, move::math::Acceleration Accel>
MVM_INLINE_NODISCARD bool approx_equal(
    const vec3<T, Accel>& a, const vec3<T, Accel>& b,
    const T& epsilon = std::numeric_limits<T>::epsilon())
{
    T aloaded[3]; T bloaded[3];
    a.store_array(aloaded); b.store_array(bloaded);
    return approx_equal(aloaded[0], bloaded[0], epsilon) && ...;
}
```

For RTM-backed types, this stores SIMD registers to memory, then loads scalars back. A SIMD-aware version could subtract, take absolute value, compare against epsilon splat, and check the mask — all in-register.

**Severity:** Low-Medium — matters for hot comparison loops

---

### 9. `look_at_lh`/`look_at_rh` multiply cross product by 1

**File:** `packages/move/math/include/move/math/rtm/rtm_ext.hpp`, lines 31, 67

```cpp
const vec_type right_dir(
    vector_normalize3(vector_mul(vector_cross3(up, look_dir), 1)));
```

`vector_mul(..., 1)` is a no-op multiplication. This was likely a remnant from a handedness-adjustment that was refactored out. It causes an unnecessary SIMD multiply instruction.

**Severity:** Very Low — one extra instruction per look_at call

---

### 10. `base_vec2` has no `from_array`/`load_array` methods

**File:** `packages/move/math/include/move/math/scalar/base_vec2.hpp`

Unlike `base_vec3` and `base_vec4`, `base_vec2` lacks `from_array()` and `load_array()` static/member functions. This is an API inconsistency that could force users to write less efficient load patterns.

**Severity:** Low — API incompleteness

---

## Design & Correctness Concerns

### 11. `#elif` without condition in `macros.hpp`

**File:** `packages/move/math/include/move/math/macros.hpp`, line 30

```cpp
#elif
#define MVM_NODISCARD
```

This should be `#else`. A bare `#elif` is invalid per the C++ standard. GCC and Clang accept it as an extension (treating it as `#else`), but MSVC may reject it, and it triggers warnings.

---

### 12. Comparison operators (`<`, `>`, `<=`, `>=`) are semantically misleading for vectors

**All scalar base_vec types**

The operators use component-wise AND logic:
```cpp
bool operator<(const base_vec3& other) const {
    return x < other.x && y < other.y && z < other.z;
}
```

This means `vec3(1,5,1) < vec3(2,2,2)` is `false` (because `5 < 2` is false), but `vec3(2,2,2) < vec3(1,5,1)` is also `false`. The relation is **not a total order** and **not a partial order** suitable for `std::sort` or ordered containers. This is a common design choice in vectormath libraries (it matches HLSL/GLSL semantics), but it should be documented because it violates the expectations of C++ comparison operators.

---

### 13. `operator[]` clamping silently hides out-of-bounds access

**All base_vec types**

```cpp
T& operator[](const size_t& index) {
    return data[math::min<std::size_t>(index, 2)];
}
```

Out-of-bounds access silently returns the last element instead of asserting or throwing. In debug builds, an assertion would catch bugs earlier. This is a design tradeoff, but worth noting.

---

### 14. `sign(0)` returns `1`, not `0`

**File:** `packages/move/math/include/move/math/common.hpp`, line 181

```cpp
template <typename T>
MVM_INLINE_NODISCARD constexpr T sign(const T& value) {
    return value < T(0) ? T(-1) : T(1);
}
```

Mathematical convention and most other libraries (HLSL, GLSL, Unreal, Unity) define `sign(0) = 0`. This implementation returns `1` for zero. The tests explicitly document this as intentional (`// Note: sign(0) = 1 by definition in common.hpp`), but it will surprise users familiar with other engines.

---

### 15. `lerp` double-clamps

**File:** `packages/move/math/include/move/math/scalar/base_vec3.hpp`, lines 541–548

```cpp
MVM_INLINE_NODISCARD static base_vec3 lerp(const base_vec3& v1,
                                           const base_vec3& v2, T t) noexcept
{
    T clamped = math::saturate(t);   // clamp to [0,1]
    return base_vec3(math::lerp(v1.x, v2.x, clamped),  // lerp calls saturate again
                     math::lerp(v1.y, v2.y, clamped),
                     math::lerp(v1.z, v2.z, clamped));
}
```

`math::lerp` already calls `saturate` internally, so the scalar `t` gets clamped twice. The RTM vec3 backend correctly avoids this by calling `lerp_unclamped` after clamping once. This is a minor performance issue in the scalar path.

---

### 16. `vec2` RTM backend always falls back to `Scalar`

**File:** `packages/move/math/include/move/math/vec2.hpp`, lines 10–17

```cpp
template <move::math::Acceleration Accel>
static constexpr auto vec2_acceleration =
    Accel == Acceleration::Default ? Acceleration::Scalar : Accel;

template <typename T, move::math::Acceleration Accel>
using base_vec2_t =
    std::conditional_t<vec2_acceleration<Accel> == Acceleration::RTM,
                       scalar::base_vec2<T>,  // TODO: Replace with RTM type
                       scalar::base_vec2<T>>;
```

Both branches resolve to `scalar::base_vec2`. The `fast_float2` alias claims RTM acceleration but is actually scalar. This is documented with a TODO but could mislead users checking `acceleration` at runtime.

---

### 17. `byte2`/`sbyte2` aliases are both `int8_t`

**File:** `packages/move/math/include/move/math/vec2.hpp`, lines 155–158

```cpp
using fast_sbyte2 = vec2<int8_t, Acceleration::Default>;
using fast_byte2 = vec2<int8_t, Acceleration::Default>;  // Should be uint8_t?
```

Both `fast_byte2` and `fast_sbyte2` (and their storage variants) use `int8_t`. The conventionally expected mapping is `byte = uint8_t` and `sbyte = int8_t`. The same issue exists in `vec3.hpp` and `vec4.hpp`.

---

### 18. `mat3x4.hpp` is a stub

**File:** `packages/move/math/include/move/math/mat3x4.hpp`

This file only defines the RTM type wrappers but no actual `mat3x4` struct. It's included by `mat4x4.hpp` for the type alias used in `matrix_cast` operations. This is fine structurally but the file is incomplete and not included in `all.hpp`.

---

### 19. Missing `constexpr` on math wrappers that can't be constexpr

**File:** `packages/move/math/include/move/math/common.hpp`

Functions like `sqrt`, `sin`, `cos`, `log`, etc. are marked `constexpr` but delegate to `std::sqrt`, `std::sin`, etc. Prior to C++26, these `<cmath>` functions are **not** `constexpr` in the standard (though some compilers provide them as an extension). This could cause compilation failures on strict compilers or when used in actual `constexpr` contexts.

---

### 20. `all.hpp` doesn't include `mat3x3.hpp`, `mat4x4.hpp`, or `quat.hpp`

**File:** `packages/move/math/include/move/math/all.hpp`

```cpp
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/vec2.hpp>
#include <move/math/vec3.hpp>
#include <move/math/vec4.hpp>
```

Matrix and quaternion headers are not included. Users including `<move/vectormath.hpp>` (which includes `all.hpp`) won't get matrix or quaternion types. This could be intentional to reduce compile times, but is surprising for a "give me everything" header.

---

### 21. Cross product doc comment mentions `v3` parameter that doesn't exist

**Files:** `scalar/base_vec3.hpp` (line 390), `rtm/base_vec3.hpp` (line 421)

```cpp
/**
 * @brief Calculates the cross product between two three-dimensional vectors.
 * @param v1 The first vector
 * @param v2 The second vector
 * @param v3 The third vector     // <-- does not exist
 * @return base_vec3 The cross product
 */
static base_vec3 cross(const base_vec3& v1, const base_vec3& v2) noexcept
```

The `@param v3` documentation is a copy-paste artifact from the vec4 cross product.

---

### 22. `quat::normalize()` has conflicting const/non-const semantics

**File:** `packages/move/math/include/move/math/quat.hpp`

```cpp
// Returns a new normalized quat (line 284)
MVM_INLINE_NODISCARD quat normalize() const { ... }

// Normalizes in place (line 305)
MVM_INLINE quat& normalize() { ... }
```

The same name is used for both a const "return normalized copy" method and a non-const "normalize in place" mutator. While C++ allows this overload, it creates confusing semantics — calling `q.normalize()` on a non-const quat mutates it; calling it on a const quat returns a copy. Other methods in the library use naming like `normalized()` (returning a copy) vs `normalize()` (in-place), which is much clearer. The vec types use this convention correctly.

---

## Test Coverage Gaps

- **Cross product test is empty:** The vec3 cross product test has a `// TODO: Check cross product` comment with no actual assertions.
- **`base_vec3`/`base_vec4` scalar backends are not directly tested** — only tested through the `vec3`/`vec4` wrappers, which shadow the buggy `operator-=`.
- **`transform_point` / `transform_vector` / `transform_vector4` on `mat4x4` are not tested** in the mat4x4 test file — only `operator*` (vec * mat) is tested, which uses the correct `matrix_mul_vector` path.
- **No test for total internal reflection** in `base_vec4::refract`.
- **Refract only has sanity checks**, not exact value verification.
- **No negative/edge-case tests** for `normalized()` on zero-length vectors (division by zero).
- **No tests for `mat3x3::transform_vector`** (which has the same bug as `mat4x4::transform_point`).

---

## Summary of Issues by Severity

| # | Severity | Description |
|---|----------|-------------|
| 1 | **Critical** | `scalar::base_vec3::operator-=` missing `return *this` (UB) |
| 3 | **Critical** | `mat4x4::transform_point/vector` and `mat3x3::transform_vector` use wrong RTM function |
| 2 | **High** | `quat::inverse()` wrong for non-unit quaternions |
| 4 | **Medium** | `base_vec4::refract` doesn't handle total internal reflection |
| 6 | **Medium** | `vec3::to_rtm()` does unnecessary scalar round-trip when RTM-backed |
| 7 | **Low-Med** | Wrapper compound assignment ops create unnecessary temporaries |
| 8 | **Low-Med** | `approx_equal` stores to arrays instead of comparing in-register |
| 5 | **Low** | Implicit temporary construction in RTM `angle_between_vectors` |
| 9 | **Very Low** | `look_at` multiplies cross product by 1 |
| 10 | **Low** | `base_vec2` missing `from_array`/`load_array` |
| 11 | **Low** | `#elif` without condition (should be `#else`) |
| 12 | **Design** | Comparison operators are not a total order |
| 13 | **Design** | `operator[]` silently clamps OOB access |
| 14 | **Design** | `sign(0) = 1` differs from standard convention |
| 15 | **Low** | `lerp` double-clamps `t` in scalar path |
| 16 | **Design** | `vec2` RTM path is actually scalar |
| 17 | **Bug** | `byte2`/`byte3`/`byte4` aliases use `int8_t` not `uint8_t` |
| 18 | **Incomplete** | `mat3x4.hpp` is a stub |
| 19 | **Portability** | `constexpr` on non-constexpr `<cmath>` wrappers |
| 20 | **Design** | `all.hpp` omits matrix/quaternion headers |
| 21 | **Doc** | Cross product doxygen mentions nonexistent `v3` param |
| 22 | **Design** | `quat::normalize()` has confusing const/non-const overloads |

---

## Positive Observations

- Clean separation of scalar and SIMD backends with a consistent template-based dispatch mechanism.
- The RTM integration is generally well done — SIMD operations are used for nearly all hot paths in the vec3/vec4/mat4x4/quat types.
- Good use of `[[nodiscard]]` and forced inlining throughout the public API.
- Serialization support is thoughtfully implemented with load/save distinction for SIMD types.
- The type alias system (`fast_float3`, `storage_float3`, `float3`, `vec3f`) provides useful flexibility.
- Swizzle support on vec3/vec4 is comprehensive.
- The `approx_equal` free functions correctly handle NaN in quaternion comparisons.
- Overall code organization (base types -> wrapper types -> type aliases) is clear and extensible.
