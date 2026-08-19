#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Vec4.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    class Sphere3f
    {
    public:
        Sphere3f() noexcept = default;

        [[nodiscard]] static std::optional<Sphere3f> TryFromCenterRadius(
            const Point3f& center, float radius) noexcept
        {
            if (!std::isfinite(center.X()) || !std::isfinite(center.Y()) ||
                !std::isfinite(center.Z()) || !(radius >= 0.0F) ||
                !std::isfinite(radius))
            {
                return std::nullopt;
            }
            return Sphere3f(center, radius, UncheckedTag{});
        }

        [[nodiscard]] Point3f Center() const noexcept
        {
            return Point3f(Value_.X(), Value_.Y(), Value_.Z());
        }

        [[nodiscard]] float Radius() const noexcept
        {
            return Value_.W();
        }

        [[nodiscard]] float Diameter() const noexcept
        {
            return Radius() * 2.0F;
        }

        [[nodiscard]] bool Contains(const Point3f& point) const noexcept
        {
            const double x = static_cast<double>(point.X()) -
                             static_cast<double>(Value_.X());
            const double y = static_cast<double>(point.Y()) -
                             static_cast<double>(Value_.Y());
            const double z = static_cast<double>(point.Z()) -
                             static_cast<double>(Value_.Z());
            const double radius = static_cast<double>(Radius());
            return x * x + y * y + z * z <= radius * radius;
        }

        [[nodiscard]] friend bool operator==(const Sphere3f& left,
                                             const Sphere3f& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        Sphere3f(const Point3f& center, float radius, UncheckedTag) noexcept :
            Value_(center.X(), center.Y(), center.Z(), radius)
        {
        }

        Vec4f Value_{};
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Sphere3f) == 16);
static_assert(alignof(mv::math::Sphere3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Sphere3f>);
static_assert(std::is_standard_layout_v<mv::math::Sphere3f>);
