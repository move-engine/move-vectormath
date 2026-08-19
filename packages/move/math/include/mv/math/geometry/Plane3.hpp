#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Provenance: MVM-PROV-C-PRIMITIVES.
    enum class PlaneSide
    {
        Back,
        Intersecting,
        Front
    };

    class alignas(16) Plane3f
    {
    public:
        Plane3f() noexcept = default;

        [[nodiscard]] static std::optional<Plane3f> TryFromNormalAndOffset(
            Normal3f normal, float offset) noexcept
        {
            if (!std::isfinite(offset))
            {
                return std::nullopt;
            }
            return Plane3f(normal, offset, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Plane3f> TryFromPointNormal(
            const Point3f& point, Normal3f normal) noexcept
        {
            const float offset = -Dot(normal.Vector(), point.Vector());
            return TryFromNormalAndOffset(normal, offset);
        }

        [[nodiscard]] static std::optional<Plane3f> TryFromPoints(
            const Point3f& first,
            const Point3f& second,
            const Point3f& third) noexcept
        {
            const auto normal =
                Normal3f::TryFrom(Cross(second - first, third - first));
            if (!normal)
            {
                return std::nullopt;
            }
            return TryFromPointNormal(first, *normal);
        }

        [[nodiscard]] Normal3f Normal() const noexcept
        {
            return Normal3f(
                Vec3f(Coefficients_[0], Coefficients_[1], Coefficients_[2]),
                Normal3f::UncheckedTag{});
        }

        [[nodiscard]] float Offset() const noexcept
        {
            return Coefficients_[3];
        }

        [[nodiscard]] float SignedDistance(const Point3f& point) const noexcept
        {
            return Dot(Normal().Vector(), point.Vector()) + Offset();
        }

        [[nodiscard]] PlaneSide Classify(
            const Point3f& point, float boundaryTolerance = 0.0F) const noexcept
        {
            const float distance = SignedDistance(point);
            const float tolerance = std::abs(boundaryTolerance);
            if (distance > tolerance)
            {
                return PlaneSide::Front;
            }
            if (distance < -tolerance)
            {
                return PlaneSide::Back;
            }
            return PlaneSide::Intersecting;
        }

        [[nodiscard]] Point3f ClosestPoint(const Point3f& point) const noexcept
        {
            return point - Normal().Vector() * SignedDistance(point);
        }

        [[nodiscard]] Plane3f Flipped() const noexcept
        {
            return Plane3f(-Normal(), -Offset(), UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Plane3f& left,
                                             const Plane3f& right) noexcept
        {
            return left.Coefficients_[0] == right.Coefficients_[0] &&
                   left.Coefficients_[1] == right.Coefficients_[1] &&
                   left.Coefficients_[2] == right.Coefficients_[2] &&
                   left.Coefficients_[3] == right.Coefficients_[3];
        }

    private:
        struct UncheckedTag
        {
        };

        Plane3f(Normal3f normal, float offset, UncheckedTag) noexcept :
            Coefficients_{normal.Vector().X(), normal.Vector().Y(),
                          normal.Vector().Z(), offset}
        {
        }

        float Coefficients_[4]{1.0F, 0.0F, 0.0F, 0.0F};
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Plane3f) == 16);
static_assert(alignof(mv::math::Plane3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Plane3f>);
static_assert(std::is_standard_layout_v<mv::math::Plane3f>);
