#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Capsule3.hpp>
#include <mv/math/geometry/Line3.hpp>
#include <mv/math/geometry/Plane3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/geometry/Segment3.hpp>
#include <mv/math/geometry/Sphere3.hpp>
#include <mv/math/geometry/Triangle3.hpp>
#include <mv/math/queries/QueryTypes.hpp>

namespace mv::math
{
    namespace detail
    {
        template <typename T>
        [[nodiscard]] inline T PointDistanceSquared(
            const Point3<T>& first, const Point3<T>& second) noexcept
        {
            return LengthSquared(first - second);
        }

        template <typename T>
        using QueryCalculation =
            std::conditional_t<std::is_same_v<T, float>, double, long double>;

        template <typename T>
        [[nodiscard]] inline QueryCalculation<T> DotPrecise(
            const Vec3<T>& first, const Vec3<T>& second) noexcept
        {
            using Calculation = QueryCalculation<T>;
            return static_cast<Calculation>(first.X()) *
                       static_cast<Calculation>(second.X()) +
                   static_cast<Calculation>(first.Y()) *
                       static_cast<Calculation>(second.Y()) +
                   static_cast<Calculation>(first.Z()) *
                       static_cast<Calculation>(second.Z());
        }

        template <typename T>
        [[nodiscard]] inline PointTriangleClosest3<T> MakePointTriangleClosest(
            const Point3<T>& point,
            const Point3<T>& pointOnTriangle,
            const Vec3<T>& barycentric) noexcept
        {
            return PointTriangleClosest3<T>{
                pointOnTriangle, barycentric,
                PointDistanceSquared(point, pointOnTriangle)};
        }
    }  // namespace detail

    // Orthogonal projection specialized for a unit direction; equations and
    // parameter semantics follow Geometric Tools DistPointLine.h (BSL-1.0).
    template <typename T>
    [[nodiscard]] inline PointLineClosest3<T> ClosestPoints(
        const Point3<T>& point, const Line3<T>& line) noexcept
    {
        const T parameter =
            Dot(line.Direction().Vector(), point - line.Origin());
        const Point3<T> pointOnLine = line.PointAt(parameter);
        return PointLineClosest3<T>{
            pointOnLine, parameter,
            detail::PointDistanceSquared(point, pointOnLine)};
    }

    // The line projection above, clamped to the ray domain [0,+infinity),
    // follows Geometric Tools DistPointRay.h (BSL-1.0).
    template <typename T>
    [[nodiscard]] inline PointRayClosest3<T> ClosestPoints(
        const Point3<T>& point, const Ray3<T>& ray) noexcept
    {
        const T parameter =
            std::max(Dot(ray.Direction().Vector(), point - ray.Origin()), T(0));
        const Point3<T> pointOnRay = ray.PointAt(parameter);
        return PointRayClosest3<T>{
            pointOnRay, parameter,
            detail::PointDistanceSquared(point, pointOnRay)};
    }

    // Endpoint tests and the interior projection follow Geometric Tools
    // DistPointSegment.h (BSL-1.0); a zero-length segment returns its start.
    template <typename T>
    [[nodiscard]] inline PointSegmentClosest3<T> ClosestPoints(
        const Point3<T>& point, const Segment3<T>& segment) noexcept
    {
        const Vec3<T> displacement = segment.Displacement();
        const T lengthSquared = LengthSquared(displacement);
        T fraction = T(0);
        if (lengthSquared > T(0))
        {
            fraction = std::clamp(
                Dot(point - segment.Start(), displacement) / lengthSquared,
                T(0), T(1));
        }
        const Point3<T> pointOnSegment = segment.PointAtFraction(fraction);
        return PointSegmentClosest3<T>{
            pointOnSegment, fraction,
            detail::PointDistanceSquared(point, pointOnSegment)};
    }

    // Unit-normal orthogonal projection; see Ericson, Real-Time Collision
    // Detection (2005), section 5.1.1.
    template <typename T>
    [[nodiscard]] inline PointPlaneClosest3<T> ClosestPoints(
        const Point3<T>& point, const Plane3<T>& plane) noexcept
    {
        const T signedDistance = plane.SignedDistance(point);
        return PointPlaneClosest3<T>{
            point - plane.Normal().Vector() * signedDistance, signedDistance,
            signedDistance * signedDistance};
    }

    // Voronoi-region point/triangle query from Ericson, Real-Time Collision
    // Detection (2005), section 5.1.5, cross-checked against Eberly,
    // "Distance Between Point and Triangle in 3D" (1999, CC BY 4.0).
    // Move resolves degenerate triangles by testing their three edges.
    template <typename T>
    [[nodiscard]] inline PointTriangleClosest3<T> ClosestPoints(
        const Point3<T>& point, const Triangle3<T>& triangle) noexcept
    {
        const Point3<T>& first = triangle.First();
        const Point3<T>& second = triangle.Second();
        const Point3<T>& third = triangle.Third();
        const Vec3<T> edge01 = second - first;
        const Vec3<T> edge02 = third - first;

        if (!(LengthSquared(Cross(edge01, edge02)) > T(0)))
        {
            const PointSegmentClosest3<T> edge01Closest =
                ClosestPoints(point, Segment3<T>(first, second));
            PointTriangleClosest3<T> closest = detail::MakePointTriangleClosest(
                point, edge01Closest.PointOnSegment,
                Vec3<T>(T(1) - edge01Closest.SegmentFraction,
                        edge01Closest.SegmentFraction, T(0)));

            const PointSegmentClosest3<T> edge02Closest =
                ClosestPoints(point, Segment3<T>(first, third));
            if (edge02Closest.SquaredDistance < closest.SquaredDistance)
            {
                closest = detail::MakePointTriangleClosest(
                    point, edge02Closest.PointOnSegment,
                    Vec3<T>(T(1) - edge02Closest.SegmentFraction, T(0),
                            edge02Closest.SegmentFraction));
            }

            const PointSegmentClosest3<T> edge12Closest =
                ClosestPoints(point, Segment3<T>(second, third));
            if (edge12Closest.SquaredDistance < closest.SquaredDistance)
            {
                closest = detail::MakePointTriangleClosest(
                    point, edge12Closest.PointOnSegment,
                    Vec3<T>(T(0), T(1) - edge12Closest.SegmentFraction,
                            edge12Closest.SegmentFraction));
            }
            return closest;
        }

        const Vec3<T> fromFirst = point - first;
        const T firstAlong01 = Dot(edge01, fromFirst);
        const T firstAlong02 = Dot(edge02, fromFirst);
        if (firstAlong01 <= T(0) && firstAlong02 <= T(0))
        {
            return detail::MakePointTriangleClosest(point, first,
                                                    Vec3<T>(T(1), T(0), T(0)));
        }

        const Vec3<T> fromSecond = point - second;
        const T secondAlong01 = Dot(edge01, fromSecond);
        const T secondAlong02 = Dot(edge02, fromSecond);
        if (secondAlong01 >= T(0) && secondAlong02 <= secondAlong01)
        {
            return detail::MakePointTriangleClosest(point, second,
                                                    Vec3<T>(T(0), T(1), T(0)));
        }

        const T edge01Region =
            firstAlong01 * secondAlong02 - secondAlong01 * firstAlong02;
        if (edge01Region <= T(0) && firstAlong01 >= T(0) &&
            secondAlong01 <= T(0))
        {
            const T fraction = firstAlong01 / (firstAlong01 - secondAlong01);
            return detail::MakePointTriangleClosest(
                point, first + edge01 * fraction,
                Vec3<T>(T(1) - fraction, fraction, T(0)));
        }

        const Vec3<T> fromThird = point - third;
        const T thirdAlong01 = Dot(edge01, fromThird);
        const T thirdAlong02 = Dot(edge02, fromThird);
        if (thirdAlong02 >= T(0) && thirdAlong01 <= thirdAlong02)
        {
            return detail::MakePointTriangleClosest(point, third,
                                                    Vec3<T>(T(0), T(0), T(1)));
        }

        const T edge02Region =
            thirdAlong01 * firstAlong02 - firstAlong01 * thirdAlong02;
        if (edge02Region <= T(0) && firstAlong02 >= T(0) &&
            thirdAlong02 <= T(0))
        {
            const T fraction = firstAlong02 / (firstAlong02 - thirdAlong02);
            return detail::MakePointTriangleClosest(
                point, first + edge02 * fraction,
                Vec3<T>(T(1) - fraction, T(0), fraction));
        }

        const T edge12Region =
            secondAlong01 * thirdAlong02 - thirdAlong01 * secondAlong02;
        const T secondTowardThird = secondAlong02 - secondAlong01;
        const T thirdTowardSecond = thirdAlong01 - thirdAlong02;
        if (edge12Region <= T(0) && secondTowardThird >= T(0) &&
            thirdTowardSecond >= T(0))
        {
            const T fraction =
                secondTowardThird / (secondTowardThird + thirdTowardSecond);
            return detail::MakePointTriangleClosest(
                point, second + (third - second) * fraction,
                Vec3<T>(T(0), T(1) - fraction, fraction));
        }

        const T reciprocalSum =
            T(1) / (edge12Region + edge02Region + edge01Region);
        const T secondWeight = edge02Region * reciprocalSum;
        const T thirdWeight = edge01Region * reciprocalSum;
        return detail::MakePointTriangleClosest(
            point, first + edge01 * secondWeight + edge02 * thirdWeight,
            Vec3<T>(T(1) - secondWeight - thirdWeight, secondWeight,
                    thirdWeight));
    }

    // Component clamping is the point/AABB distance construction in Ericson,
    // Real-Time Collision Detection (2005), section 5.1.3. Empty boxes and
    // non-finite query points preserve Aabb3's existing fallible contract.
    template <typename T>
    [[nodiscard]] inline std::optional<PointAabbClosest3<T>> TryClosestPoints(
        const Point3<T>& point, const Aabb3<T>& box) noexcept
    {
        const std::optional<Point3<T>> closest = box.TryClosestPoint(point);
        if (!closest)
        {
            return std::nullopt;
        }
        return PointAabbClosest3<T>{
            *closest, detail::PointDistanceSquared(point, *closest)};
    }

    // Radial projection is the solid-sphere specialization of Ericson,
    // Real-Time Collision Detection (2005), section 5.1.4. A contained point
    // is already the nearest member of the bounding volume and has distance 0.
    template <typename T>
    [[nodiscard]] inline PointSphereClosest3<T> ClosestPoints(
        const Point3<T>& point, const Sphere3<T>& sphere) noexcept
    {
        const Point3<T> center = sphere.Center();
        const Vec3<T> offset = point - center;
        const T squaredCenterDistance = LengthSquared(offset);
        const T squaredRadius = sphere.Radius() * sphere.Radius();
        if (squaredCenterDistance <= squaredRadius)
        {
            return PointSphereClosest3<T>{point, T(0)};
        }

        const T scale = sphere.Radius() / std::sqrt(squaredCenterDistance);
        const Point3<T> pointInSphere = center + offset * scale;
        return PointSphereClosest3<T>{
            pointInSphere, detail::PointDistanceSquared(point, pointInSphere)};
    }

    // A capsule is a segment swept by a sphere (Ericson, Real-Time Collision
    // Detection, 2005, sections 4.5 and 4.5.1). Project to the center line,
    // then apply the same radial solid-volume rule as point/sphere.
    template <typename T>
    [[nodiscard]] inline PointCapsuleClosest3<T> ClosestPoints(
        const Point3<T>& point, const Capsule3<T>& capsule) noexcept
    {
        const PointSegmentClosest3<T> centerLineClosest =
            ClosestPoints(point, capsule.CenterLine());
        if (centerLineClosest.SquaredDistance <=
            capsule.Radius() * capsule.Radius())
        {
            return PointCapsuleClosest3<T>{
                point, centerLineClosest.SegmentFraction, T(0)};
        }

        const Vec3<T> radialOffset = point - centerLineClosest.PointOnSegment;
        const T scale =
            capsule.Radius() / std::sqrt(centerLineClosest.SquaredDistance);
        const Point3<T> pointInCapsule =
            centerLineClosest.PointOnSegment + radialOffset * scale;
        return PointCapsuleClosest3<T>{
            pointInCapsule, centerLineClosest.SegmentFraction,
            detail::PointDistanceSquared(point, pointInCapsule)};
    }

    // ClosestPtSegmentSegment from Ericson, Real-Time Collision Detection
    // (2005), section 5.1.9. Wider intermediates reduce cancellation for
    // nearly parallel segments; degenerate segments remain valid.
    template <typename T>
    [[nodiscard]] inline SegmentSegmentClosest3<T> ClosestPoints(
        const Segment3<T>& first, const Segment3<T>& second) noexcept
    {
        using Calculation = detail::QueryCalculation<T>;
        const Vec3<T> firstDirection = first.Displacement();
        const Vec3<T> secondDirection = second.Displacement();
        const Vec3<T> startOffset = first.Start() - second.Start();
        const Calculation firstLengthSquared =
            detail::DotPrecise(firstDirection, firstDirection);
        const Calculation secondLengthSquared =
            detail::DotPrecise(secondDirection, secondDirection);
        const Calculation secondProjection =
            detail::DotPrecise(secondDirection, startOffset);

        Calculation firstFraction = Calculation(0);
        Calculation secondFraction = Calculation(0);
        if (firstLengthSquared == Calculation(0) &&
            secondLengthSquared == Calculation(0))
        {
            // Both segments are points; the initialized fractions are final.
        }
        else if (firstLengthSquared == Calculation(0))
        {
            secondFraction = std::clamp(secondProjection / secondLengthSquared,
                                        Calculation(0), Calculation(1));
        }
        else
        {
            const Calculation firstProjection =
                detail::DotPrecise(firstDirection, startOffset);
            if (secondLengthSquared == Calculation(0))
            {
                firstFraction =
                    std::clamp(-firstProjection / firstLengthSquared,
                               Calculation(0), Calculation(1));
            }
            else
            {
                const Calculation directionsDot =
                    detail::DotPrecise(firstDirection, secondDirection);
                const Calculation denominator =
                    firstLengthSquared * secondLengthSquared -
                    directionsDot * directionsDot;
                if (denominator > Calculation(0))
                {
                    firstFraction =
                        std::clamp((directionsDot * secondProjection -
                                    firstProjection * secondLengthSquared) /
                                       denominator,
                                   Calculation(0), Calculation(1));
                }

                secondFraction =
                    (directionsDot * firstFraction + secondProjection) /
                    secondLengthSquared;
                if (secondFraction < Calculation(0))
                {
                    secondFraction = Calculation(0);
                    firstFraction =
                        std::clamp(-firstProjection / firstLengthSquared,
                                   Calculation(0), Calculation(1));
                }
                else if (secondFraction > Calculation(1))
                {
                    secondFraction = Calculation(1);
                    firstFraction = std::clamp(
                        (directionsDot - firstProjection) / firstLengthSquared,
                        Calculation(0), Calculation(1));
                }
            }
        }

        const T firstValue = static_cast<T>(firstFraction);
        const T secondValue = static_cast<T>(secondFraction);
        const Point3<T> pointOnFirst = first.PointAtFraction(firstValue);
        const Point3<T> pointOnSecond = second.PointAtFraction(secondValue);
        return SegmentSegmentClosest3<T>{
            pointOnFirst, pointOnSecond, firstValue, secondValue,
            detail::PointDistanceSquared(pointOnFirst, pointOnSecond)};
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(const Point3<T>& point,
                                                const Line3<T>& line) noexcept
    {
        return ClosestPoints(point, line).PointOnLine;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(const Point3<T>& point,
                                                const Ray3<T>& ray) noexcept
    {
        return ClosestPoints(point, ray).PointOnRay;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(
        const Point3<T>& point, const Segment3<T>& segment) noexcept
    {
        return ClosestPoints(point, segment).PointOnSegment;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(const Point3<T>& point,
                                                const Plane3<T>& plane) noexcept
    {
        return ClosestPoints(point, plane).PointOnPlane;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(
        const Point3<T>& point, const Triangle3<T>& triangle) noexcept
    {
        return ClosestPoints(point, triangle).PointOnTriangle;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>> TryClosestPoint(
        const Point3<T>& point, const Aabb3<T>& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<Point3<T>>(result->PointInAabb)
                      : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(
        const Point3<T>& point, const Sphere3<T>& sphere) noexcept
    {
        return ClosestPoints(point, sphere).PointInSphere;
    }

    template <typename T>
    [[nodiscard]] inline Point3<T> ClosestPoint(
        const Point3<T>& point, const Capsule3<T>& capsule) noexcept
    {
        return ClosestPoints(point, capsule).PointInCapsule;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Line3<T>& line) noexcept
    {
        return ClosestPoints(point, line).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Ray3<T>& ray) noexcept
    {
        return ClosestPoints(point, ray).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Segment3<T>& segment) noexcept
    {
        return ClosestPoints(point, segment).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Plane3<T>& plane) noexcept
    {
        return ClosestPoints(point, plane).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(
        const Point3<T>& point, const Triangle3<T>& triangle) noexcept
    {
        return ClosestPoints(point, triangle).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<T> TryDistanceSquared(
        const Point3<T>& point, const Aabb3<T>& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<T>(result->SquaredDistance)
                      : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Sphere3<T>& sphere) noexcept
    {
        return ClosestPoints(point, sphere).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Point3<T>& point,
                                           const Capsule3<T>& capsule) noexcept
    {
        return ClosestPoints(point, capsule).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T DistanceSquared(const Segment3<T>& first,
                                           const Segment3<T>& second) noexcept
    {
        return ClosestPoints(first, second).SquaredDistance;
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Line3<T>& line) noexcept
    {
        return ClosestPoints(point, line).Distance();
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Ray3<T>& ray) noexcept
    {
        return ClosestPoints(point, ray).Distance();
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Segment3<T>& segment) noexcept
    {
        return ClosestPoints(point, segment).Distance();
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Plane3<T>& plane) noexcept
    {
        return ClosestPoints(point, plane).Distance();
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Triangle3<T>& triangle) noexcept
    {
        return ClosestPoints(point, triangle).Distance();
    }

    template <typename T>
    [[nodiscard]] inline std::optional<T> TryDistance(
        const Point3<T>& point, const Aabb3<T>& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<T>(result->Distance()) : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Sphere3<T>& sphere) noexcept
    {
        return ClosestPoints(point, sphere).Distance();
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Point3<T>& point,
                                    const Capsule3<T>& capsule) noexcept
    {
        return ClosestPoints(point, capsule).Distance();
    }

    template <typename T>
    [[nodiscard]] inline bool Contains(const Capsule3<T>& capsule,
                                       const Point3<T>& point) noexcept
    {
        return ClosestPoints(point, capsule).SquaredDistance == T(0);
    }

    template <typename T>
    [[nodiscard]] inline T Distance(const Segment3<T>& first,
                                    const Segment3<T>& second) noexcept
    {
        return ClosestPoints(first, second).Distance();
    }
}  // namespace mv::math
