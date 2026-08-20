#pragma once

#include <algorithm>
#include <cmath>
#include <concepts>
#include <optional>
#include <type_traits>

#include <mv/math/Constants.hpp>

namespace mv::math
{
    template <typename T>
    concept Arithmetic = std::is_arithmetic_v<T>;

    template <Arithmetic T>
    [[nodiscard]] constexpr T Saturate(T value) noexcept
    {
        return std::clamp(value, T(0), T(1));
    }

    template <std::floating_point T>
    [[nodiscard]] inline std::optional<T> TryInverseLerp(T from,
                                                         T to,
                                                         T value) noexcept
    {
        // Algebraic inverse of linear interpolation. The scaled fallback uses
        // the overflow-avoidance principle described by David Goldberg,
        // "What Every Computer Scientist Should Know About Floating-Point
        // Arithmetic" (ACM CSUR 1991, doi:10.1145/103162.103163).
        if (!std::isfinite(from) || !std::isfinite(to) ||
            !std::isfinite(value) || from == to)
        {
            return std::nullopt;
        }

        const T difference = to - from;
        const T offset = value - from;
        if (difference != T(0) && std::isfinite(difference) &&
            std::isfinite(offset))
        {
            const T result = offset / difference;
            return std::isfinite(result) ? std::optional<T>(result)
                                         : std::nullopt;
        }

        // Scaling preserves the affine ratio while avoiding overflow for
        // finite values near the limits of the floating-point type.
        const T scale =
            std::max({std::abs(from), std::abs(to), std::abs(value)});
        if (!(scale > T(0)) || !std::isfinite(scale))
        {
            return std::nullopt;
        }

        const T scaledFrom = from / scale;
        const T scaledDifference = to / scale - scaledFrom;
        if (scaledDifference == T(0))
        {
            return std::nullopt;
        }

        const T result = (value / scale - scaledFrom) / scaledDifference;
        return std::isfinite(result) ? std::optional<T>(result) : std::nullopt;
    }

    template <std::floating_point T>
    [[nodiscard]] inline std::optional<T> TryInverseLerpClamped(
        T from, T to, T value) noexcept
    {
        const auto result = TryInverseLerp(from, to, value);
        return result ? std::optional<T>(Saturate(*result)) : std::nullopt;
    }

    template <std::floating_point T>
    [[nodiscard]] constexpr T SmoothStep(T amount) noexcept
    {
        // Cubic Hermite smoothstep as specified by GLSL 4.60, section 8.3.
        const T t = Saturate(amount);
        return t * t * (T(3) - T(2) * t);
    }

    template <std::floating_point T>
    [[nodiscard]] constexpr T SmootherStep(T amount) noexcept
    {
        // Quintic fade polynomial from Ken Perlin, "Improving Noise",
        // SIGGRAPH 2002, doi:10.1145/566570.566636.
        const T t = Saturate(amount);
        return t * t * t * (t * (t * T(6) - T(15)) + T(10));
    }
}  // namespace mv::math
