#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

#include <mv/math/Vec3.hpp>

namespace mv::math::detail
{
    // Safe scaling follows Blue (ACM TOMS 1978, doi:10.1145/355769.355771),
    // specialized here for fixed 3-component vectors.
    [[nodiscard]] inline std::optional<Vec3f> TryNormalizeVector(
        const Vec3f& value) noexcept
    {
        const float maximum = std::max(
            {std::abs(value.X()), std::abs(value.Y()), std::abs(value.Z())});
        if (!(maximum > 0.0F) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }

        const Vec3f scaled = value * (1.0F / maximum);
        const float scaledLengthSquared = LengthSquared(scaled);
        if (!(scaledLengthSquared > 0.0F) ||
            !std::isfinite(scaledLengthSquared))
        {
            return std::nullopt;
        }

        return scaled * (1.0F / std::sqrt(scaledLengthSquared));
    }
}  // namespace mv::math::detail
