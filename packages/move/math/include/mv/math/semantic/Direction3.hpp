#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Vec3.hpp>

namespace mv::math
{
    class Rotation3f;

    class Direction3f
    {
    public:
        Direction3f() noexcept : Value_(1.0F, 0.0F, 0.0F)
        {
        }

        [[nodiscard]] static std::optional<Direction3f> TryFrom(
            const Vec3f& value) noexcept
        {
            const float maximum =
                std::max({std::abs(value.X()), std::abs(value.Y()),
                          std::abs(value.Z())});
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

            return Direction3f(scaled * (1.0F / std::sqrt(scaledLengthSquared)),
                               UncheckedTag{});
        }

        [[nodiscard]] static Direction3f FromOr(const Vec3f& value,
                                                Direction3f fallback) noexcept
        {
            const auto result = TryFrom(value);
            return result ? *result : fallback;
        }

        [[nodiscard]] static Direction3f AxisX() noexcept
        {
            return Direction3f(Vec3f(1.0F, 0.0F, 0.0F), UncheckedTag{});
        }

        [[nodiscard]] static Direction3f AxisY() noexcept
        {
            return Direction3f(Vec3f(0.0F, 1.0F, 0.0F), UncheckedTag{});
        }

        [[nodiscard]] static Direction3f AxisZ() noexcept
        {
            return Direction3f(Vec3f(0.0F, 0.0F, 1.0F), UncheckedTag{});
        }

        [[nodiscard]] static Direction3f Forward() noexcept
        {
            return AxisZ();
        }

        [[nodiscard]] const Vec3f& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] Direction3f operator-() const noexcept
        {
            return Direction3f(Value_ * -1.0F, UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Direction3f& left,
                                             const Direction3f& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        explicit Direction3f(Vec3f value, UncheckedTag) noexcept : Value_(value)
        {
        }

        Vec3f Value_;

        friend class Rotation3f;
        friend Direction3f Rotate(const Rotation3f&, Direction3f) noexcept;
    };

    [[nodiscard]] inline std::optional<Direction3f> TryNormalize(
        const Vec3f& value) noexcept
    {
        return Direction3f::TryFrom(value);
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Direction3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Direction3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Direction3f>);
static_assert(std::is_standard_layout_v<mv::math::Direction3f>);
