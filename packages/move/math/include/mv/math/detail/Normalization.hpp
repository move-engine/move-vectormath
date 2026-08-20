#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

#include <mv/math/Vec3.hpp>

namespace mv::math::detail
{
    // Safe scaling follows Blue (ACM TOMS 1978, doi:10.1145/355769.355771),
    // specialized here for fixed 3-component vectors.
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Vec3<T>> TryNormalizeVector(
        const Vec3<T>& value) noexcept
    {
        // Use the direct RTM-style normalization path when the squared length
        // is representable. RTM 2.3.1's vector_normalize3 implementation
        // (MIT) uses this reciprocal-square-root form.
        const T lengthSquared = LengthSquared(value);
        if (lengthSquared > T(0) && std::isfinite(lengthSquared))
        {
            return value * (T(1) / std::sqrt(lengthSquared));
        }

        // Blue's scaling is the fallback for finite vectors whose direct
        // squared length underflows to zero or overflows to infinity.
        const T maximum = std::max(
            {std::abs(value.X()), std::abs(value.Y()), std::abs(value.Z())});
        if (!(maximum > T(0)) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }

        const Vec3<T> scaled = value * (T(1) / maximum);
        const T scaledLengthSquared = LengthSquared(scaled);
        if (!(scaledLengthSquared > T(0)) ||
            !std::isfinite(scaledLengthSquared))
        {
            return std::nullopt;
        }

        return scaled * (T(1) / std::sqrt(scaledLengthSquared));
    }
}  // namespace mv::math::detail
