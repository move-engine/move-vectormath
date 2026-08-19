#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/Rotation3.hpp>
#include <mv/math/detail/Normalization.hpp>

namespace mv::math
{
    class Plane3f;

    class Normal3f
    {
    public:
        Normal3f() noexcept : Value_(1.0F, 0.0F, 0.0F)
        {
        }

        [[nodiscard]] static std::optional<Normal3f> TryFrom(
            const Vec3f& value) noexcept
        {
            const auto normalized = detail::TryNormalizeVector(value);
            if (!normalized)
            {
                return std::nullopt;
            }
            return Normal3f(*normalized, UncheckedTag{});
        }

        [[nodiscard]] static Normal3f FromOr(const Vec3f& value,
                                             Normal3f fallback) noexcept
        {
            const auto result = TryFrom(value);
            return result ? *result : fallback;
        }

        [[nodiscard]] static Normal3f FromDirection(
            Direction3f direction) noexcept
        {
            return Normal3f(direction.Vector(), UncheckedTag{});
        }

        [[nodiscard]] static Normal3f AxisX() noexcept
        {
            return Normal3f(Vec3f(1.0F, 0.0F, 0.0F), UncheckedTag{});
        }

        [[nodiscard]] static Normal3f AxisY() noexcept
        {
            return Normal3f(Vec3f(0.0F, 1.0F, 0.0F), UncheckedTag{});
        }

        [[nodiscard]] static Normal3f AxisZ() noexcept
        {
            return Normal3f(Vec3f(0.0F, 0.0F, 1.0F), UncheckedTag{});
        }

        [[nodiscard]] const Vec3f& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] Direction3f ToDirection() const noexcept
        {
            return Direction3f(Value_, Direction3f::UncheckedTag{});
        }

        [[nodiscard]] Normal3f operator-() const noexcept
        {
            return Normal3f(Value_ * -1.0F, UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Normal3f& left,
                                             const Normal3f& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        explicit Normal3f(Vec3f value, UncheckedTag) noexcept : Value_(value)
        {
        }

        Vec3f Value_;

        friend class Plane3f;
        friend Normal3f Rotate(const Rotation3f&, Normal3f) noexcept;
    };

    [[nodiscard]] inline Normal3f Rotate(const Rotation3f& rotation,
                                         Normal3f normal) noexcept
    {
        return Normal3f(Rotate(rotation, normal.Vector()),
                        Normal3f::UncheckedTag{});
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Normal3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Normal3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Normal3f>);
static_assert(std::is_standard_layout_v<mv::math::Normal3f>);
