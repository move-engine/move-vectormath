#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>

#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/geometry/Sphere3.hpp>
#include <mv/math/queries/PreparedRay3.hpp>
#include <mv/math/queries/QueryTypes.hpp>

namespace mv::math
{
    namespace detail
    {
        [[nodiscard]] inline float Component(const Vec3f& value,
                                             std::size_t axis) noexcept
        {
            if (axis == 0U)
            {
                return value.X();
            }
            return axis == 1U ? value.Y() : value.Z();
        }

        [[nodiscard]] inline float Component(const Point3f& value,
                                             std::size_t axis) noexcept
        {
            if (axis == 0U)
            {
                return value.X();
            }
            return axis == 1U ? value.Y() : value.Z();
        }

        [[nodiscard]] inline Normal3f AxisNormal(std::size_t axis,
                                                 float sign) noexcept
        {
            const Normal3f normal = axis == 0U   ? Normal3f::AxisX()
                                    : axis == 1U ? Normal3f::AxisY()
                                                 : Normal3f::AxisZ();
            return sign < 0.0F ? -normal : normal;
        }

        [[nodiscard]] inline bool IsStrictlyInside(
            const Aabb3f& box, const Point3f& point) noexcept
        {
            return point.X() > box.Minimum().X() &&
                   point.Y() > box.Minimum().Y() &&
                   point.Z() > box.Minimum().Z() &&
                   point.X() < box.Maximum().X() &&
                   point.Y() < box.Maximum().Y() &&
                   point.Z() < box.Maximum().Z();
        }

        struct RayAabbSlabSolution
        {
            float EntryDistance;
            float ExitDistance;
            bool StartsInside;
            std::size_t EntryAxis;
            std::size_t ExitAxis;
            float EntrySign;
            float ExitSign;
            bool HasEntryAxis;
        };

        // Williams et al. ray/box slab test (JGT 2005); Move explicitly handles
        // parallel axes and returns a clipped interval with surface normals.
        [[nodiscard]] inline std::optional<RayAabbSlabSolution>
        TryIntersectPreparedAabbSlabs(const PreparedRay3f& prepared,
                                      const Aabb3f& box) noexcept
        {
            const Ray3f& ray = prepared.Ray();
            if (!ray.IsFinite() || box.IsEmpty())
            {
                return std::nullopt;
            }

            float entryDistance = 0.0F;
            float exitDistance = std::numeric_limits<float>::infinity();
            std::size_t entryAxis = 0U;
            std::size_t exitAxis = 0U;
            float entrySign = 1.0F;
            float exitSign = 1.0F;
            bool hasEntryAxis = false;
            bool hasExitAxis = false;

            for (std::size_t axis = 0U; axis < 3U; ++axis)
            {
                const float origin = Component(ray.Origin(), axis);
                const float minimum = Component(box.Minimum(), axis);
                const float maximum = Component(box.Maximum(), axis);
                if (prepared.IsParallel(axis))
                {
                    if (origin < minimum || origin > maximum)
                    {
                        return std::nullopt;
                    }
                    continue;
                }

                const float reciprocal =
                    Component(prepared.ReciprocalDirection(), axis);
                float nearDistance = (minimum - origin) * reciprocal;
                float farDistance = (maximum - origin) * reciprocal;
                float nearSign = -1.0F;
                float farSign = 1.0F;
                if (nearDistance > farDistance)
                {
                    std::swap(nearDistance, farDistance);
                    std::swap(nearSign, farSign);
                }

                if (nearDistance >= entryDistance)
                {
                    entryDistance = nearDistance;
                    entryAxis = axis;
                    entrySign = nearSign;
                    hasEntryAxis = true;
                }
                if (farDistance < exitDistance)
                {
                    exitDistance = farDistance;
                    exitAxis = axis;
                    exitSign = farSign;
                    hasExitAxis = true;
                }
                if (entryDistance > exitDistance)
                {
                    return std::nullopt;
                }
            }

            if (!(exitDistance >= 0.0F) || !std::isfinite(exitDistance) ||
                !hasExitAxis)
            {
                return std::nullopt;
            }

            return RayAabbSlabSolution{entryDistance,
                                       exitDistance,
                                       IsStrictlyInside(box, ray.Origin()),
                                       entryAxis,
                                       exitAxis,
                                       entrySign,
                                       exitSign,
                                       hasEntryAxis};
        }

        struct RaySphereSolution
        {
            float EntryDistance;
            float ExitDistance;
            bool StartsInside;
        };

        // Ray/sphere quadratic cross-checked against DirectXMath
        // BoundingSphere::Intersects and GLM intersectRaySphere (both MIT).
        [[nodiscard]] inline std::optional<RaySphereSolution>
        TryIntersectRaySphere(const Ray3f& ray, const Sphere3f& sphere) noexcept
        {
            if (!ray.IsFinite())
            {
                return std::nullopt;
            }

            const Point3f center = sphere.Center();
            const double mx = static_cast<double>(ray.Origin().X()) -
                              static_cast<double>(center.X());
            const double my = static_cast<double>(ray.Origin().Y()) -
                              static_cast<double>(center.Y());
            const double mz = static_cast<double>(ray.Origin().Z()) -
                              static_cast<double>(center.Z());
            const double dx = static_cast<double>(ray.Direction().Vector().X());
            const double dy = static_cast<double>(ray.Direction().Vector().Y());
            const double dz = static_cast<double>(ray.Direction().Vector().Z());
            const double radius = static_cast<double>(sphere.Radius());
            const double b = mx * dx + my * dy + mz * dz;
            const double c = mx * mx + my * my + mz * mz - radius * radius;
            const double discriminant = b * b - c;
            if (!(discriminant >= 0.0) || !std::isfinite(discriminant))
            {
                return std::nullopt;
            }

            const double root = std::sqrt(discriminant);
            const double nearDistance = -b - root;
            const double farDistance = -b + root;
            if (!(farDistance >= 0.0) || !std::isfinite(farDistance))
            {
                return std::nullopt;
            }

            const bool startsInside = c < 0.0;
            const float entryDistance =
                startsInside ? 0.0F
                             : static_cast<float>(std::max(nearDistance, 0.0));
            const float exitDistance = static_cast<float>(farDistance);
            if (!(entryDistance >= 0.0F) || !std::isfinite(entryDistance) ||
                !std::isfinite(exitDistance))
            {
                return std::nullopt;
            }
            return RaySphereSolution{entryDistance, exitDistance, startsInside};
        }
    }  // namespace detail

    [[nodiscard]] inline std::optional<RaySphereHit3f> Intersect(
        const Ray3f& ray, const Sphere3f& sphere) noexcept
    {
        const auto solution = detail::TryIntersectRaySphere(ray, sphere);
        if (!solution)
        {
            return std::nullopt;
        }

        std::optional<Normal3f> entryNormal;
        if (!solution->StartsInside)
        {
            entryNormal = Normal3f::TryFrom(
                ray.PointAt(solution->EntryDistance) - sphere.Center());
        }
        const auto exitNormal = Normal3f::TryFrom(
            ray.PointAt(solution->ExitDistance) - sphere.Center());

        return RaySphereHit3f{solution->EntryDistance, solution->ExitDistance,
                              solution->StartsInside, entryNormal, exitNormal};
    }

    [[nodiscard]] inline bool Intersects(const Ray3f& ray,
                                         const Sphere3f& sphere) noexcept
    {
        return detail::TryIntersectRaySphere(ray, sphere).has_value();
    }

    [[nodiscard]] inline std::optional<RayAabbHit3f> Intersect(
        const PreparedRay3f& ray, const Aabb3f& box) noexcept
    {
        const auto solution = detail::TryIntersectPreparedAabbSlabs(ray, box);
        if (!solution)
        {
            return std::nullopt;
        }

        std::optional<Normal3f> entryNormal;
        if (!solution->StartsInside && solution->HasEntryAxis)
        {
            entryNormal =
                detail::AxisNormal(solution->EntryAxis, solution->EntrySign);
        }
        return RayAabbHit3f{
            solution->EntryDistance, solution->ExitDistance,
            solution->StartsInside, entryNormal,
            detail::AxisNormal(solution->ExitAxis, solution->ExitSign)};
    }

    [[nodiscard]] inline std::optional<RayAabbHit3f> Intersect(
        const Ray3f& ray, const Aabb3f& box) noexcept
    {
        return Intersect(PreparedRay3f(ray), box);
    }

    [[nodiscard]] inline bool Intersects(const PreparedRay3f& ray,
                                         const Aabb3f& box) noexcept
    {
        return detail::TryIntersectPreparedAabbSlabs(ray, box).has_value();
    }

    [[nodiscard]] inline bool Intersects(const Ray3f& ray,
                                         const Aabb3f& box) noexcept
    {
        return Intersects(PreparedRay3f(ray), box);
    }

    // Standard squared-distance bounds tests; cross-checked against DirectXMath
    // BoundingSphere/BoundingBox intersections (MIT). Touching intersects.
    [[nodiscard]] inline bool Intersects(const Sphere3f& left,
                                         const Sphere3f& right) noexcept
    {
        const Point3f leftCenter = left.Center();
        const Point3f rightCenter = right.Center();
        const double x = static_cast<double>(leftCenter.X()) -
                         static_cast<double>(rightCenter.X());
        const double y = static_cast<double>(leftCenter.Y()) -
                         static_cast<double>(rightCenter.Y());
        const double z = static_cast<double>(leftCenter.Z()) -
                         static_cast<double>(rightCenter.Z());
        const double combinedRadius = static_cast<double>(left.Radius()) +
                                      static_cast<double>(right.Radius());
        return x * x + y * y + z * z <= combinedRadius * combinedRadius;
    }

    [[nodiscard]] inline bool Intersects(const Aabb3f& left,
                                         const Aabb3f& right) noexcept
    {
        return left.Intersects(right);
    }

    [[nodiscard]] inline bool Intersects(const Sphere3f& sphere,
                                         const Aabb3f& box) noexcept
    {
        if (box.IsEmpty())
        {
            return false;
        }

        const Point3f center = sphere.Center();
        const double closestX =
            std::clamp(static_cast<double>(center.X()),
                       static_cast<double>(box.Minimum().X()),
                       static_cast<double>(box.Maximum().X()));
        const double closestY =
            std::clamp(static_cast<double>(center.Y()),
                       static_cast<double>(box.Minimum().Y()),
                       static_cast<double>(box.Maximum().Y()));
        const double closestZ =
            std::clamp(static_cast<double>(center.Z()),
                       static_cast<double>(box.Minimum().Z()),
                       static_cast<double>(box.Maximum().Z()));
        const double x = static_cast<double>(center.X()) - closestX;
        const double y = static_cast<double>(center.Y()) - closestY;
        const double z = static_cast<double>(center.Z()) - closestZ;
        const double radius = static_cast<double>(sphere.Radius());
        return x * x + y * y + z * z <= radius * radius;
    }

    [[nodiscard]] inline bool Intersects(const Aabb3f& box,
                                         const Sphere3f& sphere) noexcept
    {
        return Intersects(sphere, box);
    }
}  // namespace mv::math
