#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include <mv/math/PhaseC.hpp>

namespace
{
    void Require(bool condition)
    {
        if (!condition)
        {
            std::abort();
        }
    }

    [[nodiscard]] bool NearlyEqual(float left,
                                   float right,
                                   float epsilon = 1.0e-5F)
    {
        return std::abs(left - right) <= epsilon;
    }

    [[nodiscard]] bool NearlyEqual(const mv::math::Vec3f& left,
                                   const mv::math::Vec3f& right,
                                   float epsilon = 1.0e-5F)
    {
        return NearlyEqual(left.X(), right.X(), epsilon) &&
               NearlyEqual(left.Y(), right.Y(), epsilon) &&
               NearlyEqual(left.Z(), right.Z(), epsilon);
    }

    [[nodiscard]] float Component(const mv::math::Point3f& point,
                                  std::size_t axis)
    {
        return axis == 0U ? point.X() : axis == 1U ? point.Y() : point.Z();
    }

    [[nodiscard]] float Component(const mv::math::Vec3f& vector,
                                  std::size_t axis)
    {
        return axis == 0U ? vector.X() : axis == 1U ? vector.Y() : vector.Z();
    }

    struct Double3
    {
        double X;
        double Y;
        double Z;
    };

    [[nodiscard]] Double3 ToDouble(const mv::math::Point3f& point)
    {
        return Double3{point.X(), point.Y(), point.Z()};
    }

    [[nodiscard]] Double3 operator+(Double3 left, Double3 right)
    {
        return Double3{left.X + right.X, left.Y + right.Y, left.Z + right.Z};
    }

    [[nodiscard]] Double3 operator-(Double3 left, Double3 right)
    {
        return Double3{left.X - right.X, left.Y - right.Y, left.Z - right.Z};
    }

    [[nodiscard]] Double3 operator*(Double3 value, double scale)
    {
        return Double3{value.X * scale, value.Y * scale, value.Z * scale};
    }

    [[nodiscard]] double Dot(Double3 left, Double3 right)
    {
        return left.X * right.X + left.Y * right.Y + left.Z * right.Z;
    }

    [[nodiscard]] Double3 Cross(Double3 left, Double3 right)
    {
        return Double3{left.Y * right.Z - left.Z * right.Y,
                       left.Z * right.X - left.X * right.Z,
                       left.X * right.Y - left.Y * right.X};
    }

    [[nodiscard]] Double3 ReferencePointSegment(Double3 point,
                                                Double3 start,
                                                Double3 end)
    {
        const Double3 displacement = end - start;
        const double lengthSquared = Dot(displacement, displacement);
        const double fraction =
            lengthSquared > 0.0
                ? std::clamp(Dot(point - start, displacement) / lengthSquared,
                             0.0, 1.0)
                : 0.0;
        return start + displacement * fraction;
    }

    // Independent projection-plus-three-edges construction described as the
    // straightforward baseline in Ericson, RTCD (2005), section 5.1.5.
    [[nodiscard]] Double3 ReferencePointTriangle(
        const mv::math::Point3f& point, const mv::math::Triangle3f& triangle)
    {
        const Double3 query = ToDouble(point);
        const Double3 first = ToDouble(triangle.First());
        const Double3 second = ToDouble(triangle.Second());
        const Double3 third = ToDouble(triangle.Third());
        const Double3 edge01 = second - first;
        const Double3 edge02 = third - first;
        const Double3 normal = Cross(edge01, edge02);
        const double normalLengthSquared = Dot(normal, normal);

        if (normalLengthSquared > 0.0)
        {
            const Double3 projected =
                query -
                normal * (Dot(query - first, normal) / normalLengthSquared);
            const Double3 fromFirst = projected - first;
            const double edge01Squared = Dot(edge01, edge01);
            const double edge02Squared = Dot(edge02, edge02);
            const double edgesDot = Dot(edge01, edge02);
            const double pointDot01 = Dot(fromFirst, edge01);
            const double pointDot02 = Dot(fromFirst, edge02);
            const double denominator =
                edge01Squared * edge02Squared - edgesDot * edgesDot;
            const double secondWeight =
                (edge02Squared * pointDot01 - edgesDot * pointDot02) /
                denominator;
            const double thirdWeight =
                (edge01Squared * pointDot02 - edgesDot * pointDot01) /
                denominator;
            const double firstWeight = 1.0 - secondWeight - thirdWeight;
            if (firstWeight >= 0.0 && secondWeight >= 0.0 && thirdWeight >= 0.0)
            {
                return projected;
            }
        }

        Double3 closest = ReferencePointSegment(query, first, second);
        double closestSquared = Dot(query - closest, query - closest);
        const Double3 edge02Closest =
            ReferencePointSegment(query, first, third);
        const double edge02Squared =
            Dot(query - edge02Closest, query - edge02Closest);
        if (edge02Squared < closestSquared)
        {
            closest = edge02Closest;
            closestSquared = edge02Squared;
        }
        const Double3 edge12Closest =
            ReferencePointSegment(query, second, third);
        if (Dot(query - edge12Closest, query - edge12Closest) < closestSquared)
        {
            closest = edge12Closest;
        }
        return closest;
    }

    [[nodiscard]] bool ReferenceRayAabb(const mv::math::Ray3f& ray,
                                        const mv::math::Aabb3f& box)
    {
        if (!ray.IsFinite() || box.IsEmpty())
        {
            return false;
        }

        double entry = 0.0;
        double exit = std::numeric_limits<double>::infinity();
        for (std::size_t axis = 0U; axis < 3U; ++axis)
        {
            const double origin = Component(ray.Origin(), axis);
            const double direction = Component(ray.Direction().Vector(), axis);
            const double minimum = Component(box.Minimum(), axis);
            const double maximum = Component(box.Maximum(), axis);
            if (direction == 0.0)
            {
                if (origin < minimum || origin > maximum)
                {
                    return false;
                }
                continue;
            }

            double nearDistance = (minimum - origin) / direction;
            double farDistance = (maximum - origin) / direction;
            if (nearDistance > farDistance)
            {
                std::swap(nearDistance, farDistance);
            }
            entry = std::max(entry, nearDistance);
            exit = std::min(exit, farDistance);
            if (entry > exit)
            {
                return false;
            }
        }
        return exit >= 0.0;
    }

    [[nodiscard]] float NextValue(std::uint32_t& state)
    {
        state = state * 1664525U + 1013904223U;
        const float unit =
            static_cast<float>((state >> 8U) & 0xFFFFU) / 65535.0F;
        return unit * 20.0F - 10.0F;
    }

    void CheckRayAndPlane()
    {
        using namespace mv::math;

        const Ray3f ray(Point3f(1.0F, 2.0F, 3.0F), Direction3f::AxisZ());
        Require(ray.Origin() == Point3f(1.0F, 2.0F, 3.0F));
        Require(ray.Direction() == Direction3f::AxisZ());
        Require(ray.PointAt(4.0F) == Point3f(1.0F, 2.0F, 7.0F));
        Require(ray.IsFinite());

        const auto plane = Plane3f::TryFromPointNormal(
            Point3f(0.0F, 0.0F, 2.0F), Normal3f::AxisZ());
        Require(plane.has_value());
        Require(NearlyEqual(plane->Offset(), -2.0F));
        Require(NearlyEqual(plane->SignedDistance(Point3f(0.0F, 0.0F, 5.0F)),
                            3.0F));
        Require(plane->Classify(Point3f(0.0F, 0.0F, 5.0F)) == PlaneSide::Front);
        Require(plane->Classify(Point3f(0.0F, 0.0F, 1.0F)) == PlaneSide::Back);
        Require(plane->Classify(Point3f(0.0F, 0.0F, 2.001F), 0.01F) ==
                PlaneSide::Intersecting);
        Require(
            NearlyEqual(plane->ClosestPoint(Point3f(1.0F, 2.0F, 8.0F)).Vector(),
                        Vec3f(1.0F, 2.0F, 2.0F)));
        Require(plane->Flipped().Normal() == -Normal3f::AxisZ());
        Require(NearlyEqual(plane->Flipped().Offset(), 2.0F));

        const auto threePointPlane = Plane3f::TryFromPoints(
            Point3f(0.0F, 0.0F, 0.0F), Point3f(1.0F, 0.0F, 0.0F),
            Point3f(0.0F, 1.0F, 0.0F));
        Require(threePointPlane.has_value());
        Require(threePointPlane->Normal() == Normal3f::AxisZ());
        Require(!Plane3f::TryFromPoints(Point3f(), Point3f(), Point3f()));
        Require(!Plane3f::TryFromNormalAndOffset(
            Normal3f::AxisX(), std::numeric_limits<float>::infinity()));
    }

    void CheckLineAndSegment()
    {
        using namespace mv::math;

        const Line3f line(Point3f(1.0F, 2.0F, 3.0F), Direction3f::AxisX());
        Require(line.Origin() == Point3f(1.0F, 2.0F, 3.0F));
        Require(line.Direction() == Direction3f::AxisX());
        Require(line.PointAt(-2.0F) == Point3f(-1.0F, 2.0F, 3.0F));
        Require(line.IsFinite());

        const Segment3f segment(Point3f(), Point3f(0.0F, 0.0F, 4.0F));
        Require(segment.Start() == Point3f());
        Require(segment.End() == Point3f(0.0F, 0.0F, 4.0F));
        Require(segment.Displacement() == Vec3f(0.0F, 0.0F, 4.0F));
        Require(segment.LengthSquared() == 16.0F);
        Require(segment.Length() == 4.0F);
        Require(!segment.IsDegenerate());
        Require(segment.TryDirection() == Direction3f::AxisZ());
        Require(segment.PointAtFraction(0.25F) == Point3f(0.0F, 0.0F, 1.0F));
        Require(segment.PointAtFraction(2.0F) == Point3f(0.0F, 0.0F, 8.0F));
        Require(segment.IsFinite());

        const Segment3f degenerate(Point3f(1.0F, 2.0F, 3.0F),
                                   Point3f(1.0F, 2.0F, 3.0F));
        Require(degenerate.IsDegenerate());
        Require(!degenerate.TryDirection());
        Require(degenerate.PointAtFraction(0.75F) == degenerate.Start());
    }

    void CheckTriangleAndSphere()
    {
        using namespace mv::math;

        const Triangle3f triangle(Point3f(0.0F, 0.0F, 0.0F),
                                  Point3f(2.0F, 0.0F, 0.0F),
                                  Point3f(0.0F, 2.0F, 0.0F));
        Require(triangle.IsFinite());
        Require(triangle.Edge01() == Vec3f(2.0F, 0.0F, 0.0F));
        Require(triangle.Edge02() == Vec3f(0.0F, 2.0F, 0.0F));
        Require(NearlyEqual(triangle.Centroid().Vector(),
                            Vec3f(2.0F / 3.0F, 2.0F / 3.0F, 0.0F)));
        Require(NearlyEqual(triangle.Area(), 2.0F));
        Require(triangle.TryNormal() == Normal3f::AxisZ());

        const Triangle3f degenerate(Point3f(), Point3f(1.0F, 0.0F, 0.0F),
                                    Point3f(2.0F, 0.0F, 0.0F));
        Require(!degenerate.TryNormal());
        Require(NearlyEqual(degenerate.Area(), 0.0F));

        Require(!Sphere3f::TryFromCenterRadius(Point3f(), -1.0F));
        Require(!Sphere3f::TryFromCenterRadius(
            Point3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F), 1.0F));
        const auto sphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.0F, 2.0F, 3.0F), 2.0F);
        Require(sphere.has_value());
        Require(sphere->Center() == Point3f(1.0F, 2.0F, 3.0F));
        Require(sphere->Radius() == 2.0F);
        Require(sphere->Diameter() == 4.0F);
        Require(sphere->Contains(Point3f(3.0F, 2.0F, 3.0F)));
        Require(!sphere->Contains(Point3f(3.01F, 2.0F, 3.0F)));
    }

    void CheckAabb()
    {
        using namespace mv::math;

        const Aabb3f empty;
        Require(empty.IsEmpty());
        Require(empty.IsValid());
        Require(!empty.TryCenter());
        Require(!empty.TrySize());
        Require(!empty.TryCorners());
        Require(empty.Volume() == 0.0F);
        Require(empty.SurfaceArea() == 0.0F);

        Require(!Aabb3f::TryFromMinMax(Point3f(1.0F, 0.0F, 0.0F),
                                       Point3f(0.0F, 1.0F, 1.0F)));
        Require(!Aabb3f::TryFromCenterHalfExtents(Point3f(),
                                                  Vec3f(1.0F, -1.0F, 1.0F)));

        const auto box = Aabb3f::TryFromCenterHalfExtents(
            Point3f(1.0F, 2.0F, 3.0F), Vec3f(1.0F, 2.0F, 3.0F));
        Require(box.has_value());
        Require(box->Minimum() == Point3f(0.0F, 0.0F, 0.0F));
        Require(box->Maximum() == Point3f(2.0F, 4.0F, 6.0F));
        Require(box->TryCenter() == Point3f(1.0F, 2.0F, 3.0F));
        Require(box->TrySize() == Vec3f(2.0F, 4.0F, 6.0F));
        Require(box->TryHalfExtents() == Vec3f(1.0F, 2.0F, 3.0F));
        Require(box->Contains(box->Minimum()));
        Require(box->Contains(box->Maximum()));
        Require(!box->Contains(Point3f(2.01F, 2.0F, 3.0F)));
        Require(box->Volume() == 48.0F);
        Require(box->SurfaceArea() == 88.0F);
        Require(box->TryCorners()->size() == 8U);
        Require(box->TryClosestPoint(Point3f(-1.0F, 5.0F, 2.0F)) ==
                Point3f(0.0F, 4.0F, 2.0F));

        const auto touching = Aabb3f::TryFromMinMax(Point3f(2.0F, 1.0F, 1.0F),
                                                    Point3f(3.0F, 2.0F, 2.0F));
        Require(touching.has_value());
        Require(box->Intersects(*touching));
        const Aabb3f overlap = box->Intersection(*touching);
        Require(!overlap.IsEmpty());
        Require(overlap.Minimum().X() == 2.0F);
        Require(overlap.Maximum().X() == 2.0F);

        const auto disjoint = Aabb3f::TryFromMinMax(Point3f(3.0F, 0.0F, 0.0F),
                                                    Point3f(4.0F, 1.0F, 1.0F));
        Require(disjoint.has_value());
        Require(!box->Intersects(*disjoint));
        Require(box->Intersection(*disjoint).IsEmpty());
        Require(box->Merged(*disjoint).Maximum() == Point3f(4.0F, 4.0F, 6.0F));
        Require(empty.Merged(*box) == *box);

        const std::array points = {Point3f(-2.0F, 3.0F, 1.0F),
                                   Point3f(4.0F, -1.0F, 6.0F)};
        const auto fromPoints = Aabb3f::TryFromPoints(points);
        Require(fromPoints.has_value());
        Require(fromPoints->Minimum() == Point3f(-2.0F, -1.0F, 1.0F));
        Require(fromPoints->Maximum() == Point3f(4.0F, 3.0F, 6.0F));
        Require(Aabb3f::TryFromPoints(std::span<const Point3f>{})->IsEmpty());
        Require(!box->TryExpanded(
            Point3f(std::numeric_limits<float>::quiet_NaN(), 0.0F, 0.0F)));
    }

    void CheckLinearQueries()
    {
        using namespace mv::math;

        const auto plane =
            Plane3f::TryFromPointNormal(Point3f(), Normal3f::AxisZ());
        Require(plane.has_value());
        const Ray3f frontRay(Point3f(0.0F, 0.0F, 2.0F), -Direction3f::AxisZ());
        const auto planeHit = Intersect(frontRay, *plane);
        Require(planeHit.has_value());
        Require(planeHit->Distance == 2.0F);
        Require(planeHit->Point == Point3f());
        Require(planeHit->Normal == Normal3f::AxisZ());
        Require(planeHit->Face == FaceOrientation::Front);

        const Ray3f backRay(Point3f(0.0F, 0.0F, -2.0F), Direction3f::AxisZ());
        Require(Intersect(backRay, *plane)->Face == FaceOrientation::Back);
        Require(!Intersect(
            Ray3f(Point3f(0.0F, 0.0F, 1.0F), Direction3f::AxisX()), *plane));
        Require(!Intersect(Ray3f(Point3f(), Direction3f::AxisX()), *plane));

        const Triangle3f triangle(Point3f(0.0F, 0.0F, 0.0F),
                                  Point3f(1.0F, 0.0F, 0.0F),
                                  Point3f(0.0F, 1.0F, 0.0F));
        const Ray3f triangleRay(Point3f(0.25F, 0.25F, 1.0F),
                                -Direction3f::AxisZ());
        const auto triangleHit = Intersect(triangleRay, triangle);
        Require(triangleHit.has_value());
        Require(triangleHit->Distance == 1.0F);
        Require(triangleHit->Point == Point3f(0.25F, 0.25F, 0.0F));
        Require(triangleHit->GeometricNormal == Normal3f::AxisZ());
        Require(
            NearlyEqual(triangleHit->Barycentric, Vec3f(0.5F, 0.25F, 0.25F)));
        Require(triangleHit->Face == FaceOrientation::Front);

        const Ray3f triangleBackRay(Point3f(0.25F, 0.25F, -1.0F),
                                    Direction3f::AxisZ());
        Require(Intersect(triangleBackRay, triangle)->Face ==
                FaceOrientation::Back);
        Require(!Intersect(triangleBackRay, triangle,
                           RayTriangleOptions{BackFaceMode::Cull}));
        Require(!Intersect(
            Ray3f(Point3f(2.0F, 2.0F, 1.0F), -Direction3f::AxisZ()), triangle));
        Require(!Intersect(triangleRay,
                           Triangle3f(Point3f(), Point3f(), Point3f())));
    }

    void CheckClosestPointQueries()
    {
        using namespace mv::math;

        const Point3f point(2.0F, 3.0F, 0.0F);
        const Line3f line(Point3f(), Direction3f::AxisX());
        const PointLineClosest3f lineClosest = ClosestPoints(point, line);
        Require(lineClosest.PointOnLine == Point3f(2.0F, 0.0F, 0.0F));
        Require(lineClosest.LineDistance == 2.0F);
        Require(lineClosest.SquaredDistance == 9.0F);
        Require(lineClosest.Distance() == 3.0F);
        Require(ClosestPoint(point, line) == lineClosest.PointOnLine);
        Require(DistanceSquared(point, line) == 9.0F);
        Require(Distance(point, line) == 3.0F);

        const PointLineClosest3f negativeLineClosest =
            ClosestPoints(Point3f(-2.0F, 1.0F, 0.0F), line);
        Require(negativeLineClosest.LineDistance == -2.0F);
        Require(negativeLineClosest.PointOnLine == Point3f(-2.0F, 0.0F, 0.0F));

        const Ray3f ray(Point3f(), Direction3f::AxisX());
        const PointRayClosest3f rayClosest =
            ClosestPoints(Point3f(-2.0F, 1.0F, 0.0F), ray);
        Require(rayClosest.RayDistance == 0.0F);
        Require(rayClosest.PointOnRay == Point3f());
        Require(rayClosest.SquaredDistance == 5.0F);

        const Segment3f segment(Point3f(), Point3f(4.0F, 0.0F, 0.0F));
        const PointSegmentClosest3f segmentClosest =
            ClosestPoints(point, segment);
        Require(segmentClosest.PointOnSegment == Point3f(2.0F, 0.0F, 0.0F));
        Require(segmentClosest.SegmentFraction == 0.5F);
        Require(segmentClosest.SquaredDistance == 9.0F);
        Require(
            ClosestPoints(Point3f(6.0F, 1.0F, 0.0F), segment).SegmentFraction ==
            1.0F);

        const Segment3f pointSegment(Point3f(1.0F, 2.0F, 3.0F),
                                     Point3f(1.0F, 2.0F, 3.0F));
        const PointSegmentClosest3f pointSegmentClosest =
            ClosestPoints(Point3f(4.0F, 6.0F, 3.0F), pointSegment);
        Require(pointSegmentClosest.PointOnSegment == pointSegment.Start());
        Require(pointSegmentClosest.SegmentFraction == 0.0F);
        Require(pointSegmentClosest.SquaredDistance == 25.0F);

        const auto plane =
            Plane3f::TryFromPointNormal(Point3f(), Normal3f::AxisZ());
        Require(plane.has_value());
        const PointPlaneClosest3f planeClosest =
            ClosestPoints(Point3f(1.0F, 2.0F, -3.0F), *plane);
        Require(planeClosest.PointOnPlane == Point3f(1.0F, 2.0F, 0.0F));
        Require(planeClosest.SignedDistance == -3.0F);
        Require(planeClosest.SquaredDistance == 9.0F);

        const Triangle3f triangle(Point3f(0.0F, 0.0F, 0.0F),
                                  Point3f(2.0F, 0.0F, 0.0F),
                                  Point3f(0.0F, 2.0F, 0.0F));
        const PointTriangleClosest3f faceClosest =
            ClosestPoints(Point3f(0.5F, 0.5F, 2.0F), triangle);
        Require(faceClosest.PointOnTriangle == Point3f(0.5F, 0.5F, 0.0F));
        Require(
            NearlyEqual(faceClosest.Barycentric, Vec3f(0.5F, 0.25F, 0.25F)));
        Require(faceClosest.SquaredDistance == 4.0F);

        const PointTriangleClosest3f firstClosest =
            ClosestPoints(Point3f(-1.0F, -1.0F, 0.0F), triangle);
        Require(firstClosest.PointOnTriangle == triangle.First());
        Require(firstClosest.Barycentric == Vec3f(1.0F, 0.0F, 0.0F));

        const PointTriangleClosest3f edge01Closest =
            ClosestPoints(Point3f(0.5F, -1.0F, 0.0F), triangle);
        Require(edge01Closest.PointOnTriangle == Point3f(0.5F, 0.0F, 0.0F));
        Require(
            NearlyEqual(edge01Closest.Barycentric, Vec3f(0.75F, 0.25F, 0.0F)));

        const PointTriangleClosest3f edge12Closest =
            ClosestPoints(Point3f(2.0F, 2.0F, 0.0F), triangle);
        Require(edge12Closest.PointOnTriangle == Point3f(1.0F, 1.0F, 0.0F));
        Require(
            NearlyEqual(edge12Closest.Barycentric, Vec3f(0.0F, 0.5F, 0.5F)));

        const Triangle3f lineTriangle(Point3f(), Point3f(2.0F, 0.0F, 0.0F),
                                      Point3f(1.0F, 0.0F, 0.0F));
        const PointTriangleClosest3f lineTriangleClosest =
            ClosestPoints(Point3f(1.0F, 2.0F, 0.0F), lineTriangle);
        Require(lineTriangleClosest.PointOnTriangle ==
                Point3f(1.0F, 0.0F, 0.0F));
        Require(NearlyEqual(lineTriangleClosest.Barycentric,
                            Vec3f(0.5F, 0.5F, 0.0F)));

        const Triangle3f pointTriangle(Point3f(3.0F, 4.0F, 5.0F),
                                       Point3f(3.0F, 4.0F, 5.0F),
                                       Point3f(3.0F, 4.0F, 5.0F));
        const PointTriangleClosest3f pointTriangleClosest =
            ClosestPoints(Point3f(), pointTriangle);
        Require(pointTriangleClosest.PointOnTriangle == pointTriangle.First());
        Require(pointTriangleClosest.Barycentric == Vec3f(1.0F, 0.0F, 0.0F));
        Require(pointTriangleClosest.SquaredDistance == 50.0F);
    }

    void CheckBoundsQueries()
    {
        using namespace mv::math;

        const auto sphere = Sphere3f::TryFromCenterRadius(Point3f(), 1.0F);
        Require(sphere.has_value());
        const Ray3f sphereRay(Point3f(-3.0F, 0.0F, 0.0F), Direction3f::AxisX());
        const auto sphereHit = Intersect(sphereRay, *sphere);
        Require(sphereHit.has_value());
        Require(sphereHit->EntryDistance == 2.0F);
        Require(sphereHit->ExitDistance == 4.0F);
        Require(!sphereHit->StartsInside);
        Require(sphereHit->EntryNormal == -Normal3f::AxisX());
        Require(sphereHit->ExitNormal == Normal3f::AxisX());

        const auto insideSphereHit =
            Intersect(Ray3f(Point3f(), Direction3f::AxisX()), *sphere);
        Require(insideSphereHit.has_value());
        Require(insideSphereHit->EntryDistance == 0.0F);
        Require(insideSphereHit->ExitDistance == 1.0F);
        Require(insideSphereHit->StartsInside);
        Require(!insideSphereHit->EntryNormal);
        Require(insideSphereHit->ExitNormal == Normal3f::AxisX());

        const auto pointSphere = Sphere3f::TryFromCenterRadius(Point3f(), 0.0F);
        Require(pointSphere.has_value());
        const auto pointHit =
            Intersect(Ray3f(Point3f(-1.0F, 0.0F, 0.0F), Direction3f::AxisX()),
                      *pointSphere);
        Require(pointHit.has_value());
        Require(!pointHit->EntryNormal);
        Require(!pointHit->ExitNormal);
        Require(!Intersect(
            Ray3f(Point3f(-3.0F, 2.0F, 0.0F), Direction3f::AxisX()), *sphere));

        const auto box = Aabb3f::TryFromMinMax(Point3f(-1.0F, -1.0F, -1.0F),
                                               Point3f(1.0F, 1.0F, 1.0F));
        Require(box.has_value());
        const Ray3f boxRay(Point3f(-3.0F, 0.0F, 0.0F), Direction3f::AxisX());
        const auto boxHit = Intersect(boxRay, *box);
        Require(boxHit.has_value());
        Require(boxHit->EntryDistance == 2.0F);
        Require(boxHit->ExitDistance == 4.0F);
        Require(!boxHit->StartsInside);
        Require(boxHit->EntryNormal == -Normal3f::AxisX());
        Require(boxHit->ExitNormal == Normal3f::AxisX());
        Require(Intersect(PreparedRay3f(boxRay), *box) == boxHit);

        const auto insideBoxHit =
            Intersect(Ray3f(Point3f(), Direction3f::AxisY()), *box);
        Require(insideBoxHit.has_value());
        Require(insideBoxHit->EntryDistance == 0.0F);
        Require(insideBoxHit->ExitDistance == 1.0F);
        Require(insideBoxHit->StartsInside);
        Require(!insideBoxHit->EntryNormal);
        Require(insideBoxHit->ExitNormal == Normal3f::AxisY());

        Require(
            Intersect(Ray3f(Point3f(-3.0F, 1.0F, 0.0F), Direction3f::AxisX()),
                      *box)
                .has_value());
        Require(!Intersect(
            Ray3f(Point3f(-3.0F, 2.0F, 0.0F), Direction3f::AxisX()), *box));
        Require(!Intersect(boxRay, Aabb3f::Empty()));

        const auto touchingSphere =
            Sphere3f::TryFromCenterRadius(Point3f(2.0F, 0.0F, 0.0F), 1.0F);
        Require(touchingSphere.has_value());
        Require(Intersects(*sphere, *touchingSphere));
        Require(Intersects(*box, *box));
        Require(Intersects(*sphere, *box));
        const auto separatedSphere =
            Sphere3f::TryFromCenterRadius(Point3f(3.0F, 3.0F, 3.0F), 1.0F);
        Require(separatedSphere.has_value());
        Require(!Intersects(*separatedSphere, *box));
        Require(!Intersects(Aabb3f::Empty(), *sphere));

        const auto boundarySphereHit = Intersect(
            Ray3f(Point3f(1.0F, 0.0F, 0.0F), Direction3f::AxisX()), *sphere);
        Require(boundarySphereHit.has_value());
        Require(!boundarySphereHit->StartsInside);
        Require(boundarySphereHit->EntryDistance == 0.0F);
        Require(boundarySphereHit->EntryNormal == Normal3f::AxisX());
    }

    void CheckAabbReferenceParity()
    {
        using namespace mv::math;

        std::uint32_t state = 0xC001CAFEU;
        for (std::size_t index = 0U; index < 1024U; ++index)
        {
            const Point3f center(NextValue(state), NextValue(state),
                                 NextValue(state));
            const Vec3f halfExtents(std::abs(NextValue(state)) + 0.01F,
                                    std::abs(NextValue(state)) + 0.01F,
                                    std::abs(NextValue(state)) + 0.01F);
            const auto box =
                Aabb3f::TryFromCenterHalfExtents(center, halfExtents);
            Require(box.has_value());

            const Vec3f directionValue(NextValue(state), NextValue(state),
                                       NextValue(state));
            const auto direction = Direction3f::TryFrom(directionValue);
            Require(direction.has_value());
            const Ray3f ray(
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                *direction);

            const bool reference = ReferenceRayAabb(ray, *box);
            Require(Intersects(ray, *box) == reference);
            Require(Intersects(PreparedRay3f(ray), *box) == reference);
        }
    }

    void CheckTriangleClosestReferenceParity()
    {
        using namespace mv::math;

        std::uint32_t state = 0x51A7C105U;
        for (std::size_t index = 0U; index < 1024U; ++index)
        {
            const Triangle3f triangle(
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                Point3f(NextValue(state), NextValue(state), NextValue(state)));
            const Point3f point(NextValue(state), NextValue(state),
                                NextValue(state));
            const PointTriangleClosest3f result =
                ClosestPoints(point, triangle);
            const Double3 reference = ReferencePointTriangle(point, triangle);
            Require(NearlyEqual(result.PointOnTriangle.Vector(),
                                Vec3f(static_cast<float>(reference.X),
                                      static_cast<float>(reference.Y),
                                      static_cast<float>(reference.Z)),
                                2.0e-4F));
            Require(NearlyEqual(result.Barycentric.X() +
                                    result.Barycentric.Y() +
                                    result.Barycentric.Z(),
                                1.0F, 2.0e-4F));
            const Point3f reconstructed = Point3f::FromVector(
                triangle.First().Vector() * result.Barycentric.X() +
                triangle.Second().Vector() * result.Barycentric.Y() +
                triangle.Third().Vector() * result.Barycentric.Z());
            Require(NearlyEqual(reconstructed.Vector(),
                                result.PointOnTriangle.Vector(), 2.0e-4F));
        }
    }

    void CheckLayouts()
    {
        using namespace mv::math;
        static_assert(sizeof(Ray3f) == 32);
        static_assert(sizeof(Line3f) == 32);
        static_assert(sizeof(Segment3f) == 32);
        static_assert(sizeof(Plane3f) == 16);
        static_assert(sizeof(Triangle3f) == 48);
        static_assert(sizeof(Sphere3f) == 16);
        static_assert(sizeof(Aabb3f) == 32);
        static_assert(std::is_trivially_copyable_v<PreparedRay3f>);
    }
}  // namespace

int main()
{
    CheckRayAndPlane();
    CheckLineAndSegment();
    CheckTriangleAndSphere();
    CheckAabb();
    CheckLinearQueries();
    CheckClosestPointQueries();
    CheckBoundsQueries();
    CheckAabbReferenceParity();
    CheckTriangleClosestReferenceParity();
    CheckLayouts();
    return 0;
}
