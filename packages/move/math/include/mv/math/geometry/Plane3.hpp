#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard Hessian-normal plane form; finite construction and semantic
    // Normal3f storage are Move-specific.
    enum class PlaneSide
    {
        Back,
        Intersecting,
        Front
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    class alignas(sizeof(T) * 4U) Plane3
    {
    public:
        using Component = T;

        Plane3() noexcept = default;

        [[nodiscard]] static std::optional<Plane3> TryFromNormalAndOffset(
            const Normal3<T>& normal, T offset) noexcept
        {
            if (!std::isfinite(offset))
            {
                return std::nullopt;
            }
            return Plane3(normal, offset, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Plane3> TryFromPointNormal(
            const Point3<T>& point, const Normal3<T>& normal) noexcept
        {
            const T offset = -Dot(normal.Vector(), point.Vector());
            return TryFromNormalAndOffset(normal, offset);
        }

        [[nodiscard]] static std::optional<Plane3> TryFromPoints(
            const Point3<T>& first,
            const Point3<T>& second,
            const Point3<T>& third) noexcept
        {
            const auto normal =
                Normal3<T>::TryFrom(Cross(second - first, third - first));
            if (!normal)
            {
                return std::nullopt;
            }
            return TryFromPointNormal(first, *normal);
        }

        [[nodiscard]] Normal3<T> Normal() const noexcept
        {
            return Normal3<T>(
                Vec3<T>(Coefficients_[0], Coefficients_[1], Coefficients_[2]),
                typename Normal3<T>::UncheckedTag{});
        }

        [[nodiscard]] T Offset() const noexcept
        {
            return Coefficients_[3];
        }

        [[nodiscard]] T SignedDistance(const Point3<T>& point) const noexcept
        {
            return Dot(Normal().Vector(), point.Vector()) + Offset();
        }

        [[nodiscard]] PlaneSide Classify(
            const Point3<T>& point, T boundaryTolerance = T(0)) const noexcept
        {
            const T distance = SignedDistance(point);
            const T tolerance = std::abs(boundaryTolerance);
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

        [[nodiscard]] Point3<T> ClosestPoint(
            const Point3<T>& point) const noexcept
        {
            return point - Normal().Vector() * SignedDistance(point);
        }

        [[nodiscard]] Plane3 Flipped() const noexcept
        {
            return Plane3(-Normal(), -Offset(), UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Plane3& left,
                                             const Plane3& right) noexcept
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

        Plane3(const Normal3<T>& normal, T offset, UncheckedTag) noexcept :
            Coefficients_{normal.Vector().X(), normal.Vector().Y(),
                          normal.Vector().Z(), offset}
        {
        }

        T Coefficients_[4]{T(1), T(0), T(0), T(0)};
    };

    using Plane3f = Plane3<float>;
    using Plane3d = Plane3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Plane3f) == 16);
static_assert(alignof(mv::math::Plane3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Plane3f>);
static_assert(std::is_standard_layout_v<mv::math::Plane3f>);
static_assert(sizeof(mv::math::Plane3d) == 32);
static_assert(alignof(mv::math::Plane3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Plane3d>);
static_assert(std::is_standard_layout_v<mv::math::Plane3d>);
