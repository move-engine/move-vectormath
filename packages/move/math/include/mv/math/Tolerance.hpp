#pragma once

#include <algorithm>
#include <cmath>
#include <limits>
#include <type_traits>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    struct Tolerance
    {
        T Absolute;
        T Relative;
        T UnitLength;
        T Parallel;

        [[nodiscard]] static constexpr Tolerance Default() noexcept
        {
            return {std::numeric_limits<T>::epsilon() * T(4),
                    std::numeric_limits<T>::epsilon() * T(4),
                    std::numeric_limits<T>::epsilon() * T(16),
                    std::sqrt(std::numeric_limits<T>::epsilon())};
        }
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool IsNearlyEqual(
        T left,
        T right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        if (left == right)
        {
            return true;
        }
        if (!std::isfinite(left) || !std::isfinite(right))
        {
            return false;
        }

        // The combined relative/absolute bound follows PEP 485 (2015),
        // section 11; Move exposes both tolerances instead of hiding them.
        const T difference = std::abs(left - right);
        const T scale = std::max(std::abs(left), std::abs(right));
        return difference <=
               std::max(tolerance.Absolute, tolerance.Relative * scale);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] constexpr bool IsNearlyZero(
        T value,
        T absoluteTolerance = Tolerance<T>::Default().Absolute) noexcept
    {
        return std::isfinite(value) && std::abs(value) <= absoluteTolerance;
    }
}  // namespace mv::math
