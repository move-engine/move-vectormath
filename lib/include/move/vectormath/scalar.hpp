#pragma once
#include <rtm/scalarf.h>
#include <cmath>
#include <type_traits>

namespace move::vectormath::scalar
{
    template <typename value_t>
    inline constexpr value_t& min_ref(value_t& a, value_t& b)
    {
        return a < b ? a : b;
    }

    template <typename value_t>
    inline constexpr value_t min(value_t a, value_t b)
    {
        return a < b ? a : b;
    }

    template <typename value_t>
    inline constexpr value_t& max_ref(value_t& a, value_t& b)
    {
        return a > b ? a : b;
    }

    template <typename value_t>
    inline constexpr value_t max(value_t a, value_t b)
    {
        return a > b ? a : b;
    }

    template <typename value_t>
    inline constexpr value_t clamp(value_t value, value_t min, value_t max)
    {
        return value < min ? min : value > max ? max : value;
    }

    template <typename value_t>
    inline constexpr value_t saturate(value_t value)
    {
        return value < 0 ? 0 : value > 1 ? 1 : value;
    }

    template <typename value_t>
    inline constexpr value_t lerp_unclamped(value_t a, value_t b, value_t t)
    {
        return a + (b - a) * t;
    }

    template <typename value_t>
    inline constexpr value_t lerp(value_t a, value_t b, value_t t)
    {
        return lerp_clamped(a, b, t);
    }

    template <typename value_t>
    inline constexpr value_t invlerp(value_t a, value_t b, value_t value)
    {
        return (value - a) / (b - a);
    }

    template <typename value_t>
    inline constexpr value_t smoothstep(value_t a, value_t b, value_t t)
    {
        t = clamp((t - a) / (b - a), 0, 1);
        return t * t * (3 - 2 * t);
    }

    template <typename value_t>
    inline constexpr value_t smootherstep(value_t a, value_t b, value_t t)
    {
        t = clamp((t - a) / (b - a), 0, 1);
        return t * t * t * (t * (t * 6 - 15) + 10);
    }

    template <typename value_t>
    inline constexpr value_t pow(value_t value, value_t exponent)
    {
        return std::pow(value, exponent);
    }

    template <typename value_t>
    inline constexpr value_t exp(value_t value)
    {
        return std::exp(value);
    }

    template <typename value_t>
    inline constexpr value_t log(value_t value)
    {
        return std::log(value);
    }

    template <typename value_t>
    inline constexpr value_t log2(value_t value)
    {
        return std::log2(value);
    }

    template <typename value_t>
    inline constexpr value_t log10(value_t value)
    {
        return std::log10(value);
    }

    template <typename value_t>
    inline constexpr value_t logn(value_t value, value_t base)
    {
        return log(value) / log(base);
    }

    template <typename value_t>
    inline constexpr value_t reciprocal(value_t value)
    {
        return value_t(1) / value;
    }

    template <typename value_t>
    inline constexpr value_t sqrt(value_t value)
    {
        return std::sqrt(value);
    }

    template <typename value_t>
    inline constexpr value_t rsqrt(value_t value)
    {
        return reciprocal(sqrt(value));
    }

    template <typename value_t>
    inline constexpr value_t abs(value_t value)
    {
        return value < 0 ? -value : value;
    }

    template <typename value_t>
    inline constexpr value_t approx(value_t a, value_t b,
        value_t epsilon = std::numeric_limits<value_t>::epsilon())
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(int8_t a, int8_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(int8_t a, int8_t b, int8_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(int16_t a, int16_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(int16_t a, int16_t b, int16_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(int32_t a, int32_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(int32_t a, int32_t b, int32_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(int64_t a, int64_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(int64_t a, int64_t b, int64_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(uint8_t a, uint8_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(uint8_t a, uint8_t b, uint8_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(uint16_t a, uint16_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(uint16_t a, uint16_t b, uint16_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(uint32_t a, uint32_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(uint32_t a, uint32_t b, uint32_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    inline constexpr bool approx(uint64_t a, uint64_t b)
    {
        return a == b;
    }

    inline constexpr bool approx(uint64_t a, uint64_t b, uint64_t epsilon)
    {
        return abs(a - b) < epsilon;
    }

    template <typename value_t>
    inline constexpr value_t sign(value_t value)
    {
        return value < 0 ? -1 : value > 0 ? 1 : 0;
    }

    template <typename value_t>
    inline constexpr value_t floor(value_t value)
    {
        return std::floor(value);
    }

    template <typename value_t>
    inline constexpr value_t ceil(value_t value)
    {
        return std::ceil(value);
    }

    template <typename value_t>
    inline constexpr value_t round(value_t value)
    {
        return std::round(value);
    }

    template <typename value_t>
    inline constexpr value_t frac(value_t value)
    {
        return value - floor(value);
    }

    template <typename value_t>
    inline constexpr value_t mod(value_t value, value_t divisor)
    {
        return value - floor(value / divisor) * divisor;
    }

    template <typename value_t>
    inline constexpr value_t pi()
    {
        // Sorry for the massive constant, wanted this to be generic :P
        constexpr value_t piconst = value_t(
            3.141592653589793238462643383279502884197169399375105820974944592307816406286208998628034825342117067982148086513282306647093844609550582231725359408128481117450284102701938521105559644622948954930381964428810975665933446128475648233786783165271201909145648566923);
        return value_t(piconst);
    }

    template <typename value_t>
    inline constexpr value_t halfpi()
    {
        return pi<value_t>() / value_t(2);
    }

    template <typename value_t>
    inline constexpr value_t quarterpi()
    {
        return pi<value_t>() / value_t(4);
    }

    template <typename value_t>
    inline constexpr value_t two_pi()
    {
        return pi<value_t>() * value_t(2);
    }

    template <typename value_t>
    inline constexpr value_t deg2rad(value_t degrees)
    {
        return degrees * pi<value_t>() / value_t(180);
    }

    template <typename value_t>
    inline constexpr value_t rad2deg(value_t radians)
    {
        return radians * value_t(180) / pi<value_t>();
    }

    template <typename value_t>
    inline constexpr value_t sin(value_t value)
    {
        return std::sin(value);
    }

    template <typename value_t>
    inline constexpr value_t cos(value_t value)
    {
        return std::cos(value);
    }

    template <typename value_t>
    inline constexpr value_t tan(value_t value)
    {
        return std::tan(value);
    }

    template <typename value_t>
    inline constexpr value_t asin(value_t value)
    {
        return std::asin(value);
    }

    template <typename value_t>
    inline constexpr value_t acos(value_t value)
    {
        return std::acos(value);
    }

    template <typename value_t>
    inline constexpr value_t atan(value_t value)
    {
        return std::atan(value);
    }

    template <typename value_t>
    inline constexpr value_t atan2(value_t y, value_t x)
    {
        return std::atan2(y, x);
    }

    template <typename value_t>
    inline constexpr value_t sinh(value_t value)
    {
        return std::sinh(value);
    }

    template <typename value_t>
    inline constexpr value_t cosh(value_t value)
    {
        return std::cosh(value);
    }

    template <typename value_t>
    inline constexpr value_t tanh(value_t value)
    {
        return std::tanh(value);
    }

    template <typename value_t>
    inline constexpr value_t asinh(value_t value)
    {
        return std::asinh(value);
    }

    template <typename value_t>
    inline constexpr value_t acosh(value_t value)
    {
        return std::acosh(value);
    }

    template <typename value_t>
    inline constexpr value_t atanh(value_t value)
    {
        return std::atanh(value);
    }
}  // namespace move::vectormath::scalar