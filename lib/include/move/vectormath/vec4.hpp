#pragma once
#include "vec4_rtm.hpp"
#include "vec4_scalar.hpp"

namespace move::vectormath
{
#if defined(MVM_NO_RTM)
    using vec4f32 = vec4f32_scalar;
    using vec4f64 = vec4f64_scalar;
#else
    using vec4f32 = vec4f32_rtm;
    using vec4f64 = vec4f64_rtm;
#endif

    using vec4i8 = vec4i8_scalar;
    using vec4i16 = vec4i16_scalar;
    using vec4i32 = vec4i32_scalar;
    using vec4i64 = vec4i64_scalar;

    using vec4u8 = vec4u8_scalar;
    using vec4u16 = vec4u16_scalar;
    using vec4u32 = vec4u32_scalar;
    using vec4u64 = vec4u64_scalar;

    using vec4f = vec4f32;
    using vec4d = vec4f64;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using vec4 = vec4d;
    using vec4i = vec4i64;
    using vec4u = vec4u64;
#else
    using vec4 = vec4f;
    using vec4i = vec4i32;
    using vec4u = vec4u32;
#endif

    using fvec4 = vec4;
    using ivec4 = vec4i;
    using uvec4 = vec4u;
}  // namespace move::vectormath
