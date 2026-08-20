#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    enum class FaceOrientation
    {
        Front,
        Back
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointLineClosest3
    {
        Point3<T> PointOnLine;
        T LineDistance;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointLineClosest3&,
            const PointLineClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointRayClosest3
    {
        Point3<T> PointOnRay;
        T RayDistance;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(const PointRayClosest3&,
                                             const PointRayClosest3&) noexcept =
            default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointSegmentClosest3
    {
        Point3<T> PointOnSegment;
        T SegmentFraction;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointSegmentClosest3&,
            const PointSegmentClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointPlaneClosest3
    {
        Point3<T> PointOnPlane;
        T SignedDistance;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointPlaneClosest3&,
            const PointPlaneClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointTriangleClosest3
    {
        Point3<T> PointOnTriangle;
        Vec3<T> Barycentric;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointTriangleClosest3&,
            const PointTriangleClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointAabbClosest3
    {
        Point3<T> PointInAabb;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointAabbClosest3&,
            const PointAabbClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointSphereClosest3
    {
        Point3<T> PointInSphere;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointSphereClosest3&,
            const PointSphereClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct PointCapsuleClosest3
    {
        Point3<T> PointInCapsule;
        T CenterLineFraction;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointCapsuleClosest3&,
            const PointCapsuleClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct SegmentSegmentClosest3
    {
        Point3<T> PointOnFirst;
        Point3<T> PointOnSecond;
        T FirstFraction;
        T SecondFraction;
        T SquaredDistance;

        [[nodiscard]] T Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const SegmentSegmentClosest3&,
            const SegmentSegmentClosest3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RayPlaneHit3
    {
        T Distance;
        Point3<T> Point;
        Normal3<T> Normal;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(
            const RayPlaneHit3&, const RayPlaneHit3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RayTriangleHit3
    {
        T Distance;
        Point3<T> Point;
        Normal3<T> GeometricNormal;
        Vec3<T> Barycentric;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(
            const RayTriangleHit3&, const RayTriangleHit3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RaySphereHit3
    {
        T EntryDistance;
        T ExitDistance;
        bool StartsInside;
        std::optional<Normal3<T>> EntryNormal;
        std::optional<Normal3<T>> ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RaySphereHit3&, const RaySphereHit3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RayAabbHit3
    {
        T EntryDistance;
        T ExitDistance;
        bool StartsInside;
        std::optional<Normal3<T>> EntryNormal;
        Normal3<T> ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RayAabbHit3&, const RayAabbHit3&) noexcept = default;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RayObbHit3
    {
        T EntryDistance;
        T ExitDistance;
        bool StartsInside;
        std::optional<Normal3<T>> EntryNormal;
        Normal3<T> ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RayObbHit3&, const RayObbHit3&) noexcept = default;
    };

    using PointLineClosest3f = PointLineClosest3<float>;
    using PointLineClosest3d = PointLineClosest3<double>;
    using PointRayClosest3f = PointRayClosest3<float>;
    using PointRayClosest3d = PointRayClosest3<double>;
    using PointSegmentClosest3f = PointSegmentClosest3<float>;
    using PointSegmentClosest3d = PointSegmentClosest3<double>;
    using PointPlaneClosest3f = PointPlaneClosest3<float>;
    using PointPlaneClosest3d = PointPlaneClosest3<double>;
    using PointTriangleClosest3f = PointTriangleClosest3<float>;
    using PointTriangleClosest3d = PointTriangleClosest3<double>;
    using PointAabbClosest3f = PointAabbClosest3<float>;
    using PointAabbClosest3d = PointAabbClosest3<double>;
    using PointSphereClosest3f = PointSphereClosest3<float>;
    using PointSphereClosest3d = PointSphereClosest3<double>;
    using PointCapsuleClosest3f = PointCapsuleClosest3<float>;
    using PointCapsuleClosest3d = PointCapsuleClosest3<double>;
    using SegmentSegmentClosest3f = SegmentSegmentClosest3<float>;
    using SegmentSegmentClosest3d = SegmentSegmentClosest3<double>;
    using RayPlaneHit3f = RayPlaneHit3<float>;
    using RayPlaneHit3d = RayPlaneHit3<double>;
    using RayTriangleHit3f = RayTriangleHit3<float>;
    using RayTriangleHit3d = RayTriangleHit3<double>;
    using RaySphereHit3f = RaySphereHit3<float>;
    using RaySphereHit3d = RaySphereHit3<double>;
    using RayAabbHit3f = RayAabbHit3<float>;
    using RayAabbHit3d = RayAabbHit3<double>;
    using RayObbHit3f = RayObbHit3<float>;
    using RayObbHit3d = RayObbHit3<double>;
}  // namespace mv::math

static_assert(std::is_trivially_copyable_v<mv::math::RayPlaneHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayTriangleHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RaySphereHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayAabbHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointLineClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointRayClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointSegmentClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointPlaneClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointTriangleClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointAabbClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointSphereClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PointCapsuleClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::SegmentSegmentClosest3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayPlaneHit3d>);
static_assert(std::is_trivially_copyable_v<mv::math::RayTriangleHit3d>);
static_assert(std::is_trivially_copyable_v<mv::math::RaySphereHit3d>);
static_assert(std::is_trivially_copyable_v<mv::math::RayAabbHit3d>);
static_assert(std::is_trivially_copyable_v<mv::math::RayObbHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayObbHit3d>);
