#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Direction3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard endpoint segment; the [0,1] fraction convention matches
    // Geometric Tools Segment.h and DistPointSegment.h (BSL-1.0).
    template <typename T>
        requires std::is_floating_point_v<T>
    class Segment3
    {
    public:
        using Component = T;

        Segment3() noexcept = default;

        Segment3(const Point3<T>& start, const Point3<T>& end) noexcept :
            Start_(start), End_(end)
        {
        }

        [[nodiscard]] const Point3<T>& Start() const noexcept
        {
            return Start_;
        }

        [[nodiscard]] const Point3<T>& End() const noexcept
        {
            return End_;
        }

        [[nodiscard]] Vec3<T> Displacement() const noexcept
        {
            return End_ - Start_;
        }

        [[nodiscard]] T LengthSquared() const noexcept
        {
            return mv::math::LengthSquared(Displacement());
        }

        [[nodiscard]] T Length() const noexcept
        {
            return std::sqrt(LengthSquared());
        }

        [[nodiscard]] bool IsDegenerate() const noexcept
        {
            return Start_ == End_;
        }

        [[nodiscard]] std::optional<Direction3<T>> TryDirection() const noexcept
        {
            return Direction3<T>::TryFrom(Displacement());
        }

        // The caller supplies a fraction in [0,1]. No implicit clamping is
        // performed so extrapolation is visible rather than silently repaired.
        [[nodiscard]] Point3<T> PointAtFraction(T fraction) const noexcept
        {
            return Start_ + Displacement() * fraction;
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return IsFinitePoint(Start_) && IsFinitePoint(End_);
        }

        [[nodiscard]] friend bool operator==(const Segment3& left,
                                             const Segment3& right) noexcept
        {
            return left.Start_ == right.Start_ && left.End_ == right.End_;
        }

    private:
        [[nodiscard]] static bool IsFinitePoint(const Point3<T>& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        Point3<T> Start_;
        Point3<T> End_;
    };

    using Segment3f = Segment3<float>;
    using Segment3d = Segment3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Segment3f) == 32);
static_assert(alignof(mv::math::Segment3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Segment3f>);
static_assert(std::is_standard_layout_v<mv::math::Segment3f>);
static_assert(sizeof(mv::math::Segment3d) == 64);
static_assert(alignof(mv::math::Segment3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Segment3d>);
static_assert(std::is_standard_layout_v<mv::math::Segment3d>);
