#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Vec4.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Move-owned sphere contract; operations cross-checked against DirectXMath
    // DirectXCollision.inl (MIT).
    template <typename T>
        requires std::is_floating_point_v<T>
    class Sphere3
    {
    public:
        using Component = T;

        Sphere3() noexcept = default;

        [[nodiscard]] static std::optional<Sphere3> TryFromCenterRadius(
            const Point3<T>& center, T radius) noexcept
        {
            if (!std::isfinite(center.X()) || !std::isfinite(center.Y()) ||
                !std::isfinite(center.Z()) || !(radius >= T(0)) ||
                !std::isfinite(radius))
            {
                return std::nullopt;
            }
            return Sphere3(center, radius, UncheckedTag{});
        }

        [[nodiscard]] Point3<T> Center() const noexcept
        {
            return Point3<T>(Value_.X(), Value_.Y(), Value_.Z());
        }

        [[nodiscard]] T Radius() const noexcept
        {
            return Value_.W();
        }

        [[nodiscard]] T Diameter() const noexcept
        {
            return Radius() * T(2);
        }

        [[nodiscard]] bool Contains(const Point3<T>& point) const noexcept
        {
            using Calculation =
                std::conditional_t<(sizeof(T) < sizeof(double)), double, T>;
            const Calculation x = static_cast<Calculation>(point.X()) -
                                  static_cast<Calculation>(Value_.X());
            const Calculation y = static_cast<Calculation>(point.Y()) -
                                  static_cast<Calculation>(Value_.Y());
            const Calculation z = static_cast<Calculation>(point.Z()) -
                                  static_cast<Calculation>(Value_.Z());
            const Calculation radius = static_cast<Calculation>(Radius());
            return x * x + y * y + z * z <= radius * radius;
        }

        [[nodiscard]] friend bool operator==(const Sphere3& left,
                                             const Sphere3& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        Sphere3(const Point3<T>& center, T radius, UncheckedTag) noexcept :
            Value_(center.X(), center.Y(), center.Z(), radius)
        {
        }

        Vec4<T> Value_{};
    };

    using Sphere3f = Sphere3<float>;
    using Sphere3d = Sphere3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Sphere3f) == 16);
static_assert(alignof(mv::math::Sphere3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Sphere3f>);
static_assert(std::is_standard_layout_v<mv::math::Sphere3f>);
static_assert(sizeof(mv::math::Sphere3d) == 32);
static_assert(alignof(mv::math::Sphere3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Sphere3d>);
static_assert(std::is_standard_layout_v<mv::math::Sphere3d>);
