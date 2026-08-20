#pragma once

#include <array>
#include <cmath>
#include <cstddef>
#include <limits>
#include <optional>
#include <type_traits>

#include <mv/math/geometry/Obb3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/geometry/Sphere3.hpp>
#include <mv/math/queries/BoundsQueries.hpp>

namespace mv::math
{
    template <typename T>
    [[nodiscard]] inline std::optional<RayObbHit3<T>> Intersect(
        const Ray3<T>& ray, const Obb3<T>& box) noexcept
    {
        const Rotation3<T> inverse = box.Orientation().Inverse();
        const auto localDirection =
            Direction3<T>::TryFrom(Rotate(inverse, ray.Direction().Vector()));
        const auto localRay =
            localDirection ? Ray3<T>::TryFromOriginDirection(
                                 Point3<T>::FromVector(Rotate(
                                     inverse, ray.Origin() - box.Center())),
                                 *localDirection)
                           : std::nullopt;
        const auto localBox = Aabb3<T>::TryFromCenterHalfExtents(
            Point3<T>::Origin(), box.HalfExtents());
        if (!localRay || !localBox)
        {
            return std::nullopt;
        }

        const auto localHit = Intersect(*localRay, *localBox);
        if (!localHit)
        {
            return std::nullopt;
        }
        std::optional<Normal3<T>> entryNormal;
        if (localHit->EntryNormal)
        {
            entryNormal = Rotate(box.Orientation(), *localHit->EntryNormal);
        }
        return RayObbHit3<T>{localHit->EntryDistance, localHit->ExitDistance,
                             localHit->StartsInside, entryNormal,
                             Rotate(box.Orientation(), localHit->ExitNormal)};
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Ray3<T>& ray,
                                         const Obb3<T>& box) noexcept
    {
        return Intersect(ray, box).has_value();
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Sphere3<T>& sphere,
                                         const Obb3<T>& box) noexcept
    {
        const Vec3<T> difference =
            sphere.Center() - box.ClosestPoint(sphere.Center());
        return LengthSquared(difference) <= sphere.Radius() * sphere.Radius();
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Obb3<T>& box,
                                         const Sphere3<T>& sphere) noexcept
    {
        return Intersects(sphere, box);
    }

    // Full 15-axis OBB separating-axis test from Ericson, Real-Time Collision
    // Detection (2005), section 4.4.1. The epsilon makes nearly parallel axes
    // conservative, avoiding culling false negatives at the cost of possible
    // touching/near-touching false positives.
    template <typename T>
    [[nodiscard]] inline bool Intersects(const Obb3<T>& first,
                                         const Obb3<T>& second) noexcept
    {
        const std::array<Vec3<T>, 3> a = {first.AxisX().Vector(),
                                          first.AxisY().Vector(),
                                          first.AxisZ().Vector()};
        const std::array<Vec3<T>, 3> b = {second.AxisX().Vector(),
                                          second.AxisY().Vector(),
                                          second.AxisZ().Vector()};
        const std::array<T, 3> ae = {first.HalfExtents().X(),
                                     first.HalfExtents().Y(),
                                     first.HalfExtents().Z()};
        const std::array<T, 3> be = {second.HalfExtents().X(),
                                     second.HalfExtents().Y(),
                                     second.HalfExtents().Z()};
        T rotation[3][3]{};
        T absoluteRotation[3][3]{};
        const T epsilon = std::numeric_limits<T>::epsilon() * T(8);
        for (std::size_t i = 0U; i < 3U; ++i)
        {
            for (std::size_t j = 0U; j < 3U; ++j)
            {
                rotation[i][j] = Dot(a[i], b[j]);
                absoluteRotation[i][j] = std::abs(rotation[i][j]) + epsilon;
            }
        }

        const Vec3<T> centerDifference = second.Center() - first.Center();
        const std::array<T, 3> translation = {Dot(centerDifference, a[0]),
                                              Dot(centerDifference, a[1]),
                                              Dot(centerDifference, a[2])};
        for (std::size_t i = 0U; i < 3U; ++i)
        {
            const T secondRadius = be[0] * absoluteRotation[i][0] +
                                   be[1] * absoluteRotation[i][1] +
                                   be[2] * absoluteRotation[i][2];
            if (std::abs(translation[i]) > ae[i] + secondRadius)
            {
                return false;
            }
        }
        for (std::size_t j = 0U; j < 3U; ++j)
        {
            const T projection = std::abs(translation[0] * rotation[0][j] +
                                          translation[1] * rotation[1][j] +
                                          translation[2] * rotation[2][j]);
            const T firstRadius = ae[0] * absoluteRotation[0][j] +
                                  ae[1] * absoluteRotation[1][j] +
                                  ae[2] * absoluteRotation[2][j];
            if (projection > firstRadius + be[j])
            {
                return false;
            }
        }
        for (std::size_t i = 0U; i < 3U; ++i)
        {
            const std::size_t i1 = (i + 1U) % 3U;
            const std::size_t i2 = (i + 2U) % 3U;
            for (std::size_t j = 0U; j < 3U; ++j)
            {
                const std::size_t j1 = (j + 1U) % 3U;
                const std::size_t j2 = (j + 2U) % 3U;
                const T projection =
                    std::abs(translation[i2] * rotation[i1][j] -
                             translation[i1] * rotation[i2][j]);
                const T firstRadius = ae[i1] * absoluteRotation[i2][j] +
                                      ae[i2] * absoluteRotation[i1][j];
                const T secondRadius = be[j1] * absoluteRotation[i][j2] +
                                       be[j2] * absoluteRotation[i][j1];
                if (projection > firstRadius + secondRadius)
                {
                    return false;
                }
            }
        }
        return true;
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Aabb3<T>& first,
                                         const Obb3<T>& second) noexcept
    {
        const auto center = first.TryCenter();
        const auto halfExtents = first.TryHalfExtents();
        if (!center || !halfExtents)
        {
            return false;
        }
        const auto firstObb = Obb3<T>::TryFromCenterHalfExtents(
            *center, *halfExtents, Rotation3<T>::Identity());
        return firstObb && Intersects(*firstObb, second);
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(const Obb3<T>& first,
                                         const Aabb3<T>& second) noexcept
    {
        return Intersects(second, first);
    }
}  // namespace mv::math
