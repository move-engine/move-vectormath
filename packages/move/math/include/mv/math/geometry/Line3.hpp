#pragma once

#include <cmath>
#include <type_traits>

#include <mv/math/semantic/Direction3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard infinite line with a unit direction; the signed-distance
    // parameter convention matches Geometric Tools Line.h (BSL-1.0).
    template <typename T>
        requires std::is_floating_point_v<T>
    class Line3
    {
    public:
        using Component = T;

        Line3() noexcept = default;

        Line3(const Point3<T>& origin, const Direction3<T>& direction) noexcept
            :
            Origin_(origin), Direction_(direction)
        {
        }

        [[nodiscard]] const Point3<T>& Origin() const noexcept
        {
            return Origin_;
        }

        [[nodiscard]] const Direction3<T>& Direction() const noexcept
        {
            return Direction_;
        }

        [[nodiscard]] Point3<T> PointAt(T signedDistance) const noexcept
        {
            return Origin_ + Direction_.Vector() * signedDistance;
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return std::isfinite(Origin_.X()) && std::isfinite(Origin_.Y()) &&
                   std::isfinite(Origin_.Z());
        }

        [[nodiscard]] friend bool operator==(const Line3& left,
                                             const Line3& right) noexcept
        {
            return left.Origin_ == right.Origin_ &&
                   left.Direction_ == right.Direction_;
        }

    private:
        Point3<T> Origin_;
        Direction3<T> Direction_;
    };

    using Line3f = Line3<float>;
    using Line3d = Line3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Line3f) == 32);
static_assert(alignof(mv::math::Line3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Line3f>);
static_assert(std::is_standard_layout_v<mv::math::Line3f>);
static_assert(sizeof(mv::math::Line3d) == 64);
static_assert(alignof(mv::math::Line3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Line3d>);
static_assert(std::is_standard_layout_v<mv::math::Line3d>);
