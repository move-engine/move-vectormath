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
    class Capsule3f
    {
    public:
        Capsule3f() noexcept = default;

        [[nodiscard]] static std::optional<Capsule3f> TryFromSegmentRadius(
            const Segment3f& centerLine, float radius) noexcept
        {
            if (!centerLine.IsFinite() || !(radius >= 0.0F) ||
                !std::isfinite(radius))
            {
                return std::nullopt;
            }
            return Capsule3f(centerLine, radius, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Capsule3f> TryFromEndpointsRadius(
            const Point3f& start, const Point3f& end, float radius) noexcept
        {
            return TryFromSegmentRadius(Segment3f(start, end), radius);
        }

        [[nodiscard]] Point3f Start() const noexcept
        {
            return Point3f(StartRadius_.X(), StartRadius_.Y(),
                           StartRadius_.Z());
        }

        [[nodiscard]] const Point3f& End() const noexcept
        {
            return End_;
        }

        [[nodiscard]] Segment3f CenterLine() const noexcept
        {
            return Segment3f(Start(), End_);
        }

        [[nodiscard]] float Radius() const noexcept
        {
            return StartRadius_.W();
        }

        [[nodiscard]] bool IsDegenerate() const noexcept
        {
            return Start() == End_;
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return CenterLine().IsFinite() && Radius() >= 0.0F &&
                   std::isfinite(Radius());
        }

        [[nodiscard]] friend bool operator==(const Capsule3f& left,
                                             const Capsule3f& right) noexcept
        {
            return left.StartRadius_ == right.StartRadius_ &&
                   left.End_ == right.End_;
        }

    private:
        struct UncheckedTag
        {
        };

        Capsule3f(const Segment3f& centerLine,
                  float radius,
                  UncheckedTag) noexcept :
            StartRadius_(centerLine.Start().X(),
                         centerLine.Start().Y(),
                         centerLine.Start().Z(),
                         radius),
            End_(centerLine.End())
        {
        }

        Vec4f StartRadius_{};
        Point3f End_{};
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Capsule3f) == 32);
static_assert(alignof(mv::math::Capsule3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Capsule3f>);
static_assert(std::is_standard_layout_v<mv::math::Capsule3f>);
