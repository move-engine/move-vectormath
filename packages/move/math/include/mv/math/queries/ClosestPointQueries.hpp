#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

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
        [[nodiscard]] inline float PointDistanceSquared(
            const Point3f& first, const Point3f& second) noexcept
        {
            return LengthSquared(first - second);
        }

        [[nodiscard]] inline double DotDouble(const Vec3f& first,
                                              const Vec3f& second) noexcept
        {
            return static_cast<double>(first.X()) *
                       static_cast<double>(second.X()) +
                   static_cast<double>(first.Y()) *
                       static_cast<double>(second.Y()) +
                   static_cast<double>(first.Z()) *
                       static_cast<double>(second.Z());
        }

        [[nodiscard]] inline PointTriangleClosest3f MakePointTriangleClosest(
            const Point3f& point,
            const Point3f& pointOnTriangle,
            const Vec3f& barycentric) noexcept
        {
            return PointTriangleClosest3f{
                pointOnTriangle, barycentric,
                PointDistanceSquared(point, pointOnTriangle)};
        }
    }  // namespace detail

    // Orthogonal projection specialized for a unit direction; equations and
    // parameter semantics follow Geometric Tools DistPointLine.h (BSL-1.0).
    [[nodiscard]] inline PointLineClosest3f ClosestPoints(
        const Point3f& point, const Line3f& line) noexcept
    {
        const float parameter =
            Dot(line.Direction().Vector(), point - line.Origin());
        const Point3f pointOnLine = line.PointAt(parameter);
        return PointLineClosest3f{
            pointOnLine, parameter,
            detail::PointDistanceSquared(point, pointOnLine)};
    }

    // The line projection above, clamped to the ray domain [0,+infinity),
    // follows Geometric Tools DistPointRay.h (BSL-1.0).
    [[nodiscard]] inline PointRayClosest3f ClosestPoints(
        const Point3f& point, const Ray3f& ray) noexcept
    {
        const float parameter =
            std::max(Dot(ray.Direction().Vector(), point - ray.Origin()), 0.0F);
        const Point3f pointOnRay = ray.PointAt(parameter);
        return PointRayClosest3f{
            pointOnRay, parameter,
            detail::PointDistanceSquared(point, pointOnRay)};
    }

    // Endpoint tests and the interior projection follow Geometric Tools
    // DistPointSegment.h (BSL-1.0); a zero-length segment returns its start.
    [[nodiscard]] inline PointSegmentClosest3f ClosestPoints(
        const Point3f& point, const Segment3f& segment) noexcept
    {
        const Vec3f displacement = segment.Displacement();
        const float lengthSquared = LengthSquared(displacement);
        float fraction = 0.0F;
        if (lengthSquared > 0.0F)
        {
            fraction = std::clamp(
                Dot(point - segment.Start(), displacement) / lengthSquared,
                0.0F, 1.0F);
        }
        const Point3f pointOnSegment = segment.PointAtFraction(fraction);
        return PointSegmentClosest3f{
            pointOnSegment, fraction,
            detail::PointDistanceSquared(point, pointOnSegment)};
    }

    // Unit-normal orthogonal projection; see Ericson, Real-Time Collision
    // Detection (2005), section 5.1.1.
    [[nodiscard]] inline PointPlaneClosest3f ClosestPoints(
        const Point3f& point, const Plane3f& plane) noexcept
    {
        const float signedDistance = plane.SignedDistance(point);
        return PointPlaneClosest3f{
            point - plane.Normal().Vector() * signedDistance, signedDistance,
            signedDistance * signedDistance};
    }

    // Voronoi-region point/triangle query from Ericson, Real-Time Collision
    // Detection (2005), section 5.1.5, cross-checked against Eberly,
    // "Distance Between Point and Triangle in 3D" (1999, CC BY 4.0).
    // Move resolves degenerate triangles by testing their three edges.
    [[nodiscard]] inline PointTriangleClosest3f ClosestPoints(
        const Point3f& point, const Triangle3f& triangle) noexcept
    {
        const Point3f& first = triangle.First();
        const Point3f& second = triangle.Second();
        const Point3f& third = triangle.Third();
        const Vec3f edge01 = second - first;
        const Vec3f edge02 = third - first;

        if (!(LengthSquared(Cross(edge01, edge02)) > 0.0F))
        {
            const PointSegmentClosest3f edge01Closest =
                ClosestPoints(point, Segment3f(first, second));
            PointTriangleClosest3f closest = detail::MakePointTriangleClosest(
                point, edge01Closest.PointOnSegment,
                Vec3f(1.0F - edge01Closest.SegmentFraction,
                      edge01Closest.SegmentFraction, 0.0F));

            const PointSegmentClosest3f edge02Closest =
                ClosestPoints(point, Segment3f(first, third));
            if (edge02Closest.SquaredDistance < closest.SquaredDistance)
            {
                closest = detail::MakePointTriangleClosest(
                    point, edge02Closest.PointOnSegment,
                    Vec3f(1.0F - edge02Closest.SegmentFraction, 0.0F,
                          edge02Closest.SegmentFraction));
            }

            const PointSegmentClosest3f edge12Closest =
                ClosestPoints(point, Segment3f(second, third));
            if (edge12Closest.SquaredDistance < closest.SquaredDistance)
            {
                closest = detail::MakePointTriangleClosest(
                    point, edge12Closest.PointOnSegment,
                    Vec3f(0.0F, 1.0F - edge12Closest.SegmentFraction,
                          edge12Closest.SegmentFraction));
            }
            return closest;
        }

        const Vec3f fromFirst = point - first;
        const float firstAlong01 = Dot(edge01, fromFirst);
        const float firstAlong02 = Dot(edge02, fromFirst);
        if (firstAlong01 <= 0.0F && firstAlong02 <= 0.0F)
        {
            return detail::MakePointTriangleClosest(point, first,
                                                    Vec3f(1.0F, 0.0F, 0.0F));
        }

        const Vec3f fromSecond = point - second;
        const float secondAlong01 = Dot(edge01, fromSecond);
        const float secondAlong02 = Dot(edge02, fromSecond);
        if (secondAlong01 >= 0.0F && secondAlong02 <= secondAlong01)
        {
            return detail::MakePointTriangleClosest(point, second,
                                                    Vec3f(0.0F, 1.0F, 0.0F));
        }

        const float edge01Region =
            firstAlong01 * secondAlong02 - secondAlong01 * firstAlong02;
        if (edge01Region <= 0.0F && firstAlong01 >= 0.0F &&
            secondAlong01 <= 0.0F)
        {
            const float fraction =
                firstAlong01 / (firstAlong01 - secondAlong01);
            return detail::MakePointTriangleClosest(
                point, first + edge01 * fraction,
                Vec3f(1.0F - fraction, fraction, 0.0F));
        }

        const Vec3f fromThird = point - third;
        const float thirdAlong01 = Dot(edge01, fromThird);
        const float thirdAlong02 = Dot(edge02, fromThird);
        if (thirdAlong02 >= 0.0F && thirdAlong01 <= thirdAlong02)
        {
            return detail::MakePointTriangleClosest(point, third,
                                                    Vec3f(0.0F, 0.0F, 1.0F));
        }

        const float edge02Region =
            thirdAlong01 * firstAlong02 - firstAlong01 * thirdAlong02;
        if (edge02Region <= 0.0F && firstAlong02 >= 0.0F &&
            thirdAlong02 <= 0.0F)
        {
            const float fraction = firstAlong02 / (firstAlong02 - thirdAlong02);
            return detail::MakePointTriangleClosest(
                point, first + edge02 * fraction,
                Vec3f(1.0F - fraction, 0.0F, fraction));
        }

        const float edge12Region =
            secondAlong01 * thirdAlong02 - thirdAlong01 * secondAlong02;
        const float secondTowardThird = secondAlong02 - secondAlong01;
        const float thirdTowardSecond = thirdAlong01 - thirdAlong02;
        if (edge12Region <= 0.0F && secondTowardThird >= 0.0F &&
            thirdTowardSecond >= 0.0F)
        {
            const float fraction =
                secondTowardThird / (secondTowardThird + thirdTowardSecond);
            return detail::MakePointTriangleClosest(
                point, second + (third - second) * fraction,
                Vec3f(0.0F, 1.0F - fraction, fraction));
        }

        const float reciprocalSum =
            1.0F / (edge12Region + edge02Region + edge01Region);
        const float secondWeight = edge02Region * reciprocalSum;
        const float thirdWeight = edge01Region * reciprocalSum;
        return detail::MakePointTriangleClosest(
            point, first + edge01 * secondWeight + edge02 * thirdWeight,
            Vec3f(1.0F - secondWeight - thirdWeight, secondWeight,
                  thirdWeight));
    }

    // Component clamping is the point/AABB distance construction in Ericson,
    // Real-Time Collision Detection (2005), section 5.1.3. Empty boxes and
    // non-finite query points preserve Aabb3f's existing fallible contract.
    [[nodiscard]] inline std::optional<PointAabbClosest3f> TryClosestPoints(
        const Point3f& point, const Aabb3f& box) noexcept
    {
        const std::optional<Point3f> closest = box.TryClosestPoint(point);
        if (!closest)
        {
            return std::nullopt;
        }
        return PointAabbClosest3f{
            *closest, detail::PointDistanceSquared(point, *closest)};
    }

    // Radial projection is the solid-sphere specialization of Ericson,
    // Real-Time Collision Detection (2005), section 5.1.4. A contained point
    // is already the nearest member of the bounding volume and has distance 0.
    [[nodiscard]] inline PointSphereClosest3f ClosestPoints(
        const Point3f& point, const Sphere3f& sphere) noexcept
    {
        const Point3f center = sphere.Center();
        const Vec3f offset = point - center;
        const float squaredCenterDistance = LengthSquared(offset);
        const float squaredRadius = sphere.Radius() * sphere.Radius();
        if (squaredCenterDistance <= squaredRadius)
        {
            return PointSphereClosest3f{point, 0.0F};
        }

        const float scale = sphere.Radius() / std::sqrt(squaredCenterDistance);
        const Point3f pointInSphere = center + offset * scale;
        return PointSphereClosest3f{
            pointInSphere, detail::PointDistanceSquared(point, pointInSphere)};
    }

    // A capsule is a segment swept by a sphere (Ericson, Real-Time Collision
    // Detection, 2005, sections 4.5 and 4.5.1). Project to the center line,
    // then apply the same radial solid-volume rule as point/sphere.
    [[nodiscard]] inline PointCapsuleClosest3f ClosestPoints(
        const Point3f& point, const Capsule3f& capsule) noexcept
    {
        const PointSegmentClosest3f centerLineClosest =
            ClosestPoints(point, capsule.CenterLine());
        if (centerLineClosest.SquaredDistance <=
            capsule.Radius() * capsule.Radius())
        {
            return PointCapsuleClosest3f{
                point, centerLineClosest.SegmentFraction, 0.0F};
        }

        const Vec3f radialOffset = point - centerLineClosest.PointOnSegment;
        const float scale =
            capsule.Radius() / std::sqrt(centerLineClosest.SquaredDistance);
        const Point3f pointInCapsule =
            centerLineClosest.PointOnSegment + radialOffset * scale;
        return PointCapsuleClosest3f{
            pointInCapsule, centerLineClosest.SegmentFraction,
            detail::PointDistanceSquared(point, pointInCapsule)};
    }

    // ClosestPtSegmentSegment from Ericson, Real-Time Collision Detection
    // (2005), section 5.1.9. Double intermediates reduce cancellation for
    // nearly parallel float segments; degenerate segments remain valid.
    [[nodiscard]] inline SegmentSegmentClosest3f ClosestPoints(
        const Segment3f& first, const Segment3f& second) noexcept
    {
        const Vec3f firstDirection = first.Displacement();
        const Vec3f secondDirection = second.Displacement();
        const Vec3f startOffset = first.Start() - second.Start();
        const double firstLengthSquared =
            detail::DotDouble(firstDirection, firstDirection);
        const double secondLengthSquared =
            detail::DotDouble(secondDirection, secondDirection);
        const double secondProjection =
            detail::DotDouble(secondDirection, startOffset);

        double firstFraction = 0.0;
        double secondFraction = 0.0;
        if (firstLengthSquared == 0.0 && secondLengthSquared == 0.0)
        {
            // Both segments are points; the initialized fractions are final.
        }
        else if (firstLengthSquared == 0.0)
        {
            secondFraction =
                std::clamp(secondProjection / secondLengthSquared, 0.0, 1.0);
        }
        else
        {
            const double firstProjection =
                detail::DotDouble(firstDirection, startOffset);
            if (secondLengthSquared == 0.0)
            {
                firstFraction =
                    std::clamp(-firstProjection / firstLengthSquared, 0.0, 1.0);
            }
            else
            {
                const double directionsDot =
                    detail::DotDouble(firstDirection, secondDirection);
                const double denominator =
                    firstLengthSquared * secondLengthSquared -
                    directionsDot * directionsDot;
                if (denominator > 0.0)
                {
                    firstFraction =
                        std::clamp((directionsDot * secondProjection -
                                    firstProjection * secondLengthSquared) /
                                       denominator,
                                   0.0, 1.0);
                }

                secondFraction =
                    (directionsDot * firstFraction + secondProjection) /
                    secondLengthSquared;
                if (secondFraction < 0.0)
                {
                    secondFraction = 0.0;
                    firstFraction = std::clamp(
                        -firstProjection / firstLengthSquared, 0.0, 1.0);
                }
                else if (secondFraction > 1.0)
                {
                    secondFraction = 1.0;
                    firstFraction = std::clamp(
                        (directionsDot - firstProjection) / firstLengthSquared,
                        0.0, 1.0);
                }
            }
        }

        const float firstFloat = static_cast<float>(firstFraction);
        const float secondFloat = static_cast<float>(secondFraction);
        const Point3f pointOnFirst = first.PointAtFraction(firstFloat);
        const Point3f pointOnSecond = second.PointAtFraction(secondFloat);
        return SegmentSegmentClosest3f{
            pointOnFirst, pointOnSecond, firstFloat, secondFloat,
            detail::PointDistanceSquared(pointOnFirst, pointOnSecond)};
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Line3f& line) noexcept
    {
        return ClosestPoints(point, line).PointOnLine;
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Ray3f& ray) noexcept
    {
        return ClosestPoints(point, ray).PointOnRay;
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Segment3f& segment) noexcept
    {
        return ClosestPoints(point, segment).PointOnSegment;
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Plane3f& plane) noexcept
    {
        return ClosestPoints(point, plane).PointOnPlane;
    }

    [[nodiscard]] inline Point3f ClosestPoint(
        const Point3f& point, const Triangle3f& triangle) noexcept
    {
        return ClosestPoints(point, triangle).PointOnTriangle;
    }

    [[nodiscard]] inline std::optional<Point3f> TryClosestPoint(
        const Point3f& point, const Aabb3f& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<Point3f>(result->PointInAabb)
                      : std::nullopt;
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Sphere3f& sphere) noexcept
    {
        return ClosestPoints(point, sphere).PointInSphere;
    }

    [[nodiscard]] inline Point3f ClosestPoint(const Point3f& point,
                                              const Capsule3f& capsule) noexcept
    {
        return ClosestPoints(point, capsule).PointInCapsule;
    }

    [[nodiscard]] inline float DistanceSquared(const Point3f& point,
                                               const Line3f& line) noexcept
    {
        return ClosestPoints(point, line).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(const Point3f& point,
                                               const Ray3f& ray) noexcept
    {
        return ClosestPoints(point, ray).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(
        const Point3f& point, const Segment3f& segment) noexcept
    {
        return ClosestPoints(point, segment).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(const Point3f& point,
                                               const Plane3f& plane) noexcept
    {
        return ClosestPoints(point, plane).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(
        const Point3f& point, const Triangle3f& triangle) noexcept
    {
        return ClosestPoints(point, triangle).SquaredDistance;
    }

    [[nodiscard]] inline std::optional<float> TryDistanceSquared(
        const Point3f& point, const Aabb3f& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<float>(result->SquaredDistance)
                      : std::nullopt;
    }

    [[nodiscard]] inline float DistanceSquared(const Point3f& point,
                                               const Sphere3f& sphere) noexcept
    {
        return ClosestPoints(point, sphere).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(
        const Point3f& point, const Capsule3f& capsule) noexcept
    {
        return ClosestPoints(point, capsule).SquaredDistance;
    }

    [[nodiscard]] inline float DistanceSquared(const Segment3f& first,
                                               const Segment3f& second) noexcept
    {
        return ClosestPoints(first, second).SquaredDistance;
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Line3f& line) noexcept
    {
        return ClosestPoints(point, line).Distance();
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Ray3f& ray) noexcept
    {
        return ClosestPoints(point, ray).Distance();
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Segment3f& segment) noexcept
    {
        return ClosestPoints(point, segment).Distance();
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Plane3f& plane) noexcept
    {
        return ClosestPoints(point, plane).Distance();
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Triangle3f& triangle) noexcept
    {
        return ClosestPoints(point, triangle).Distance();
    }

    [[nodiscard]] inline std::optional<float> TryDistance(
        const Point3f& point, const Aabb3f& box) noexcept
    {
        const auto result = TryClosestPoints(point, box);
        return result ? std::optional<float>(result->Distance()) : std::nullopt;
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Sphere3f& sphere) noexcept
    {
        return ClosestPoints(point, sphere).Distance();
    }

    [[nodiscard]] inline float Distance(const Point3f& point,
                                        const Capsule3f& capsule) noexcept
    {
        return ClosestPoints(point, capsule).Distance();
    }

    [[nodiscard]] inline bool Contains(const Capsule3f& capsule,
                                       const Point3f& point) noexcept
    {
        return ClosestPoints(point, capsule).SquaredDistance == 0.0F;
    }

    [[nodiscard]] inline float Distance(const Segment3f& first,
                                        const Segment3f& second) noexcept
    {
        return ClosestPoints(first, second).Distance();
    }
}  // namespace mv::math
