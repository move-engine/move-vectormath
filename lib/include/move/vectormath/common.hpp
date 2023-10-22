#pragma once

#include <DirectXMath.h>

#if defined(_MSC_VER)
#define MVM_ALIGNED __declspec(align(16))
#elif defined(__clang__)
#define MVM_ALIGNED __attribute__((aligned(16)))
#elif defined(__GNUC__)
#define MVM_ALIGNED __attribute__((aligned(16)))
#else
#error "Unknown compiler"
#endif

namespace move::vectormath
{
    namespace internal
    {
        namespace DirectX = ::DirectX;  // NOLINT
    }
    struct fastvec2;
    struct fastvec3;
    struct fastnorm3;
    struct fastvec4;
    struct fastmat3;
    struct fastmat4;
    struct fastquat;

    struct vec2;
    struct vec3;
    // struct vec4f;
    struct quat;
    struct mat3;
    struct generic_mat4;
    struct irect;
    struct frect;

    struct ivec2;
    struct ivec3;
    struct ivec4;

    inline float deg_to_rad(float deg)
    {
        return internal::DirectX::XMConvertToRadians(deg);
    }

    inline float rad_to_deg(float rad)
    {
        return internal::DirectX::XMConvertToDegrees(rad);
    }
}  // namespace move::vectormath