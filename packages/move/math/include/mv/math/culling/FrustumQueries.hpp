#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <type_traits>

#include <mv/math/culling/Frustum3.hpp>
#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Obb3.hpp>
#include <mv/math/geometry/Sphere3.hpp>

namespace mv::math
{
    enum class Containment : std::uint8_t
    {
        Outside,
        Intersecting,
        Inside
    };

    struct CullingResult
    {
        Containment Classification;
        FrustumPlaneMask RemainingPlanes;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    class PreparedFrustum3
    {
    public:
        explicit PreparedFrustum3(const Frustum3<T>& frustum) noexcept :
            Frustum_(frustum)
        {
            for (std::size_t index = 0U; index < 6U; ++index)
            {
                const FrustumPlane plane = static_cast<FrustumPlane>(index);
                if (!frustum.IsActive(plane))
                {
                    continue;
                }
                const Vec3<T> normal = frustum.Plane(plane).Normal().Vector();
                AbsoluteNormals_[index] =
                    Vec3<T>(std::abs(normal.X()), std::abs(normal.Y()),
                            std::abs(normal.Z()));
            }
        }

        [[nodiscard]] const Frustum3<T>& Frustum() const noexcept
        {
            return Frustum_;
        }

        [[nodiscard]] const Vec3<T>& AbsoluteNormal(
            FrustumPlane plane) const noexcept
        {
            return AbsoluteNormals_[static_cast<std::size_t>(plane)];
        }

    private:
        Frustum3<T> Frustum_;
        std::array<Vec3<T>, 6> AbsoluteNormals_{};
    };

    using PreparedFrustum3f = PreparedFrustum3<float>;
    using PreparedFrustum3d = PreparedFrustum3<double>;

    namespace detail
    {
        template <typename T, typename RadiusForPlane>
        [[nodiscard]] inline CullingResult ClassifyAgainstFrustum(
            const Frustum3<T>& frustum,
            const Point3<T>& center,
            FrustumPlaneMask requestedPlanes,
            T tolerance,
            RadiusForPlane&& radiusForPlane) noexcept
        {
            FrustumPlaneMask remaining = FrustumPlaneMask::None;
            const FrustumPlaneMask active =
                requestedPlanes & frustum.ActivePlanes();
            for (std::size_t index = 0U; index < 6U; ++index)
            {
                const FrustumPlane plane = static_cast<FrustumPlane>(index);
                if (!HasPlane(active, plane))
                {
                    continue;
                }
                const T radius = radiusForPlane(plane);
                const T distance = frustum.Plane(plane).SignedDistance(center);
                if (distance < -radius - tolerance)
                {
                    return {Containment::Outside, FrustumPlaneMask::None};
                }
                if (distance <= radius + tolerance)
                {
                    remaining |= PlaneMask(plane);
                }
            }
            return {remaining == FrustumPlaneMask::None
                        ? Containment::Inside
                        : Containment::Intersecting,
                    remaining};
        }
    }  // namespace detail

    template <typename T>
    [[nodiscard]] inline CullingResult Classify(
        const PreparedFrustum3<T>& prepared,
        const Point3<T>& point,
        FrustumPlaneMask active = FrustumPlaneMask::All,
        T tolerance = T(0)) noexcept
    {
        return detail::ClassifyAgainstFrustum(prepared.Frustum(), point, active,
                                              std::abs(tolerance),
                                              [](FrustumPlane) noexcept
                                              {
                                                  return T(0);
                                              });
    }

    template <typename T>
    [[nodiscard]] inline CullingResult Classify(
        const PreparedFrustum3<T>& prepared,
        const Sphere3<T>& sphere,
        FrustumPlaneMask active = FrustumPlaneMask::All,
        T tolerance = T(0)) noexcept
    {
        return detail::ClassifyAgainstFrustum(
            prepared.Frustum(), sphere.Center(), active, std::abs(tolerance),
            [&sphere](FrustumPlane) noexcept
            {
                return sphere.Radius();
            });
    }

    template <typename T>
    [[nodiscard]] inline CullingResult Classify(
        const PreparedFrustum3<T>& prepared,
        const Aabb3<T>& box,
        FrustumPlaneMask active = FrustumPlaneMask::All,
        T tolerance = T(0)) noexcept
    {
        const auto center = box.TryCenter();
        const auto halfExtents = box.TryHalfExtents();
        if (!center || !halfExtents)
        {
            return {Containment::Outside, FrustumPlaneMask::None};
        }
        return detail::ClassifyAgainstFrustum(
            prepared.Frustum(), *center, active, std::abs(tolerance),
            [&prepared, &halfExtents](FrustumPlane plane) noexcept
            {
                return Dot(prepared.AbsoluteNormal(plane), *halfExtents);
            });
    }

    template <typename T>
    [[nodiscard]] inline CullingResult Classify(
        const PreparedFrustum3<T>& prepared,
        const Obb3<T>& box,
        FrustumPlaneMask active = FrustumPlaneMask::All,
        T tolerance = T(0)) noexcept
    {
        const std::array<Vec3<T>, 3> axes = {
            box.AxisX().Vector(), box.AxisY().Vector(), box.AxisZ().Vector()};
        return detail::ClassifyAgainstFrustum(
            prepared.Frustum(), box.Center(), active, std::abs(tolerance),
            [&prepared, &box, &axes](FrustumPlane plane) noexcept
            {
                const Vec3<T> normal =
                    prepared.Frustum().Plane(plane).Normal().Vector();
                return box.HalfExtents().X() * std::abs(Dot(normal, axes[0])) +
                       box.HalfExtents().Y() * std::abs(Dot(normal, axes[1])) +
                       box.HalfExtents().Z() * std::abs(Dot(normal, axes[2]));
            });
    }

    template <typename T, typename Bounds>
    [[nodiscard]] inline Containment Classify(const Frustum3<T>& frustum,
                                              const Bounds& bounds,
                                              T tolerance = T(0)) noexcept
    {
        return Classify(PreparedFrustum3<T>(frustum), bounds,
                        FrustumPlaneMask::All, tolerance)
            .Classification;
    }
}  // namespace mv::math
