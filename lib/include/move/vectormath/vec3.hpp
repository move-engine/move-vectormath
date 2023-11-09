#pragma once
#include "vec3_rtm.hpp"
#include "vec3_scalar.hpp"

namespace move::vectormath
{
#if defined(MVM_NO_RTM)
    using vec3f32 = vec3f32_scalar;
    using vec3f64 = vec3f64_scalar;
#else
    using vec3f32 = vec3f_rtm;
    using vec3f64 = vec3d_rtm;
#endif

    using vec3i8 = vec3i8_scalar;
    using vec3i16 = vec3i16_scalar;
    using vec3i32 = vec3i32_scalar;
    using vec3i64 = vec3i64_scalar;

    using vec3u8 = vec3u8_scalar;
    using vec3u16 = vec3u16_scalar;
    using vec3u32 = vec3u32_scalar;
    using vec3u64 = vec3u64_scalar;

    using vec3f = vec3f32;
    using vec3d = vec3f64;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using vec3 = vec3d;
    using vec3i = vec3i64;
    using vec3u = vec3u64;
#else
    using vec3 = vec3f;
    using vec3i = vec3i32;
    using vec3u = vec3u32;
#endif

    using fvec3 = vec3;
    using ivec3 = vec3i;
    using uvec3 = vec3u;
}  // namespace move::vectormath