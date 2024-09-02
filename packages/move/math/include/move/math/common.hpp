#pragma once
#include <cmath>

#include <move/math/macros.hpp>
#include <type_traits>

namespace move::math
{
    enum class Acceleration
    {
        Scalar,
        SIMD,
        Default = Scalar
    };

    template <typename T>
    MVM_INLINE_NODISCARD T sqrt(const T& value)
    {
        return std::sqrt(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T sqrt_reciprocal(const T& value)
    {
        return T(1) / std::sqrt(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T abs(const T& value)
    {
        return std::abs(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T min(const T& a, const T& b)
    {
        return std::min(a, b);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T max(const T& a, const T& b)
    {
        return std::max(a, b);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T clamp(const T& value, const T& min, const T& max)
    {
        return min(max(value, min), max);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T lerp(const T& a, const T& b, const T& t)
    {
        return a + (b - a) * t;
    }

    template <typename T>
    MVM_INLINE_NODISCARD T sign(const T& value)
    {
        return value < T(0) ? T(-1) : T(1);
    }

    // If floating typeness of both is equal, use the bigger type.
    // Otherwise use the one that is a floating point type.
    template <typename T1, typename T2>
    using refined_t = std::conditional_t<
        std::is_floating_point_v<T1> == std::is_floating_point_v<T2>,
        std::conditional_t<(sizeof(T1) > sizeof(T2)), T1, T2>,
        std::conditional_t<std::is_floating_point_v<T1>, T1, T2>>;

    template <typename T1, typename T2, typename EpsilonT = refined_t<T1, T2>>
    MVM_INLINE_NODISCARD bool approx_equal(
        const T1& a,
        const T2& b,
        const EpsilonT& epsilon = std::numeric_limits<EpsilonT>::epsilon())
    {
        if constexpr (std::is_floating_point_v<T1> ||
                      std::is_floating_point_v<T2>)
        {
            using type_to_compare = EpsilonT;
            return abs(static_cast<type_to_compare>(a) -
                       static_cast<type_to_compare>(b)) <= epsilon;
        }
        else
        {
            return a == b;
        }
    }
}  // namespace move::math