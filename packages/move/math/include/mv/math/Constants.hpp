#pragma once

#include <concepts>
#include <numbers>

namespace mv::math
{
    template <std::floating_point T>
    inline constexpr T Pi = std::numbers::pi_v<T>;

    template <std::floating_point T>
    inline constexpr T HalfPi = Pi<T> / T(2);

    template <std::floating_point T>
    inline constexpr T QuarterPi = Pi<T> / T(4);

    template <std::floating_point T>
    inline constexpr T Tau = Pi<T> * T(2);
}  // namespace mv::math
