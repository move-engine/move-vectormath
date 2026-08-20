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
    class Ray3f
    {
    public:
        Ray3f() noexcept = default;

        [[nodiscard]] static std::optional<Ray3f> TryFromOriginDirection(
            Point3f origin, Direction3f direction) noexcept
        {
            if (!IsFinitePoint(origin))
            {
                return std::nullopt;
            }
            return Ray3f(origin, direction, UncheckedTag{});
        }

        [[nodiscard]] const Point3f& Origin() const noexcept
        {
            return Origin_;
        }

        [[nodiscard]] const Direction3f& Direction() const noexcept
        {
            return Direction_;
        }

        // The caller supplies a distance in the ray domain [0, +infinity).
        // No implicit clamping is performed.
        [[nodiscard]] Point3f PointAt(float distance) const noexcept
        {
            return Origin_ + Direction_.Vector() * distance;
        }

        [[nodiscard]] friend bool operator==(const Ray3f& left,
                                             const Ray3f& right) noexcept
        {
            return left.Origin_ == right.Origin_ &&
                   left.Direction_ == right.Direction_;
        }

    private:
        struct UncheckedTag
        {
        };

        Ray3f(Point3f origin, Direction3f direction, UncheckedTag) noexcept :
            Origin_(origin), Direction_(direction)
        {
        }

        [[nodiscard]] static bool IsFinitePoint(const Point3f& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        Point3f Origin_;
        Direction3f Direction_;
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Ray3f) == 32);
static_assert(alignof(mv::math::Ray3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Ray3f>);
static_assert(std::is_standard_layout_v<mv::math::Ray3f>);
