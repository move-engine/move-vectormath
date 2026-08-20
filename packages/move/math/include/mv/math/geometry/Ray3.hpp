#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Direction3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard half-line with a finite origin and unit direction; invariant
    // enforcement and semantic Point3f/Direction3f storage are Move-specific.
    template <typename T>
        requires std::is_floating_point_v<T>
    class Ray3
    {
    public:
        using Component = T;

        Ray3() noexcept = default;

        [[nodiscard]] static std::optional<Ray3> TryFromOriginDirection(
            const Point3<T>& origin, const Direction3<T>& direction) noexcept
        {
            if (!IsFinitePoint(origin))
            {
                return std::nullopt;
            }
            return Ray3(origin, direction, UncheckedTag{});
        }

        [[nodiscard]] const Point3<T>& Origin() const noexcept
        {
            return Origin_;
        }

        [[nodiscard]] const Direction3<T>& Direction() const noexcept
        {
            return Direction_;
        }

        // The caller supplies a distance in the ray domain [0, +infinity).
        // No implicit clamping is performed.
        [[nodiscard]] Point3<T> PointAt(T distance) const noexcept
        {
            return Origin_ + Direction_.Vector() * distance;
        }

        [[nodiscard]] friend bool operator==(const Ray3& left,
                                             const Ray3& right) noexcept
        {
            return left.Origin_ == right.Origin_ &&
                   left.Direction_ == right.Direction_;
        }

    private:
        struct UncheckedTag
        {
        };

        Ray3(const Point3<T>& origin,
             const Direction3<T>& direction,
             UncheckedTag) noexcept :
            Origin_(origin), Direction_(direction)
        {
        }

        [[nodiscard]] static bool IsFinitePoint(const Point3<T>& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        Point3<T> Origin_;
        Direction3<T> Direction_;
    };

    using Ray3f = Ray3<float>;
    using Ray3d = Ray3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Ray3f) == 32);
static_assert(alignof(mv::math::Ray3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Ray3f>);
static_assert(std::is_standard_layout_v<mv::math::Ray3f>);
static_assert(sizeof(mv::math::Ray3d) == 64);
static_assert(alignof(mv::math::Ray3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Ray3d>);
static_assert(std::is_standard_layout_v<mv::math::Ray3d>);
