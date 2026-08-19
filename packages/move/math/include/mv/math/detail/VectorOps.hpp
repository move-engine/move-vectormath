#pragma once

#include <cmath>
#include <type_traits>

#include <mv/math/Config.hpp>

#if !defined(MV_MATH_FORCE_SCALAR)
#include <rtm/vector4f.h>
#endif

namespace mv::math::detail
{
    // Provenance: MVM-PROV-A-VECTOR-OPS.
    template <typename T>
    struct ScalarNative4
    {
        T X;
        T Y;
        T Z;
        T W;
    };

    template <typename T>
    struct ScalarVectorOps
    {
        using Native = ScalarNative4<T>;

        [[nodiscard]] static constexpr Native Load2(const T* value) noexcept
        {
            return {value[0], value[1], T(0), T(0)};
        }

        [[nodiscard]] static constexpr Native Load3(const T* value) noexcept
        {
            return {value[0], value[1], value[2], T(0)};
        }

        [[nodiscard]] static constexpr Native Load4(const T* value) noexcept
        {
            return {value[0], value[1], value[2], value[3]};
        }

        static constexpr void Store2(Native value, T* destination) noexcept
        {
            destination[0] = value.X;
            destination[1] = value.Y;
        }

        static constexpr void Store3(Native value, T* destination) noexcept
        {
            destination[0] = value.X;
            destination[1] = value.Y;
            destination[2] = value.Z;
        }

        static constexpr void Store4(Native value, T* destination) noexcept
        {
            destination[0] = value.X;
            destination[1] = value.Y;
            destination[2] = value.Z;
            destination[3] = value.W;
        }

        [[nodiscard]] static constexpr T GetX(Native value) noexcept
        {
            return value.X;
        }

        [[nodiscard]] static constexpr T GetY(Native value) noexcept
        {
            return value.Y;
        }

        [[nodiscard]] static constexpr T GetZ(Native value) noexcept
        {
            return value.Z;
        }

        [[nodiscard]] static constexpr Native SetX(Native value,
                                                   T lane) noexcept
        {
            value.X = lane;
            return value;
        }

        [[nodiscard]] static constexpr Native SetY(Native value,
                                                   T lane) noexcept
        {
            value.Y = lane;
            return value;
        }

        [[nodiscard]] static constexpr Native SetZ(Native value,
                                                   T lane) noexcept
        {
            value.Z = lane;
            return value;
        }

        [[nodiscard]] static constexpr Native Add(Native left,
                                                  Native right) noexcept
        {
            return {left.X + right.X, left.Y + right.Y, left.Z + right.Z,
                    left.W + right.W};
        }

        [[nodiscard]] static constexpr Native Subtract(Native left,
                                                       Native right) noexcept
        {
            return {left.X - right.X, left.Y - right.Y, left.Z - right.Z,
                    left.W - right.W};
        }

        [[nodiscard]] static constexpr Native Multiply(Native left,
                                                       Native right) noexcept
        {
            return {left.X * right.X, left.Y * right.Y, left.Z * right.Z,
                    left.W * right.W};
        }

        [[nodiscard]] static constexpr Native Multiply(Native value,
                                                       T scalar) noexcept
        {
            return {value.X * scalar, value.Y * scalar, value.Z * scalar,
                    value.W * scalar};
        }

        [[nodiscard]] static constexpr T Dot2(Native left,
                                              Native right) noexcept
        {
            return left.X * right.X + left.Y * right.Y;
        }

        [[nodiscard]] static constexpr T Dot3(Native left,
                                              Native right) noexcept
        {
            return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
        }

        [[nodiscard]] static constexpr T Dot4(Native left,
                                              Native right) noexcept
        {
            return left.X * right.X + left.Y * right.Y + left.Z * right.Z +
                   left.W * right.W;
        }

        [[nodiscard]] static constexpr Native Cross3(Native left,
                                                     Native right) noexcept
        {
            return {left.Y * right.Z - left.Z * right.Y,
                    left.Z * right.X - left.X * right.Z,
                    left.X * right.Y - left.Y * right.X, T(0)};
        }
    };

#if !defined(MV_MATH_FORCE_SCALAR)
    struct RtmFloatVectorOps
    {
        using Native = rtm::vector4f;

        [[nodiscard]] static Native Load2(const float* value) noexcept
        {
            return rtm::vector_set(value[0], value[1], 0.0F, 0.0F);
        }

        [[nodiscard]] static Native Load3(const float* value) noexcept
        {
            return rtm::vector_load3(value);
        }

        [[nodiscard]] static Native Load4(const float* value) noexcept
        {
            return rtm::vector_load(value);
        }

        static void Store2(Native value, float* destination) noexcept
        {
            destination[0] = rtm::vector_get_x(value);
            destination[1] = rtm::vector_get_y(value);
        }

        static void Store3(Native value, float* destination) noexcept
        {
            rtm::vector_store3(value, destination);
        }

        static void Store4(Native value, float* destination) noexcept
        {
            rtm::vector_store(value, destination);
        }

        [[nodiscard]] static float GetX(Native value) noexcept
        {
            return rtm::vector_get_x(value);
        }

        [[nodiscard]] static float GetY(Native value) noexcept
        {
            return rtm::vector_get_y(value);
        }

        [[nodiscard]] static float GetZ(Native value) noexcept
        {
            return rtm::vector_get_z(value);
        }

        [[nodiscard]] static Native SetX(Native value, float lane) noexcept
        {
            return rtm::vector_set_x(value, lane);
        }

        [[nodiscard]] static Native SetY(Native value, float lane) noexcept
        {
            return rtm::vector_set_y(value, lane);
        }

        [[nodiscard]] static Native SetZ(Native value, float lane) noexcept
        {
            return rtm::vector_set_z(value, lane);
        }

        [[nodiscard]] static Native Add(Native left, Native right) noexcept
        {
            return rtm::vector_add(left, right);
        }

        [[nodiscard]] static Native Subtract(Native left, Native right) noexcept
        {
            return rtm::vector_sub(left, right);
        }

        [[nodiscard]] static Native Multiply(Native left, Native right) noexcept
        {
            return rtm::vector_mul(left, right);
        }

        [[nodiscard]] static Native Multiply(Native value,
                                             float scalar) noexcept
        {
            return rtm::vector_mul(value, scalar);
        }

        [[nodiscard]] static float Dot2(Native left, Native right) noexcept
        {
            return rtm::vector_dot2(left, right);
        }

        [[nodiscard]] static float Dot3(Native left, Native right) noexcept
        {
            return rtm::vector_dot3(left, right);
        }

        [[nodiscard]] static float Dot4(Native left, Native right) noexcept
        {
            return rtm::vector_dot(left, right);
        }

        [[nodiscard]] static Native Cross3(Native left, Native right) noexcept
        {
            return rtm::vector_cross3(left, right);
        }
    };
#endif

    template <typename T>
    struct SelectedVectorOpsSelector
    {
        using Type = ScalarVectorOps<T>;
    };

#if !defined(MV_MATH_FORCE_SCALAR)
    template <>
    struct SelectedVectorOpsSelector<float>
    {
        using Type = RtmFloatVectorOps;
    };
#endif

    template <typename T>
    using SelectedVectorOps = typename SelectedVectorOpsSelector<T>::Type;
}  // namespace mv::math::detail
