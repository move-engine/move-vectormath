#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Vec4.hpp>
#include <mv/math/geometry/Segment3.hpp>

namespace mv::math
{
    // A capsule is the sphere-swept segment described by Ericson, Real-Time
    // Collision Detection (2005), section 4.5. Start+radius share one SIMD
    // lane group so the semantic value stays 32 bytes rather than 48.
    template <typename T>
        requires std::is_floating_point_v<T>
    class Capsule3
    {
    public:
        using Component = T;

        Capsule3() noexcept = default;

        [[nodiscard]] static std::optional<Capsule3> TryFromSegmentRadius(
            const Segment3<T>& centerLine, T radius) noexcept
        {
            if (!centerLine.IsFinite() || !(radius >= T(0)) ||
                !std::isfinite(radius))
            {
                return std::nullopt;
            }
            return Capsule3(centerLine, radius, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Capsule3> TryFromEndpointsRadius(
            const Point3<T>& start, const Point3<T>& end, T radius) noexcept
        {
            return TryFromSegmentRadius(Segment3<T>(start, end), radius);
        }

        [[nodiscard]] Point3<T> Start() const noexcept
        {
            return Point3<T>(StartRadius_.X(), StartRadius_.Y(),
                             StartRadius_.Z());
        }

        [[nodiscard]] const Point3<T>& End() const noexcept
        {
            return End_;
        }

        [[nodiscard]] Segment3<T> CenterLine() const noexcept
        {
            return Segment3<T>(Start(), End_);
        }

        [[nodiscard]] T Radius() const noexcept
        {
            return StartRadius_.W();
        }

        [[nodiscard]] bool IsDegenerate() const noexcept
        {
            return Start() == End_;
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return CenterLine().IsFinite() && Radius() >= T(0) &&
                   std::isfinite(Radius());
        }

        [[nodiscard]] friend bool operator==(const Capsule3& left,
                                             const Capsule3& right) noexcept
        {
            return left.StartRadius_ == right.StartRadius_ &&
                   left.End_ == right.End_;
        }

    private:
        struct UncheckedTag
        {
        };

        Capsule3(const Segment3<T>& centerLine, T radius, UncheckedTag) noexcept
            :
            StartRadius_(centerLine.Start().X(),
                         centerLine.Start().Y(),
                         centerLine.Start().Z(),
                         radius),
            End_(centerLine.End())
        {
        }

        Vec4<T> StartRadius_{};
        Point3<T> End_{};
    };

    using Capsule3f = Capsule3<float>;
    using Capsule3d = Capsule3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Capsule3f) == 32);
static_assert(alignof(mv::math::Capsule3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Capsule3f>);
static_assert(std::is_standard_layout_v<mv::math::Capsule3f>);
static_assert(sizeof(mv::math::Capsule3d) == 64);
static_assert(alignof(mv::math::Capsule3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Capsule3d>);
static_assert(std::is_standard_layout_v<mv::math::Capsule3d>);
