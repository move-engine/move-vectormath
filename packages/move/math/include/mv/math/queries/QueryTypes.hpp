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

    struct PointLineClosest3f
    {
        Point3f PointOnLine;
        float LineDistance;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointLineClosest3f&,
            const PointLineClosest3f&) noexcept = default;
    };

    struct PointRayClosest3f
    {
        Point3f PointOnRay;
        float RayDistance;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointRayClosest3f&,
            const PointRayClosest3f&) noexcept = default;
    };

    struct PointSegmentClosest3f
    {
        Point3f PointOnSegment;
        float SegmentFraction;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointSegmentClosest3f&,
            const PointSegmentClosest3f&) noexcept = default;
    };

    struct PointPlaneClosest3f
    {
        Point3f PointOnPlane;
        float SignedDistance;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointPlaneClosest3f&,
            const PointPlaneClosest3f&) noexcept = default;
    };

    struct PointTriangleClosest3f
    {
        Point3f PointOnTriangle;
        Vec3f Barycentric;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointTriangleClosest3f&,
            const PointTriangleClosest3f&) noexcept = default;
    };

    struct PointAabbClosest3f
    {
        Point3f PointInAabb;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointAabbClosest3f&,
            const PointAabbClosest3f&) noexcept = default;
    };

    struct PointSphereClosest3f
    {
        Point3f PointInSphere;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointSphereClosest3f&,
            const PointSphereClosest3f&) noexcept = default;
    };

    struct PointCapsuleClosest3f
    {
        Point3f PointInCapsule;
        float CenterLineFraction;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const PointCapsuleClosest3f&,
            const PointCapsuleClosest3f&) noexcept = default;
    };

    struct SegmentSegmentClosest3f
    {
        Point3f PointOnFirst;
        Point3f PointOnSecond;
        float FirstFraction;
        float SecondFraction;
        float SquaredDistance;

        [[nodiscard]] float Distance() const noexcept
        {
            return std::sqrt(SquaredDistance);
        }

        [[nodiscard]] friend bool operator==(
            const SegmentSegmentClosest3f&,
            const SegmentSegmentClosest3f&) noexcept = default;
    };

    struct RayPlaneHit3f
    {
        float Distance;
        Point3f Point;
        Normal3f Normal;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(
            const RayPlaneHit3f&, const RayPlaneHit3f&) noexcept = default;
    };

    struct RayTriangleHit3f
    {
        float Distance;
        Point3f Point;
        Normal3f GeometricNormal;
        Vec3f Barycentric;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(const RayTriangleHit3f&,
                                             const RayTriangleHit3f&) noexcept =
            default;
    };

    struct RaySphereHit3f
    {
        float EntryDistance;
        float ExitDistance;
        bool StartsInside;
        std::optional<Normal3f> EntryNormal;
        std::optional<Normal3f> ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RaySphereHit3f&, const RaySphereHit3f&) noexcept = default;
    };

    struct RayAabbHit3f
    {
        float EntryDistance;
        float ExitDistance;
        bool StartsInside;
        std::optional<Normal3f> EntryNormal;
        Normal3f ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RayAabbHit3f&, const RayAabbHit3f&) noexcept = default;
    };
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
