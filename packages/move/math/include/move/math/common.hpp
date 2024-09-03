#pragma once
#include <cmath>

#include <move/math/macros.hpp>
#include <type_traits>

namespace move::math
{
    /*
     * @brief Enum to specify the acceleration type to use.
     *
     * @note Passing SIMD to a given type or function does not guarantee that it
     * will use SIMD.  If a SIMD implementation is not available, it will fall
     * back to Scalar.
     */
    enum class Acceleration
    {
        Default,
        Scalar,
        RTM
    };

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T sqrt(const T& value)
    {
        return std::sqrt(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T sqrt_reciprocal(const T& value)
    {
        return T(1) / std::sqrt(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T abs(const T& value)
    {
        return std::abs(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T min(const T& a, const T& b)
    {
        return a < b ? a : b;
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T max(const T& a, const T& b)
    {
        return a > b ? a : b;
    }

    template <typename T>
    MVM_INLINE constexpr void minmax(const T& a, const T& b, T& min, T& max)
    {
        if (a < b)
        {
            min = a;
            max = b;
        }
        else
        {
            min = b;
            max = a;
        }
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T clamp(const T& value,
                                           const T& min,
                                           const T& max)
    {
        return math::min(math::max(value, min), max);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T saturate(T value)
    {
        if constexpr (std::is_integral_v<T>)
        {
            return value == 0 ? 0 : 1;
        }
        else
        {
            return clamp(value, T(0), T(1));
        }
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T lerp_unclamped(const T& a,
                                                    const T& b,
                                                    const T& t)
    {
        return a + (b - a) * t;
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T lerp(const T& a, const T& b, const T& t)
    {
        return lerp_unclamped(a, b, saturate(t));
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T invlerp(T a, T b, T value)
    {
        return (value - a) / (b - a);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T smoothstep(T a, T b, T t)
    {
        t = clamp((t - a) / (b - a), 0, 1);
        return t * t * (3 - 2 * t);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T smootherstep(T a, T b, T t)
    {
        t = clamp((t - a) / (b - a), 0, 1);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T pow(T value, T exponent)
    {
        return std::pow(value, exponent);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T exp(T value)
    {
        return std::exp(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T log(T value)
    {
        return std::log(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T log2(T value)
    {
        return std::log2(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T log10(T value)
    {
        return std::log10(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T logn(T value, T base)
    {
        return log(value) / log(base);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T sign(const T& value)
    {
        return value < T(0) ? T(-1) : T(1);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T floor(const T& value)
    {
        return std::floor(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T ceil(const T& value)
    {
        return std::ceil(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T round(const T& value)
    {
        return std::round(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T trunc(const T& value)
    {
        return std::trunc(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T frac(const T& value)
    {
        return value - floor(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T mod(const T& value, const T& divisor)
    {
        return value - divisor * floor(value / divisor);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T remainder(const T& value, const T& divisor)
    {
        return std::remainder(value, divisor);
    }

    template <typename T>
    MVM_INLINE_NODISCARD constexpr T pi()
    {
        // Sorry for the massive constant, wanted this to be generic :P
        constexpr T piconst = T(
            3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923);
        return T(piconst);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t halfpi()
    {
        return pi<value_t>() / value_t(2);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t quarterpi()
    {
        return pi<value_t>() / value_t(4);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t two_pi()
    {
        return pi<value_t>() * value_t(2);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t deg2rad(value_t degrees)
    {
        return degrees * pi<value_t>() / value_t(180);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t rad2deg(value_t radians)
    {
        return radians * value_t(180) / pi<value_t>();
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t sin(value_t value)
    {
        return std::sin(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t cos(value_t value)
    {
        return std::cos(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t tan(value_t value)
    {
        return std::tan(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t asin(value_t value)
    {
        return std::asin(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t acos(value_t value)
    {
        return std::acos(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t atan(value_t value)
    {
        return std::atan(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t atan2(value_t y, value_t x)
    {
        return std::atan2(y, x);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t sinh(value_t value)
    {
        return std::sinh(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t cosh(value_t value)
    {
        return std::cosh(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t tanh(value_t value)
    {
        return std::tanh(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t asinh(value_t value)
    {
        return std::asinh(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t acosh(value_t value)
    {
        return std::acosh(value);
    }

    template <typename value_t>
    MVM_INLINE_NODISCARD constexpr value_t atanh(value_t value)
    {
        return std::atanh(value);
    }

    // If floating typeness of both is equal, use the bigger type.
    // Otherwise use the one that is a floating point type.
    template <typename T1, typename T2>
    using most_detailed_t = std::conditional_t<
        std::is_floating_point_v<T1> == std::is_floating_point_v<T2>,
        std::conditional_t<(sizeof(T1) > sizeof(T2)), T1, T2>,
        std::conditional_t<std::is_floating_point_v<T1>, T1, T2>>;

    template <typename T1,
              typename T2,
              typename EpsilonT = most_detailed_t<T1, T2>>
    MVM_INLINE_NODISCARD constexpr bool safe_equal(
        const T1& a,
        const T2& b,
        const EpsilonT& epsilon =
            std::numeric_limits<EpsilonT>::epsilon()) requires
        std::is_floating_point_v<T1> || std::is_floating_point_v<T2>
    {
        using type_to_compare = EpsilonT;
        return abs(static_cast<type_to_compare>(a) -
                   static_cast<type_to_compare>(b)) <= epsilon;
    }

    template <typename T1, typename T2>
    MVM_INLINE_NODISCARD constexpr bool safe_equal(
        const T1& a,
        const T2& b) requires std::is_integral_v<T1> && std::is_integral_v<T2>
    {
        return a == b;
    }

    template <typename T1,
              typename T2,
              typename EpsilonT = most_detailed_t<T1, T2>>
    MVM_INLINE_NODISCARD constexpr bool approx_equal(
        const T1& a,
        const T2& b,
        const EpsilonT& epsilon = std::numeric_limits<EpsilonT>::epsilon())
    {
        using type_to_compare = EpsilonT;
        if constexpr (std::is_unsigned_v<type_to_compare>)
        {
            type_to_compare min;
            type_to_compare max;
            minmax(static_cast<type_to_compare>(a),
                   static_cast<type_to_compare>(b), min, max);
            return max - min <= epsilon;
        }
        else
        {
            return abs<type_to_compare>(static_cast<type_to_compare>(a) -
                                        static_cast<type_to_compare>(b)) <=
                   epsilon;
        }
    }
}  // namespace move::math