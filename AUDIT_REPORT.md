# Vectormath Audit and Remediation Report

**Branch:** `codex-audit`

**Repository:** `move-vectormath`

**Date:** July 25, 2026

**Status:** Implementation and local verification complete.

## Executive Summary

The audit found correctness defects in numeric edge cases, inconsistent behavior
between scalar and RTM-backed vector implementations, wrapper APIs that leaked
backend types, incomplete type traits, quaternion operations with incorrect or
underspecified semantics, avoidable matrix storage work, and a test harness that
did not execute every intended type/backend combination.

The remediation establishes explicit contracts for these cases, updates the
implementation to follow them, expands regression coverage, adds a
dependency-light CMake test target, and adds cross-platform CI configuration.
The complete Catch2 suite passes with 17,064 assertions in 12 test cases. The
same suite also passes under AddressSanitizer and UndefinedBehaviorSanitizer.

## Scope

The work covered:

- Common scalar utilities and approximate comparisons.
- `vec2`, `vec3`, and `vec4`, including scalar and RTM backends.
- Quaternion arithmetic and rotation construction.
- `mat3x3`, `mat4x4`, and their storage types.
- Public type traits and wrapper/backend conversions.
- Build configuration, package metadata, test infrastructure, and CI.
- API and numerical-contract documentation.

No unrelated repository changes were intentionally modified.

## Findings and Remediation

### 1. Public wrapper APIs leaked implementation types

Several operations exposed or returned underlying scalar/RTM implementation
types instead of the public `vec2`, `vec3`, or `vec4` wrapper. This made return
types inconsistent and could break chaining, overload resolution, and generic
code.

The public wrappers now consistently return wrapper types for:

- Arithmetic and compound operations.
- `normalized()` and mutating vector operations.
- Factories such as axes, directions, and zero/filled vectors.
- Static operations including `cross`, `reflect`, `lerp`, and related helpers.
- Array-loading APIs and vector swizzles.

Same-backend wrapper conversions now copy the backend value directly.
Cross-backend conversions extract components only when necessary. `vec3` and
`vec4` swizzles were adjusted to return canonical public vector types rather
than backend-specific types.

Compile-time assertions were added to ensure representative operations continue
to return the documented public types.

### 2. Vector construction and API parity defects

The scalar and RTM `vec4` axis factories omitted the fourth constructor
component in several cases. These constructors were corrected.

`vec2` also lacked approximate-equality parity with the other vector types. A
component-wise `approx_equal` overload was added, and the `vec3`/`vec4`
implementations were made backend-independent and consistent for special
floating-point values.

### 3. Integral geometry produced truncated or unsafe results

Length and distance operations used the vector component type as their result
type. For integral vectors, this truncated square roots and could perform
intermediate arithmetic in a narrow signed type.

A `geometry_scalar_t<T>` result type was introduced:

- Floating-point geometry retains its component type.
- Integral lengths, squared lengths, and distances are promoted to `double`.
- Distance subtraction is performed in the promoted domain to avoid signed
  subtraction overflow.
- Square-root helpers likewise return the promoted geometry type.

Normalization and angle operations are now constrained to floating-point
vectors. Integral vectors still support meaningful discrete operations such as
component arithmetic, dot/cross products, and geometry measurements.

Tests were updated so integral interpolation uses representable endpoint cases
instead of expecting fractional results from integer components.

### 4. Normalization and vector-angle edge cases were inconsistent

Scalar and RTM paths treated tiny vectors differently. Some RTM operations used
an epsilon threshold and discarded valid non-zero vectors, while other paths
could produce non-finite results for zero vectors.

The corrected contract is:

- A zero vector normalizes to zero.
- A tiny but non-zero vector is normalized.
- The angle involving a zero vector returns zero as a finite fallback.
- Dot products used by `acos` are clamped to `[-1, 1]` to absorb ordinary
  floating-point drift.

This behavior is implemented for scalar and RTM `vec3`/`vec4` paths and covered
by parity tests.

### 5. Scalar helper and comparison semantics were incorrect

The integral implementation of `saturate` mapped every non-zero value to one,
including negative values. It now performs a true clamp to `[0, 1]`.

Approximate comparison was revised to address several issues:

- The generic overload is constrained to arithmetic types.
- Integral differences are computed without overflowing at signed extrema.
- Negative tolerances are rejected.
- Floating-point comparison uses absolute and relative tolerance.
- Identically signed infinities compare equal.
- NaNs never compare approximately equal.

These rules are now documented and have direct regression tests, including
minimum/maximum 64-bit integer inputs.

### 6. Quaternion equality had surprising semantics

Quaternion `operator==` previously performed an approximate comparison. Exact
equality and approximate equality are different contracts and should not be
silently interchangeable.

The API now provides three explicit meanings:

- `operator==` performs exact component equality.
- `approx_equal` performs approximate component equality.
- `same_rotation` treats `q` and `-q` as equivalent rotations.

Quaternion approximate comparison also follows the library-wide NaN and
infinity rules.

### 7. Quaternion logarithm and exponential were incomplete

The RTM extension implemented behavior suitable only for a subset of unit
quaternions. It did not provide the general quaternion logarithm/exponential
contract, particularly for non-unit values and negative scalar components.

The implementation now uses the general formulas:

- `log(q)` includes `log(|q|)` in the scalar component and uses
  `atan2(|v|, w)` for the vector component.
- `exp(q)` applies `exp(w)` and the trigonometric vector factor.
- Negative-real and zero degeneracies have deterministic finite fallbacks.

Regression tests cover negative-`w` rotations, non-unit quaternions, and
`exp(log(q))` round trips for both `float` and `double`.

### 8. Quaternion normalization and look rotation were fragile

Normalizing or inverting a zero quaternion could produce invalid values.
Zero now returns zero as a finite fallback.

`look_rotation` was replaced with an explicit orthonormal-basis construction:

- Zero forward returns identity.
- Forward is normalized.
- A fallback up axis is selected when forward and up are collinear.
- The resulting basis is converted to a quaternion using the appropriate RTM
  matrix type.

Tests cover identity orientation, looking right, zero forward, and collinear
forward/up inputs.

### 9. Type traits were incomplete

`component_type_t` assumed every input was a library aggregate with a nested
`component_type`, so it did not work for scalar types. It now uses detection and
falls back to the input scalar type.

Matrix traits were added for fast and storage matrix types:

- `is_matrix_type_v`
- `component_count_v` with values 9 and 16
- `component_type` support for storage matrices

`storage_mat3x3` can now convert to `mat3x3`, matching the intended storage/fast
workflow.

### 10. Matrix storage performed unnecessary copying

`mat3x3::store_array` and `load_array` used temporary four-lane arrays and
manually repacked three components per axis. They now use RTM's direct
three-component load/store operations.

In the inspected optimized workload, the matrix storage routine decreased from
75 bytes to 53 bytes of generated code, approximately a 29% reduction. The
vector workload grew from 274 bytes to 338 bytes because the corrected
normalization contract requires explicit zero handling. This is a deliberate
correctness cost rather than an overall code-size claim.

### 11. Blanket force-inlining increased code-generation pressure

The common inline macros forced every annotated function to inline. That can
increase code size and remove useful compiler discretion without demonstrating
a benefit at each call site.

The default macros now use ordinary `inline`/`[[nodiscard]] inline`.
`MVM_FORCE_INLINE` remains available for individual, measured hot paths.

### 12. The test matrix did not run every intended combination

The Catch2 helper invoked multiple template types inside shared section paths.
Catch2 section traversal meant not every type/backend instantiation executed its
test body independently.

Each type/backend combination now runs in a uniquely named `DYNAMIC_SECTION`.
`vec2` was migrated to the same helper used by the other vector tests. Tests
were also adjusted to respect the corrected numeric domains: floating-only
normalization and angles, promoted integral geometry results, and valid integer
interpolation expectations.

Additional regression coverage was added for:

- Wrapper return types and compound-assignment identity.
- Scalar/RTM tiny-vector and zero-vector parity.
- Saturation and extreme integer comparisons.
- NaN and infinity comparison behavior.
- Quaternion equality, equivalence, logarithm/exponential, and look rotation.
- Matrix traits and storage conversions.
- Documented vector/quaternion rotation syntax.

### 13. The root build did not compile C++ or expose a lightweight test

The root CMake project declared `LANGUAGES NONE`, did not state the required C++
standard, and had no dependency-light validation path.

The root configuration now:

- Enables C++.
- Requires C++20 through the interface target.
- Provides `MOVE_VECTORMATH_BUILD_STANDALONE_TESTS`.
- Originally provided `MOVE_VECTORMATH_BUILD_TESTS` for the legacy Catch2
  suite; that target was retired after its contracts migrated to the focused
  RTM/scalar current-API suites.
- Builds a public-API and numerical-contract executable when that option is on.
- Enables `-Wall -Wextra -Wconversion` for GCC/Clang and `/W4` for MSVC on that
  target.
- Can promote warnings in repository-owned test targets to errors.
- Registers the executable with CTest.

The standalone test uses active runtime checks even in Release builds and
compile-time assertions for API/trait contracts.

### 14. Continuous integration and metadata were incomplete

A GitHub Actions workflow now runs on every push and pull request and covers:

- GCC 12 in Release and Debug.
- Clang 16 in Release.
- AppleClang on macOS 15 arm64.
- MSVC on Windows 2022.
- AddressSanitizer and UndefinedBehaviorSanitizer.
- GCC coverage generation with an uploaded Cobertura report.
- Whitespace and maintained test-harness formatting checks.
- A separate FetchContent consumer configure/build/test.

The build matrix runs both the complete Catch2 suite and the dependency-light
standalone target. Linux Release, Debug, sanitizer, coverage-instrumented, and
downstream-consumer paths were validated locally; macOS and Windows remain
configured for hosted CI. The obsolete disabled workflow template was removed.
Workflow permissions are read-only, redundant runs are cancelled, jobs have
timeouts, dependencies are cached within each job, and matrix failures do not
cancel unrelated platforms.

The package metadata now contains a meaningful description, version `0.2.0`,
MIT license metadata, and relevant search keywords.

## Documentation and Compatibility Decisions

`CONVENTIONS.md` now records the corrected contracts for:

- Quaternion equality, approximate equality, and rotation equivalence.
- General quaternion logarithm/exponential.
- Zero quaternion and `look_rotation` behavior.
- Integral geometry promotion.
- Floating-only normalization and angle operations.
- Zero/tiny vector handling.
- NaN and infinity comparison semantics.

The scalar vector/storage layouts use anonymous unions to preserve public
`x`/`y`/`z`/`w` fields and contiguous `data` access. Replacing these unions with
strictly standard C++ would break the existing field-access API or layout
expectations. The implementation therefore retains the extension supported by
GCC, Clang, and MSVC and documents that strict `-pedantic-errors` builds are not
supported without a compatibility break.

## Verification Performed

The maintained test-harness files were formatted and the final source state
passed `git diff --check`.

The following validation completed successfully:

- Root CMake Release and Debug configure/builds with the complete Catch2 suite
  and standalone tests enabled.
- CTest: 13 of 13 discovered tests passed in Release and Debug.
- A separate downstream FetchContent consumer configured, built, and passed.
- Coverage instrumentation configured, built, ran all 13 tests, and produced
  98 runtime coverage data files; CI performs the final gcovr report step.
- Direct Release builds with GCC and Clang using `-O2 -DNDEBUG`,
  `-Wall`, `-Wextra`, and `-Wconversion`.
- Standalone compilation of every public `.hpp` header with GCC.
- Syntax compilation of all original Catch2 test translation units with GCC
  and Clang.
- Full directly linked Catch2 suite:
  **17,064 assertions in 12 test cases passed**.
- Full Catch2 suite under AddressSanitizer and UndefinedBehaviorSanitizer:
  **17,064 assertions in 12 test cases passed**.

Leak detection was disabled for the sanitizer run because LeakSanitizer is not
available under the environment's ptrace configuration. Address and undefined
behavior instrumentation remained enabled.

## Remaining External Limitation

The repository's usual `move-cli test move/math` entry point does not currently
reach vectormath compilation under GCC 16. It fails first in the external
`move-core` dependency's `virtual_stable_vector.hpp`, where `_alloc` and `_data`
are not found.

This dependency issue was not modified because it is outside this repository's
scope. To validate vectormath independently, the complete Catch2 suite was
compiled, linked, and executed directly, in addition to the new standalone
CMake/CTest path.

## Files Added

- `.github/workflows/vectormath.yml`
- `tests/standalone.cpp`
- `tests/consumer/CMakeLists.txt`
- `tests/consumer/main.cpp`
- `AUDIT_REPORT.md`

## Outcome

The branch now has explicit and tested numeric contracts, consistent public
wrapper APIs, robust quaternion behavior, improved matrix storage, broader type
traits, effective test-matrix execution, a standalone build path, and
cross-platform CI configuration. All locally runnable vectormath verification
passes; the only known blocked entry point is caused by the upstream
`move-core` GCC 16 incompatibility described above.
