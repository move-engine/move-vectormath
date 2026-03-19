# Review: Current Vectormath State

## Findings

### Medium: `vec2` is still split into multiple nominally different scalar-only types
- [packages/move/math/include/move/math/vec2.hpp:131](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec2.hpp#L131) through [packages/move/math/include/move/math/vec2.hpp:149](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec2.hpp#L149) still define many `fast_*2` aliases as `vec2<..., Acceleration::Default>` even though `vec2` is now documented as scalar-only.
- [packages/move/math/include/move/math/vec3.hpp:144](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec3.hpp#L144) and [packages/move/math/include/move/math/vec4.hpp:143](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec4.hpp#L143) propagate that template parameter into swizzle return types, so an RTM `float3::xy()` returns `vec2<float, Acceleration::RTM>`, not `float2`.
- I confirmed this with a local compile probe: `decltype(move::math::float3{}.xy())` is not `move::math::float2`, even though both report scalar acceleration. That leaves overloads, template specialization, and `std::is_same_v`-based code observing multiple distinct “scalar vec2” types after the cleanup pass that was supposed to make the surface honest.

### Medium: `quat::operator[]` still silently returns zero on out-of-range access
- [packages/move/math/include/move/math/quat.hpp:154](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/quat.hpp#L154) through [packages/move/math/include/move/math/quat.hpp:168](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/quat.hpp#L168) return `T(0)` for any index outside `[0, 3]`.
- That is now inconsistent with the vector types, which were explicitly moved to `assert(index < element_count)` in this pass.
- I confirmed the behavior with a small local probe: `quatf(1, 2, 3, 4)[4]` evaluates to `0`. This will silently mask caller bugs instead of failing fast, and there is no test coverage for quaternion indexing.

### Low: `approx_equal` semantics are now inconsistent across vectors, matrices, and quaternions
- [packages/move/math/include/move/math/quat.hpp:380](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/quat.hpp#L380) through [packages/move/math/include/move/math/quat.hpp:387](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/quat.hpp#L387) explicitly treat paired `NaN` lanes as equal.
- [packages/move/math/include/move/math/vec3.hpp:316](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec3.hpp#L316) through [packages/move/math/include/move/math/vec3.hpp:336](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec3.hpp#L316) and [packages/move/math/include/move/math/vec4.hpp:500](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec4.hpp#L500) through [packages/move/math/include/move/math/vec4.hpp:520](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/vec4.hpp#L500) do not preserve that behavior on the RTM path.
- I confirmed the mismatch with a local probe: paired-`NaN` `float3`/`float4` values return `false` from `approx_equal`, while the quaternion helper returns `true`.
- Because `mat3x3` and `mat4x4` approximate comparison now delegate through those vector helpers, the inconsistency now depends on the math type rather than on a single shared rule.

### Low: `vec4` reflection still depends on an RTM conversion path that is already deprecated
- [packages/move/math/include/move/math/rtm/base_vec4.hpp:525](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/rtm/base_vec4.hpp#L525) through [packages/move/math/include/move/math/rtm/base_vec4.hpp:526](/home/claire/src/move-engine/move-vectormath/packages/move/math/include/move/math/rtm/base_vec4.hpp#L526) feed the result of `vector_dot(...)` directly into `vector_add(...)`.
- A clean `move-cli test` build on the current tree emits RTM deprecation warnings here for both float and double, because this still relies on the old implicit proxy-to-vector conversion.
- It is not a correctness failure today, but it is a pinned dependency upgrade hazard and the build is no longer warning-clean in this area.

## Notes

- I did not find another critical math correctness regression in the current tree beyond the issues above.
- This pass was based on source review plus targeted local compile/runtime probes for the suspicious cases.
