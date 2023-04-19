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
    namespace fastmath = DirectX;
    struct fastvec2;
    struct fastvec3;
    struct fastnorm3;
    struct fastvec4;
    struct fastmat3;
    struct fastmat4;
    struct fastquat;

    struct vector2;
    struct vector3;
    struct vector4;
    struct quaternion;
    struct matrix3;
    struct matrix4;
    struct irect;
    struct frect;

    inline float deg_to_rad(float deg)
    {
        return fastmath::XMConvertToRadians(deg);
    }

    inline float rad_to_deg(float rad)
    {
        return fastmath::XMConvertToDegrees(rad);
    }
}  // namespace move::vectormath