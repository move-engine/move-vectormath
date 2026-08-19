#pragma once

#include <cmath>
#include <type_traits>

#include <mv/math/semantic/Direction3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard infinite line with a unit direction; the signed-distance
    // parameter convention matches Geometric Tools Line.h (BSL-1.0).
    class Line3f
    {
    public:
        Line3f() noexcept = default;

        Line3f(Point3f origin, Direction3f direction) noexcept :
            Origin_(origin), Direction_(direction)
        {
        }

        [[nodiscard]] const Point3f& Origin() const noexcept
        {
            return Origin_;
        }

        [[nodiscard]] const Direction3f& Direction() const noexcept
        {
            return Direction_;
        }

        [[nodiscard]] Point3f PointAt(float signedDistance) const noexcept
        {
            return Origin_ + Direction_.Vector() * signedDistance;
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return std::isfinite(Origin_.X()) && std::isfinite(Origin_.Y()) &&
                   std::isfinite(Origin_.Z());
        }

        [[nodiscard]] friend bool operator==(const Line3f& left,
                                             const Line3f& right) noexcept
        {
            return left.Origin_ == right.Origin_ &&
                   left.Direction_ == right.Direction_;
        }

    private:
        Point3f Origin_;
        Direction3f Direction_;
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Line3f) == 32);
static_assert(alignof(mv::math::Line3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Line3f>);
static_assert(std::is_standard_layout_v<mv::math::Line3f>);
