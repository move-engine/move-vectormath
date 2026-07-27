#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/detail/Normalization.hpp>

namespace mv::math
{
    class Rotation3f;
    class Normal3f;

    class Direction3f
    {
    public:
        Direction3f() noexcept : Value_(1.0F, 0.0F, 0.0F)
        {
        }

        [[nodiscard]] static std::optional<Direction3f> TryFrom(
            const Vec3f& value) noexcept
        {
            const auto normalized = detail::TryNormalizeVector(value);
            if (!normalized)
            {
                return std::nullopt;
            }
            return Direction3f(*normalized, UncheckedTag{});
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

        friend class Normal3f;
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
