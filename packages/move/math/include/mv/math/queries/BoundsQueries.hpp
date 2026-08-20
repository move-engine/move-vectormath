#pragma once

#include <algorithm>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>

#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/geometry/Sphere3.hpp>
#include <mv/math/queries/ClosestPointQueries.hpp>
#include <mv/math/queries/PreparedRay3.hpp>
#include <mv/math/queries/QueryTypes.hpp>

namespace mv::math
{
    namespace detail
    {
        template <typename T>
        [[nodiscard]] inline T Component(const Vec3<T>& value,
                                         std::size_t axis) noexcept
        {
            if (axis == 0U)
            {
                return value.X();
            }
            return axis == 1U ? value.Y() : value.Z();
        }

        template <typename T>
        [[nodiscard]] inline T Component(const Point3<T>& value,
                                         std::size_t axis) noexcept
        {
            if (axis == 0U)
            {
                return value.X();
            }
            return axis == 1U ? value.Y() : value.Z();
        }

        template <typename T>
        [[nodiscard]] inline Normal3<T> AxisNormal(std::size_t axis,
                                                   T sign) noexcept
        {
            const Normal3<T> normal = axis == 0U   ? Normal3<T>::AxisX()
                                      : axis == 1U ? Normal3<T>::AxisY()
                                                   : Normal3<T>::AxisZ();
            return sign < T(0) ? -normal : normal;
        }

        template <typename T>
        [[nodiscard]] inline bool IsStrictlyInside(
            const Aabb3<T>& box, const Point3<T>& point) noexcept
        {
            return point.X() > box.Minimum().X() &&
                   point.Y() > box.Minimum().Y() &&
                   point.Z() > box.Minimum().Z() &&
                   point.X() < box.Maximum().X() &&
                   point.Y() < box.Maximum().Y() &&
                   point.Z() < box.Maximum().Z();
        }

        template <typename T>
        struct RayAabbSlabSolution
        {
            T EntryDistance;
            T ExitDistance;
            bool StartsInside;
            std::size_t EntryAxis;
            std::size_t ExitAxis;
            T EntrySign;
            T ExitSign;
            bool HasEntryAxis;
        };

        // Williams et al. ray/box slab test (JGT 2005); Move explicitly handles
        // parallel axes and returns a clipped interval with surface normals.
        template <typename T>
        [[nodiscard]] inline std::optional<RayAabbSlabSolution<T>>
        TryIntersectPreparedAabbSlabs(const PreparedRay3<T>& prepared,
                                      const Aabb3<T>& box) noexcept
        {
            const Ray3<T>& ray = prepared.Ray();
            if (box.IsEmpty())
            {
                return std::nullopt;
            }

            T entryDistance = T(0);
            T exitDistance = std::numeric_limits<T>::infinity();
            std::size_t entryAxis = 0U;
            std::size_t exitAxis = 0U;
            T entrySign = T(1);
            T exitSign = T(1);
            bool hasEntryAxis = false;
            bool hasExitAxis = false;

            for (std::size_t axis = 0U; axis < 3U; ++axis)
            {
                const T origin = Component(ray.Origin(), axis);
                const T minimum = Component(box.Minimum(), axis);
                const T maximum = Component(box.Maximum(), axis);
                if (prepared.IsParallel(axis))
                {
                    if (origin < minimum || origin > maximum)
                    {
                        return std::nullopt;
                    }
                    continue;
                }

                const T reciprocal =
                    Component(prepared.ReciprocalDirection(), axis);
                T nearDistance = (minimum - origin) * reciprocal;
                T farDistance = (maximum - origin) * reciprocal;
                T nearSign = T(-1);
                T farSign = T(1);
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

            if (!(exitDistance >= T(0)) || !std::isfinite(exitDistance) ||
                !hasExitAxis)
            {
                return std::nullopt;
            }

            return RayAabbSlabSolution<T>{entryDistance,
                                          exitDistance,
                                          IsStrictlyInside(box, ray.Origin()),
                                          entryAxis,
                                          exitAxis,
                                          entrySign,
                                          exitSign,
                                          hasEntryAxis};
        }

        // Predicate-only Williams et al. slab test (JGT 2005). Keeping this
        // separate from the detailed solver avoids tracking hit axes, signs,
        // and inside state when the caller only needs a boolean result.
        template <typename T>
        [[nodiscard]] inline bool IntersectsPreparedAabbSlabs(
            const PreparedRay3<T>& prepared, const Aabb3<T>& box) noexcept
        {
            if (box.IsEmpty())
            {
                return false;
            }

            const Ray3<T>& ray = prepared.Ray();
            T entryDistance = T(0);
            T exitDistance = std::numeric_limits<T>::infinity();
            for (std::size_t axis = 0U; axis < 3U; ++axis)
            {
                const T origin = Component(ray.Origin(), axis);
                const T minimum = Component(box.Minimum(), axis);
                const T maximum = Component(box.Maximum(), axis);
                if (prepared.IsParallel(axis))
                {
                    if (origin < minimum || origin > maximum)
                    {
                        return false;
                    }
                    continue;
                }

                const T reciprocal =
                    Component(prepared.ReciprocalDirection(), axis);
                T nearDistance = (minimum - origin) * reciprocal;
                T farDistance = (maximum - origin) * reciprocal;
                if (nearDistance > farDistance)
                {
                    std::swap(nearDistance, farDistance);
                }
                entryDistance = std::max(entryDistance, nearDistance);
                exitDistance = std::min(exitDistance, farDistance);
                if (entryDistance > exitDistance)
                {
                    return false;
                }
            }
            return exitDistance >= T(0) && std::isfinite(exitDistance);
        }

        template <typename T>
        struct RaySphereSolution
        {
            T EntryDistance;
            T ExitDistance;
            bool StartsInside;
        };

        // Ray/sphere quadratic cross-checked against DirectXMath
        // BoundingSphere::Intersects and GLM intersectRaySphere (both MIT).
        template <typename T>
        [[nodiscard]] inline std::optional<RaySphereSolution<T>>
        TryIntersectRaySphere(const Ray3<T>& ray,
                              const Sphere3<T>& sphere) noexcept
        {
            using Calculation = QueryCalculation<T>;
            const Point3<T> center = sphere.Center();
            const Calculation mx = static_cast<Calculation>(ray.Origin().X()) -
                                   static_cast<Calculation>(center.X());
            const Calculation my = static_cast<Calculation>(ray.Origin().Y()) -
                                   static_cast<Calculation>(center.Y());
            const Calculation mz = static_cast<Calculation>(ray.Origin().Z()) -
                                   static_cast<Calculation>(center.Z());
            const Calculation dx =
                static_cast<Calculation>(ray.Direction().Vector().X());
            const Calculation dy =
                static_cast<Calculation>(ray.Direction().Vector().Y());
            const Calculation dz =
                static_cast<Calculation>(ray.Direction().Vector().Z());
            const Calculation radius =
                static_cast<Calculation>(sphere.Radius());
            const Calculation b = mx * dx + my * dy + mz * dz;
            const Calculation c = mx * mx + my * my + mz * mz - radius * radius;
            const Calculation discriminant = b * b - c;
            if (!(discriminant >= Calculation(0)) ||
                !std::isfinite(discriminant))
            {
                return std::nullopt;
            }

            const Calculation root = std::sqrt(discriminant);
            const Calculation nearDistance = -b - root;
            const Calculation farDistance = -b + root;
            if (!(farDistance >= Calculation(0)) || !std::isfinite(farDistance))
            {
                return std::nullopt;
            }

            const bool startsInside = c < Calculation(0);
            const T entryDistance =
                startsInside
                    ? T(0)
                    : static_cast<T>(std::max(nearDistance, Calculation(0)));
            const T exitDistance = static_cast<T>(farDistance);
            if (!(entryDistance >= T(0)) || !std::isfinite(entryDistance) ||
                !std::isfinite(exitDistance))
            {
                return std::nullopt;
            }
            return RaySphereSolution<T>{entryDistance, exitDistance,
                                        startsInside};
        }
    }  // namespace detail

    template <typename T>
    [[nodiscard]] inline std::optional<RaySphereHit3<T>> Intersect(
        const Ray3<T>& ray, const Sphere3<T>& sphere) noexcept
    {
        const auto solution = detail::TryIntersectRaySphere(ray, sphere);
        if (!solution)
        {
            return std::nullopt;
        }

        std::optional<Normal3<T>> entryNormal;
        if (!solution->StartsInside)
        {
            entryNormal = Normal3<T>::TryFrom(
                ray.PointAt(solution->EntryDistance) - sphere.Center());
        }
        const auto exitNormal = Normal3<T>::TryFrom(
            ray.PointAt(solution->ExitDistance) - sphere.Center());

        return RaySphereHit3<T>{solution->EntryDistance, solution->ExitDistance,
                                solution->StartsInside, entryNormal,
                                exitNormal};
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Ray3<T>& ray,
                                         const Sphere3<T>& sphere) noexcept
    {
        return detail::TryIntersectRaySphere(ray, sphere).has_value();
    }

    template <typename T>
    [[nodiscard]] inline std::optional<RayAabbHit3<T>> Intersect(
        const PreparedRay3<T>& ray, const Aabb3<T>& box) noexcept
    {
        const auto solution = detail::TryIntersectPreparedAabbSlabs(ray, box);
        if (!solution)
        {
            return std::nullopt;
        }

        std::optional<Normal3<T>> entryNormal;
        if (!solution->StartsInside && solution->HasEntryAxis)
        {
            entryNormal =
                detail::AxisNormal(solution->EntryAxis, solution->EntrySign);
        }
        return RayAabbHit3<T>{
            solution->EntryDistance, solution->ExitDistance,
            solution->StartsInside, entryNormal,
            detail::AxisNormal(solution->ExitAxis, solution->ExitSign)};
    }

    template <typename T>
    [[nodiscard]] inline std::optional<RayAabbHit3<T>> Intersect(
        const Ray3<T>& ray, const Aabb3<T>& box) noexcept
    {
        return Intersect(PreparedRay3<T>(ray), box);
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const PreparedRay3<T>& ray,
                                         const Aabb3<T>& box) noexcept
    {
        return detail::IntersectsPreparedAabbSlabs(ray, box);
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Ray3<T>& ray,
                                         const Aabb3<T>& box) noexcept
    {
        return Intersects(PreparedRay3<T>(ray), box);
    }

    // Standard squared-distance bounds tests; cross-checked against DirectXMath
    // BoundingSphere/BoundingBox intersections (MIT). Touching intersects.
    template <typename T>
    [[nodiscard]] inline bool Intersects(const Sphere3<T>& left,
                                         const Sphere3<T>& right) noexcept
    {
        using Calculation = detail::QueryCalculation<T>;
        const Point3<T> leftCenter = left.Center();
        const Point3<T> rightCenter = right.Center();
        const Calculation x = static_cast<Calculation>(leftCenter.X()) -
                              static_cast<Calculation>(rightCenter.X());
        const Calculation y = static_cast<Calculation>(leftCenter.Y()) -
                              static_cast<Calculation>(rightCenter.Y());
        const Calculation z = static_cast<Calculation>(leftCenter.Z()) -
                              static_cast<Calculation>(rightCenter.Z());
        const Calculation combinedRadius =
            static_cast<Calculation>(left.Radius()) +
            static_cast<Calculation>(right.Radius());
        return x * x + y * y + z * z <= combinedRadius * combinedRadius;
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Aabb3<T>& left,
                                         const Aabb3<T>& right) noexcept
    {
        return left.Intersects(right);
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Sphere3<T>& sphere,
                                         const Aabb3<T>& box) noexcept
    {
        if (box.IsEmpty())
        {
            return false;
        }

        using Calculation = detail::QueryCalculation<T>;
        const Point3<T> center = sphere.Center();
        const Calculation closestX =
            std::clamp(static_cast<Calculation>(center.X()),
                       static_cast<Calculation>(box.Minimum().X()),
                       static_cast<Calculation>(box.Maximum().X()));
        const Calculation closestY =
            std::clamp(static_cast<Calculation>(center.Y()),
                       static_cast<Calculation>(box.Minimum().Y()),
                       static_cast<Calculation>(box.Maximum().Y()));
        const Calculation closestZ =
            std::clamp(static_cast<Calculation>(center.Z()),
                       static_cast<Calculation>(box.Minimum().Z()),
                       static_cast<Calculation>(box.Maximum().Z()));
        const Calculation x = static_cast<Calculation>(center.X()) - closestX;
        const Calculation y = static_cast<Calculation>(center.Y()) - closestY;
        const Calculation z = static_cast<Calculation>(center.Z()) - closestZ;
        const Calculation radius = static_cast<Calculation>(sphere.Radius());
        return x * x + y * y + z * z <= radius * radius;
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Aabb3<T>& box,
                                         const Sphere3<T>& sphere) noexcept
    {
        return Intersects(sphere, box);
    }

    // Sphere-swept-volume reduction from Ericson, Real-Time Collision
    // Detection (2005), section 4.5.1: compare the distance between the inner
    // structures with the sum of sweep radii. Touching intersects.
    template <typename T>
    [[nodiscard]] inline bool Intersects(const Capsule3<T>& capsule,
                                         const Sphere3<T>& sphere) noexcept
    {
        const T combinedRadius = capsule.Radius() + sphere.Radius();
        return DistanceSquared(sphere.Center(), capsule.CenterLine()) <=
               combinedRadius * combinedRadius;
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Sphere3<T>& sphere,
                                         const Capsule3<T>& capsule) noexcept
    {
        return Intersects(capsule, sphere);
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Capsule3<T>& first,
                                         const Capsule3<T>& second) noexcept
    {
        const T combinedRadius = first.Radius() + second.Radius();
        return DistanceSquared(first.CenterLine(), second.CenterLine()) <=
               combinedRadius * combinedRadius;
    }
}  // namespace mv::math
