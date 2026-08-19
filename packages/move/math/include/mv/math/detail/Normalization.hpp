#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

#include <mv/math/Vec3.hpp>

namespace mv::math::detail
{
    // Provenance: MVM-PROV-A-SAFE-NORMALIZE.
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
