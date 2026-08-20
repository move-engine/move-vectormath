#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Math.hpp>

namespace
{
    [[nodiscard]] mv::math::Ray3f MakeRay(mv::math::Point3f origin,
                                          mv::math::Direction3f direction)
    {
        const auto ray =
            mv::math::Ray3f::TryFromOriginDirection(origin, direction);
        REQUIRE(ray.has_value());
        return *ray;
    }

    [[nodiscard]] mv::math::Triangle3f MakeTriangle(mv::math::Point3f first,
                                                    mv::math::Point3f second,
                                                    mv::math::Point3f third)
    {
        const auto triangle =
            mv::math::Triangle3f::TryFromPoints(first, second, third);
        REQUIRE(triangle.has_value());
        return *triangle;
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

    struct ReferenceSegmentPairResult
    {
        Double3 PointOnFirst;
        Double3 PointOnSecond;
        double FirstFraction;
        double SecondFraction;
        double SquaredDistance;
    };

    // Independent exhaustive-critical-point oracle from Eberly, "Robust
    // Computation of Distance Between Line Segments" (2018, revised 2023),
    // section 2 (CC BY 4.0). Production uses Ericson's decision tree instead.
    [[nodiscard]] ReferenceSegmentPairResult ReferenceSegmentPair(
        const mv::math::Segment3f& first, const mv::math::Segment3f& second)
    {
        const Double3 firstStart = ToDouble(first.Start());
        const Double3 secondStart = ToDouble(second.Start());
        const Double3 firstDirection = ToDouble(first.End()) - firstStart;
        const Double3 secondDirection = ToDouble(second.End()) - secondStart;
        const Double3 startOffset = firstStart - secondStart;
        const double a = Dot(firstDirection, firstDirection);
        const double b = Dot(firstDirection, secondDirection);
        const double c = Dot(secondDirection, secondDirection);
        const double d = Dot(firstDirection, startOffset);
        const double e = Dot(secondDirection, startOffset);

        auto evaluate = [&](double firstFraction, double secondFraction)
        {
            const Double3 pointOnFirst =
                firstStart + firstDirection * firstFraction;
            const Double3 pointOnSecond =
                secondStart + secondDirection * secondFraction;
            const Double3 difference = pointOnFirst - pointOnSecond;
            return ReferenceSegmentPairResult{pointOnFirst, pointOnSecond,
                                              firstFraction, secondFraction,
                                              Dot(difference, difference)};
        };

        ReferenceSegmentPairResult closest = evaluate(0.0, 0.0);
        auto consider = [&](double firstFraction, double secondFraction)
        {
            const ReferenceSegmentPairResult candidate =
                evaluate(firstFraction, secondFraction);
            if (candidate.SquaredDistance < closest.SquaredDistance)
            {
                closest = candidate;
            }
        };

        consider(1.0, 0.0);
        consider(0.0, 1.0);
        consider(1.0, 1.0);
        if (a > 0.0)
        {
            consider(std::clamp(-d / a, 0.0, 1.0), 0.0);
            consider(std::clamp((b - d) / a, 0.0, 1.0), 1.0);
        }
        if (c > 0.0)
        {
            consider(0.0, std::clamp(e / c, 0.0, 1.0));
            consider(1.0, std::clamp((b + e) / c, 0.0, 1.0));
        }
        const double determinant = a * c - b * b;
        if (determinant > 0.0)
        {
            const double firstFraction = (b * e - c * d) / determinant;
            const double secondFraction = (a * e - b * d) / determinant;
            if (firstFraction >= 0.0 && firstFraction <= 1.0 &&
                secondFraction >= 0.0 && secondFraction <= 1.0)
            {
                consider(firstFraction, secondFraction);
            }
        }
        return closest;
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
        if (box.IsEmpty())
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

        const Ray3f ray =
            MakeRay(Point3f(1.0F, 2.0F, 3.0F), Direction3f::AxisZ());
        REQUIRE(ray.Origin() == Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(ray.Direction() == Direction3f::AxisZ());
        REQUIRE(ray.PointAt(4.0F) == Point3f(1.0F, 2.0F, 7.0F));
        REQUIRE(!Ray3f::TryFromOriginDirection(
            Point3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F),
            Direction3f::AxisX()));

        const auto plane = Plane3f::TryFromPointNormal(
            Point3f(0.0F, 0.0F, 2.0F), Normal3f::AxisZ());
        REQUIRE(plane.has_value());
        REQUIRE(NearlyEqual(plane->Offset(), -2.0F));
        REQUIRE(NearlyEqual(plane->SignedDistance(Point3f(0.0F, 0.0F, 5.0F)),
                            3.0F));
        REQUIRE(plane->Classify(Point3f(0.0F, 0.0F, 5.0F)) == PlaneSide::Front);
        REQUIRE(plane->Classify(Point3f(0.0F, 0.0F, 1.0F)) == PlaneSide::Back);
        REQUIRE(plane->Classify(Point3f(0.0F, 0.0F, 2.001F), 0.01F) ==
                PlaneSide::Intersecting);
        REQUIRE(
            NearlyEqual(plane->ClosestPoint(Point3f(1.0F, 2.0F, 8.0F)).Vector(),
                        Vec3f(1.0F, 2.0F, 2.0F)));
        REQUIRE(plane->Flipped().Normal() == -Normal3f::AxisZ());
        REQUIRE(NearlyEqual(plane->Flipped().Offset(), 2.0F));

        const auto threePointPlane = Plane3f::TryFromPoints(
            Point3f(0.0F, 0.0F, 0.0F), Point3f(1.0F, 0.0F, 0.0F),
            Point3f(0.0F, 1.0F, 0.0F));
        REQUIRE(threePointPlane.has_value());
        REQUIRE(threePointPlane->Normal() == Normal3f::AxisZ());
        REQUIRE(!Plane3f::TryFromPoints(Point3f(), Point3f(), Point3f()));
        REQUIRE(!Plane3f::TryFromNormalAndOffset(
            Normal3f::AxisX(), std::numeric_limits<float>::infinity()));
    }

    void CheckLineAndSegment()
    {
        using namespace mv::math;

        const Line3f line(Point3f(1.0F, 2.0F, 3.0F), Direction3f::AxisX());
        REQUIRE(line.Origin() == Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(line.Direction() == Direction3f::AxisX());
        REQUIRE(line.PointAt(-2.0F) == Point3f(-1.0F, 2.0F, 3.0F));
        REQUIRE(line.IsFinite());

        const Segment3f segment(Point3f(), Point3f(0.0F, 0.0F, 4.0F));
        REQUIRE(segment.Start() == Point3f());
        REQUIRE(segment.End() == Point3f(0.0F, 0.0F, 4.0F));
        REQUIRE(segment.Displacement() == Vec3f(0.0F, 0.0F, 4.0F));
        REQUIRE(segment.LengthSquared() == 16.0F);
        REQUIRE(segment.Length() == 4.0F);
        REQUIRE(!segment.IsDegenerate());
        REQUIRE(segment.TryDirection() == Direction3f::AxisZ());
        REQUIRE(segment.PointAtFraction(0.25F) == Point3f(0.0F, 0.0F, 1.0F));
        REQUIRE(segment.PointAtFraction(2.0F) == Point3f(0.0F, 0.0F, 8.0F));
        REQUIRE(segment.IsFinite());

        const Segment3f degenerate(Point3f(1.0F, 2.0F, 3.0F),
                                   Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(degenerate.IsDegenerate());
        REQUIRE(!degenerate.TryDirection());
        REQUIRE(degenerate.PointAtFraction(0.75F) == degenerate.Start());
    }

    void CheckTriangleAndSphere()
    {
        using namespace mv::math;

        const Triangle3f triangle =
            MakeTriangle(Point3f(0.0F, 0.0F, 0.0F), Point3f(2.0F, 0.0F, 0.0F),
                         Point3f(0.0F, 2.0F, 0.0F));
        REQUIRE(triangle.Edge01() == Vec3f(2.0F, 0.0F, 0.0F));
        REQUIRE(triangle.Edge02() == Vec3f(0.0F, 2.0F, 0.0F));
        REQUIRE(NearlyEqual(triangle.Centroid().Vector(),
                            Vec3f(2.0F / 3.0F, 2.0F / 3.0F, 0.0F)));
        REQUIRE(NearlyEqual(triangle.Area(), 2.0F));
        REQUIRE(triangle.TryNormal() == Normal3f::AxisZ());

        const Triangle3f degenerate = MakeTriangle(
            Point3f(), Point3f(1.0F, 0.0F, 0.0F), Point3f(2.0F, 0.0F, 0.0F));
        REQUIRE(!degenerate.TryNormal());
        REQUIRE(NearlyEqual(degenerate.Area(), 0.0F));

        REQUIRE(!Triangle3f::TryFromPoints(
            Point3f(), Point3f(),
            Point3f(0.0F, std::numeric_limits<float>::quiet_NaN(), 0.0F)));

        REQUIRE(!Sphere3f::TryFromCenterRadius(Point3f(), -1.0F));
        REQUIRE(!Sphere3f::TryFromCenterRadius(
            Point3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F), 1.0F));
        const auto sphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.0F, 2.0F, 3.0F), 2.0F);
        REQUIRE(sphere.has_value());
        REQUIRE(sphere->Center() == Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(sphere->Radius() == 2.0F);
        REQUIRE(sphere->Diameter() == 4.0F);
        REQUIRE(sphere->Contains(Point3f(3.0F, 2.0F, 3.0F)));
        REQUIRE(!sphere->Contains(Point3f(3.01F, 2.0F, 3.0F)));
    }

    void CheckAabb()
    {
        using namespace mv::math;

        const Aabb3f empty;
        REQUIRE(empty.IsEmpty());
        REQUIRE(empty.IsValid());
        REQUIRE(!empty.TryCenter());
        REQUIRE(!empty.TrySize());
        REQUIRE(!empty.TryCorners());
        REQUIRE(empty.Volume() == 0.0F);
        REQUIRE(empty.SurfaceArea() == 0.0F);

        REQUIRE(!Aabb3f::TryFromMinMax(Point3f(1.0F, 0.0F, 0.0F),
                                       Point3f(0.0F, 1.0F, 1.0F)));
        REQUIRE(!Aabb3f::TryFromCenterHalfExtents(Point3f(),
                                                  Vec3f(1.0F, -1.0F, 1.0F)));

        const auto box = Aabb3f::TryFromCenterHalfExtents(
            Point3f(1.0F, 2.0F, 3.0F), Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(box.has_value());
        REQUIRE(box->Minimum() == Point3f(0.0F, 0.0F, 0.0F));
        REQUIRE(box->Maximum() == Point3f(2.0F, 4.0F, 6.0F));
        REQUIRE(box->TryCenter() == Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(box->TrySize() == Vec3f(2.0F, 4.0F, 6.0F));
        REQUIRE(box->TryHalfExtents() == Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(box->Contains(box->Minimum()));
        REQUIRE(box->Contains(box->Maximum()));
        REQUIRE(!box->Contains(Point3f(2.01F, 2.0F, 3.0F)));
        REQUIRE(box->Volume() == 48.0F);
        REQUIRE(box->SurfaceArea() == 88.0F);
        REQUIRE(box->TryCorners()->size() == 8U);
        REQUIRE(box->TryClosestPoint(Point3f(-1.0F, 5.0F, 2.0F)) ==
                Point3f(0.0F, 4.0F, 2.0F));

        const auto touching = Aabb3f::TryFromMinMax(Point3f(2.0F, 1.0F, 1.0F),
                                                    Point3f(3.0F, 2.0F, 2.0F));
        REQUIRE(touching.has_value());
        REQUIRE(box->Intersects(*touching));
        const Aabb3f overlap = box->Intersection(*touching);
        REQUIRE(!overlap.IsEmpty());
        REQUIRE(overlap.Minimum().X() == 2.0F);
        REQUIRE(overlap.Maximum().X() == 2.0F);

        const auto disjoint = Aabb3f::TryFromMinMax(Point3f(3.0F, 0.0F, 0.0F),
                                                    Point3f(4.0F, 1.0F, 1.0F));
        REQUIRE(disjoint.has_value());
        REQUIRE(!box->Intersects(*disjoint));
        REQUIRE(box->Intersection(*disjoint).IsEmpty());
        REQUIRE(box->Merged(*disjoint).Maximum() == Point3f(4.0F, 4.0F, 6.0F));
        REQUIRE(empty.Merged(*box) == *box);

        const std::array points = {Point3f(-2.0F, 3.0F, 1.0F),
                                   Point3f(4.0F, -1.0F, 6.0F)};
        const auto fromPoints = Aabb3f::TryFromPoints(points);
        REQUIRE(fromPoints.has_value());
        REQUIRE(fromPoints->Minimum() == Point3f(-2.0F, -1.0F, 1.0F));
        REQUIRE(fromPoints->Maximum() == Point3f(4.0F, 3.0F, 6.0F));
        REQUIRE(Aabb3f::TryFromPoints(std::span<const Point3f>{})->IsEmpty());
        REQUIRE(!box->TryExpanded(
            Point3f(std::numeric_limits<float>::quiet_NaN(), 0.0F, 0.0F)));
    }

    void CheckLinearQueries()
    {
        using namespace mv::math;

        const auto plane =
            Plane3f::TryFromPointNormal(Point3f(), Normal3f::AxisZ());
        REQUIRE(plane.has_value());
        const Ray3f frontRay =
            MakeRay(Point3f(0.0F, 0.0F, 2.0F), -Direction3f::AxisZ());
        const auto planeHit = Intersect(frontRay, *plane);
        REQUIRE(planeHit.has_value());
        REQUIRE(planeHit->Distance == 2.0F);
        REQUIRE(planeHit->Point == Point3f());
        REQUIRE(planeHit->Normal == Normal3f::AxisZ());
        REQUIRE(planeHit->Face == FaceOrientation::Front);

        const Ray3f backRay =
            MakeRay(Point3f(0.0F, 0.0F, -2.0F), Direction3f::AxisZ());
        REQUIRE(Intersect(backRay, *plane)->Face == FaceOrientation::Back);
        REQUIRE(!Intersect(
            MakeRay(Point3f(0.0F, 0.0F, 1.0F), Direction3f::AxisX()), *plane));
        REQUIRE(!Intersect(MakeRay(Point3f(), Direction3f::AxisX()), *plane));

        const Triangle3f triangle =
            MakeTriangle(Point3f(0.0F, 0.0F, 0.0F), Point3f(1.0F, 0.0F, 0.0F),
                         Point3f(0.0F, 1.0F, 0.0F));
        const Ray3f triangleRay =
            MakeRay(Point3f(0.25F, 0.25F, 1.0F), -Direction3f::AxisZ());
        const auto triangleHit = Intersect(triangleRay, triangle);
        REQUIRE(triangleHit.has_value());
        REQUIRE(triangleHit->Distance == 1.0F);
        REQUIRE(triangleHit->Point == Point3f(0.25F, 0.25F, 0.0F));
        REQUIRE(triangleHit->GeometricNormal == Normal3f::AxisZ());
        REQUIRE(
            NearlyEqual(triangleHit->Barycentric, Vec3f(0.5F, 0.25F, 0.25F)));
        REQUIRE(triangleHit->Face == FaceOrientation::Front);

        const Ray3f triangleBackRay =
            MakeRay(Point3f(0.25F, 0.25F, -1.0F), Direction3f::AxisZ());
        REQUIRE(Intersect(triangleBackRay, triangle)->Face ==
                FaceOrientation::Back);
        REQUIRE(!Intersect(triangleBackRay, triangle,
                           RayTriangleOptionsf{BackFaceMode::Cull}));
        REQUIRE(!Intersect(
            MakeRay(Point3f(2.0F, 2.0F, 1.0F), -Direction3f::AxisZ()),
            triangle));
        REQUIRE(!Intersect(triangleRay,
                           MakeTriangle(Point3f(), Point3f(), Point3f())));
    }

    void CheckClosestPointQueries()
    {
        using namespace mv::math;

        const Point3f point(2.0F, 3.0F, 0.0F);
        const Line3f line(Point3f(), Direction3f::AxisX());
        const PointLineClosest3f lineClosest = ClosestPoints(point, line);
        REQUIRE(lineClosest.PointOnLine == Point3f(2.0F, 0.0F, 0.0F));
        REQUIRE(lineClosest.LineDistance == 2.0F);
        REQUIRE(lineClosest.SquaredDistance == 9.0F);
        REQUIRE(lineClosest.Distance() == 3.0F);
        REQUIRE(ClosestPoint(point, line) == lineClosest.PointOnLine);
        REQUIRE(DistanceSquared(point, line) == 9.0F);
        REQUIRE(Distance(point, line) == 3.0F);

        const PointLineClosest3f negativeLineClosest =
            ClosestPoints(Point3f(-2.0F, 1.0F, 0.0F), line);
        REQUIRE(negativeLineClosest.LineDistance == -2.0F);
        REQUIRE(negativeLineClosest.PointOnLine == Point3f(-2.0F, 0.0F, 0.0F));

        const Ray3f ray = MakeRay(Point3f(), Direction3f::AxisX());
        const PointRayClosest3f rayClosest =
            ClosestPoints(Point3f(-2.0F, 1.0F, 0.0F), ray);
        REQUIRE(rayClosest.RayDistance == 0.0F);
        REQUIRE(rayClosest.PointOnRay == Point3f());
        REQUIRE(rayClosest.SquaredDistance == 5.0F);

        const Segment3f segment(Point3f(), Point3f(4.0F, 0.0F, 0.0F));
        const PointSegmentClosest3f segmentClosest =
            ClosestPoints(point, segment);
        REQUIRE(segmentClosest.PointOnSegment == Point3f(2.0F, 0.0F, 0.0F));
        REQUIRE(segmentClosest.SegmentFraction == 0.5F);
        REQUIRE(segmentClosest.SquaredDistance == 9.0F);
        REQUIRE(
            ClosestPoints(Point3f(6.0F, 1.0F, 0.0F), segment).SegmentFraction ==
            1.0F);

        const Segment3f pointSegment(Point3f(1.0F, 2.0F, 3.0F),
                                     Point3f(1.0F, 2.0F, 3.0F));
        const PointSegmentClosest3f pointSegmentClosest =
            ClosestPoints(Point3f(4.0F, 6.0F, 3.0F), pointSegment);
        REQUIRE(pointSegmentClosest.PointOnSegment == pointSegment.Start());
        REQUIRE(pointSegmentClosest.SegmentFraction == 0.0F);
        REQUIRE(pointSegmentClosest.SquaredDistance == 25.0F);

        const auto plane =
            Plane3f::TryFromPointNormal(Point3f(), Normal3f::AxisZ());
        REQUIRE(plane.has_value());
        const PointPlaneClosest3f planeClosest =
            ClosestPoints(Point3f(1.0F, 2.0F, -3.0F), *plane);
        REQUIRE(planeClosest.PointOnPlane == Point3f(1.0F, 2.0F, 0.0F));
        REQUIRE(planeClosest.SignedDistance == -3.0F);
        REQUIRE(planeClosest.SquaredDistance == 9.0F);

        const Triangle3f triangle =
            MakeTriangle(Point3f(0.0F, 0.0F, 0.0F), Point3f(2.0F, 0.0F, 0.0F),
                         Point3f(0.0F, 2.0F, 0.0F));
        const PointTriangleClosest3f faceClosest =
            ClosestPoints(Point3f(0.5F, 0.5F, 2.0F), triangle);
        REQUIRE(faceClosest.PointOnTriangle == Point3f(0.5F, 0.5F, 0.0F));
        REQUIRE(
            NearlyEqual(faceClosest.Barycentric, Vec3f(0.5F, 0.25F, 0.25F)));
        REQUIRE(faceClosest.SquaredDistance == 4.0F);

        const PointTriangleClosest3f firstClosest =
            ClosestPoints(Point3f(-1.0F, -1.0F, 0.0F), triangle);
        REQUIRE(firstClosest.PointOnTriangle == triangle.First());
        REQUIRE(firstClosest.Barycentric == Vec3f(1.0F, 0.0F, 0.0F));

        const PointTriangleClosest3f edge01Closest =
            ClosestPoints(Point3f(0.5F, -1.0F, 0.0F), triangle);
        REQUIRE(edge01Closest.PointOnTriangle == Point3f(0.5F, 0.0F, 0.0F));
        REQUIRE(
            NearlyEqual(edge01Closest.Barycentric, Vec3f(0.75F, 0.25F, 0.0F)));

        const PointTriangleClosest3f edge12Closest =
            ClosestPoints(Point3f(2.0F, 2.0F, 0.0F), triangle);
        REQUIRE(edge12Closest.PointOnTriangle == Point3f(1.0F, 1.0F, 0.0F));
        REQUIRE(
            NearlyEqual(edge12Closest.Barycentric, Vec3f(0.0F, 0.5F, 0.5F)));

        const Triangle3f lineTriangle = MakeTriangle(
            Point3f(), Point3f(2.0F, 0.0F, 0.0F), Point3f(1.0F, 0.0F, 0.0F));
        const PointTriangleClosest3f lineTriangleClosest =
            ClosestPoints(Point3f(1.0F, 2.0F, 0.0F), lineTriangle);
        REQUIRE(lineTriangleClosest.PointOnTriangle ==
                Point3f(1.0F, 0.0F, 0.0F));
        REQUIRE(NearlyEqual(lineTriangleClosest.Barycentric,
                            Vec3f(0.5F, 0.5F, 0.0F)));

        const Triangle3f pointTriangle =
            MakeTriangle(Point3f(3.0F, 4.0F, 5.0F), Point3f(3.0F, 4.0F, 5.0F),
                         Point3f(3.0F, 4.0F, 5.0F));
        const PointTriangleClosest3f pointTriangleClosest =
            ClosestPoints(Point3f(), pointTriangle);
        REQUIRE(pointTriangleClosest.PointOnTriangle == pointTriangle.First());
        REQUIRE(pointTriangleClosest.Barycentric == Vec3f(1.0F, 0.0F, 0.0F));
        REQUIRE(pointTriangleClosest.SquaredDistance == 50.0F);
    }

    void CheckBoundsClosestAndSegmentPairs()
    {
        using namespace mv::math;

        const auto box = Aabb3f::TryFromMinMax(Point3f(-1.0F, -2.0F, -3.0F),
                                               Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(box.has_value());
        const auto boxClosest =
            TryClosestPoints(Point3f(4.0F, 1.0F, -5.0F), *box);
        REQUIRE(boxClosest.has_value());
        REQUIRE(boxClosest->PointInAabb == Point3f(1.0F, 1.0F, -3.0F));
        REQUIRE(boxClosest->SquaredDistance == 13.0F);
        REQUIRE(TryClosestPoint(Point3f(), *box) == Point3f());
        REQUIRE(TryDistanceSquared(Point3f(), *box) == 0.0F);
        REQUIRE(TryDistance(Point3f(4.0F, 1.0F, -5.0F), *box) ==
                std::sqrt(13.0F));
        REQUIRE(!TryClosestPoints(Point3f(), Aabb3f::Empty()));

        const auto sphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.0F, 2.0F, 3.0F), 2.0F);
        REQUIRE(sphere.has_value());
        const PointSphereClosest3f sphereOutside =
            ClosestPoints(Point3f(5.0F, 2.0F, 3.0F), *sphere);
        REQUIRE(sphereOutside.PointInSphere == Point3f(3.0F, 2.0F, 3.0F));
        REQUIRE(sphereOutside.SquaredDistance == 4.0F);
        const Point3f inside(1.5F, 2.0F, 3.0F);
        REQUIRE(ClosestPoint(inside, *sphere) == inside);
        REQUIRE(DistanceSquared(inside, *sphere) == 0.0F);

        const Segment3f first(Point3f(-1.0F, 0.0F, 0.0F),
                              Point3f(1.0F, 0.0F, 0.0F));
        const Segment3f crossing(Point3f(0.0F, -1.0F, 0.0F),
                                 Point3f(0.0F, 1.0F, 0.0F));
        const SegmentSegmentClosest3f crossingClosest =
            ClosestPoints(first, crossing);
        REQUIRE(crossingClosest.PointOnFirst == Point3f());
        REQUIRE(crossingClosest.PointOnSecond == Point3f());
        REQUIRE(crossingClosest.FirstFraction == 0.5F);
        REQUIRE(crossingClosest.SecondFraction == 0.5F);
        REQUIRE(crossingClosest.SquaredDistance == 0.0F);

        const Segment3f skew(Point3f(0.0F, -1.0F, 3.0F),
                             Point3f(0.0F, 1.0F, 3.0F));
        const SegmentSegmentClosest3f skewClosest = ClosestPoints(first, skew);
        REQUIRE(skewClosest.PointOnFirst == Point3f());
        REQUIRE(skewClosest.PointOnSecond == Point3f(0.0F, 0.0F, 3.0F));
        REQUIRE(skewClosest.SquaredDistance == 9.0F);
        REQUIRE(Distance(first, skew) == 3.0F);

        const Segment3f pointSegment(Point3f(3.0F, 2.0F, 0.0F),
                                     Point3f(3.0F, 2.0F, 0.0F));
        const SegmentSegmentClosest3f pointClosest =
            ClosestPoints(pointSegment, first);
        REQUIRE(pointClosest.PointOnFirst == pointSegment.Start());
        REQUIRE(pointClosest.PointOnSecond == Point3f(1.0F, 0.0F, 0.0F));
        REQUIRE(pointClosest.FirstFraction == 0.0F);
        REQUIRE(pointClosest.SecondFraction == 1.0F);
        REQUIRE(pointClosest.SquaredDistance == 8.0F);

        const Segment3f secondPoint(Point3f(1.0F, 2.0F, 3.0F),
                                    Point3f(1.0F, 2.0F, 3.0F));
        const auto pointsClosest = ClosestPoints(pointSegment, secondPoint);
        REQUIRE(pointsClosest.PointOnFirst == pointSegment.Start());
        REQUIRE(pointsClosest.PointOnSecond == secondPoint.Start());
        REQUIRE(pointsClosest.SquaredDistance == 13.0F);

        const Segment3f nearlyParallel(Point3f(-1.0F, 0.001F, 0.0F),
                                       Point3f(1.0F, 0.001001F, 0.0F));
        REQUIRE(NearlyEqual(DistanceSquared(first, nearlyParallel), 1.0e-6F,
                            2.0e-9F));
    }

    void CheckCapsule()
    {
        using namespace mv::math;

        REQUIRE(!Capsule3f::TryFromEndpointsRadius(
            Point3f(), Point3f(0.0F, 1.0F, 0.0F), -1.0F));
        const auto capsule = Capsule3f::TryFromEndpointsRadius(
            Point3f(0.0F, -1.0F, 0.0F), Point3f(0.0F, 1.0F, 0.0F), 0.5F);
        REQUIRE(capsule.has_value());
        REQUIRE(capsule->Start() == Point3f(0.0F, -1.0F, 0.0F));
        REQUIRE(capsule->End() == Point3f(0.0F, 1.0F, 0.0F));
        REQUIRE(capsule->Radius() == 0.5F);
        REQUIRE(!capsule->IsDegenerate());
        REQUIRE(capsule->IsFinite());

        const PointCapsuleClosest3f side =
            ClosestPoints(Point3f(2.0F, 0.0F, 0.0F), *capsule);
        REQUIRE(side.PointInCapsule == Point3f(0.5F, 0.0F, 0.0F));
        REQUIRE(side.CenterLineFraction == 0.5F);
        REQUIRE(side.SquaredDistance == 2.25F);
        REQUIRE(Contains(*capsule, Point3f(0.25F, 0.0F, 0.0F)));
        REQUIRE(!Contains(*capsule, Point3f(0.75F, 0.0F, 0.0F)));

        const PointCapsuleClosest3f cap =
            ClosestPoints(Point3f(0.0F, 2.0F, 0.0F), *capsule);
        REQUIRE(cap.PointInCapsule == Point3f(0.0F, 1.5F, 0.0F));
        REQUIRE(cap.CenterLineFraction == 1.0F);
        REQUIRE(cap.SquaredDistance == 0.25F);

        const auto touchingSphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.5F, 0.0F, 0.0F), 1.0F);
        REQUIRE(touchingSphere.has_value());
        REQUIRE(Intersects(*capsule, *touchingSphere));
        REQUIRE(Intersects(*touchingSphere, *capsule));

        const auto touchingCapsule = Capsule3f::TryFromEndpointsRadius(
            Point3f(1.0F, -1.0F, 0.0F), Point3f(1.0F, 1.0F, 0.0F), 0.5F);
        REQUIRE(touchingCapsule.has_value());
        REQUIRE(Intersects(*capsule, *touchingCapsule));
        const auto separatedCapsule = Capsule3f::TryFromEndpointsRadius(
            Point3f(1.01F, -1.0F, 0.0F), Point3f(1.01F, 1.0F, 0.0F), 0.5F);
        REQUIRE(separatedCapsule.has_value());
        REQUIRE(!Intersects(*capsule, *separatedCapsule));

        const auto sphereCapsule = Capsule3f::TryFromEndpointsRadius(
            Point3f(2.0F, 3.0F, 4.0F), Point3f(2.0F, 3.0F, 4.0F), 2.0F);
        REQUIRE(sphereCapsule.has_value());
        REQUIRE(sphereCapsule->IsDegenerate());
        REQUIRE(Contains(*sphereCapsule, Point3f(4.0F, 3.0F, 4.0F)));
    }

    void CheckBoundsQueries()
    {
        using namespace mv::math;

        const auto sphere = Sphere3f::TryFromCenterRadius(Point3f(), 1.0F);
        REQUIRE(sphere.has_value());
        const Ray3f sphereRay =
            MakeRay(Point3f(-3.0F, 0.0F, 0.0F), Direction3f::AxisX());
        const auto sphereHit = Intersect(sphereRay, *sphere);
        REQUIRE(sphereHit.has_value());
        REQUIRE(sphereHit->EntryDistance == 2.0F);
        REQUIRE(sphereHit->ExitDistance == 4.0F);
        REQUIRE(!sphereHit->StartsInside);
        REQUIRE(sphereHit->EntryNormal == -Normal3f::AxisX());
        REQUIRE(sphereHit->ExitNormal == Normal3f::AxisX());

        const auto insideSphereHit =
            Intersect(MakeRay(Point3f(), Direction3f::AxisX()), *sphere);
        REQUIRE(insideSphereHit.has_value());
        REQUIRE(insideSphereHit->EntryDistance == 0.0F);
        REQUIRE(insideSphereHit->ExitDistance == 1.0F);
        REQUIRE(insideSphereHit->StartsInside);
        REQUIRE(!insideSphereHit->EntryNormal);
        REQUIRE(insideSphereHit->ExitNormal == Normal3f::AxisX());

        const auto pointSphere = Sphere3f::TryFromCenterRadius(Point3f(), 0.0F);
        REQUIRE(pointSphere.has_value());
        const auto pointHit =
            Intersect(MakeRay(Point3f(-1.0F, 0.0F, 0.0F), Direction3f::AxisX()),
                      *pointSphere);
        REQUIRE(pointHit.has_value());
        REQUIRE(!pointHit->EntryNormal);
        REQUIRE(!pointHit->ExitNormal);
        REQUIRE(!Intersect(
            MakeRay(Point3f(-3.0F, 2.0F, 0.0F), Direction3f::AxisX()),
            *sphere));

        const auto box = Aabb3f::TryFromMinMax(Point3f(-1.0F, -1.0F, -1.0F),
                                               Point3f(1.0F, 1.0F, 1.0F));
        REQUIRE(box.has_value());
        const Ray3f boxRay =
            MakeRay(Point3f(-3.0F, 0.0F, 0.0F), Direction3f::AxisX());
        const auto boxHit = Intersect(boxRay, *box);
        REQUIRE(boxHit.has_value());
        REQUIRE(boxHit->EntryDistance == 2.0F);
        REQUIRE(boxHit->ExitDistance == 4.0F);
        REQUIRE(!boxHit->StartsInside);
        REQUIRE(boxHit->EntryNormal == -Normal3f::AxisX());
        REQUIRE(boxHit->ExitNormal == Normal3f::AxisX());
        REQUIRE(Intersect(PreparedRay3f(boxRay), *box) == boxHit);

        const auto insideBoxHit =
            Intersect(MakeRay(Point3f(), Direction3f::AxisY()), *box);
        REQUIRE(insideBoxHit.has_value());
        REQUIRE(insideBoxHit->EntryDistance == 0.0F);
        REQUIRE(insideBoxHit->ExitDistance == 1.0F);
        REQUIRE(insideBoxHit->StartsInside);
        REQUIRE(!insideBoxHit->EntryNormal);
        REQUIRE(insideBoxHit->ExitNormal == Normal3f::AxisY());

        REQUIRE(
            Intersect(MakeRay(Point3f(-3.0F, 1.0F, 0.0F), Direction3f::AxisX()),
                      *box)
                .has_value());
        REQUIRE(!Intersect(
            MakeRay(Point3f(-3.0F, 2.0F, 0.0F), Direction3f::AxisX()), *box));
        REQUIRE(!Intersect(boxRay, Aabb3f::Empty()));

        const auto touchingSphere =
            Sphere3f::TryFromCenterRadius(Point3f(2.0F, 0.0F, 0.0F), 1.0F);
        REQUIRE(touchingSphere.has_value());
        REQUIRE(Intersects(*sphere, *touchingSphere));
        REQUIRE(Intersects(*box, *box));
        REQUIRE(Intersects(*sphere, *box));
        const auto separatedSphere =
            Sphere3f::TryFromCenterRadius(Point3f(3.0F, 3.0F, 3.0F), 1.0F);
        REQUIRE(separatedSphere.has_value());
        REQUIRE(!Intersects(*separatedSphere, *box));
        REQUIRE(!Intersects(Aabb3f::Empty(), *sphere));

        const auto boundarySphereHit = Intersect(
            MakeRay(Point3f(1.0F, 0.0F, 0.0F), Direction3f::AxisX()), *sphere);
        REQUIRE(boundarySphereHit.has_value());
        REQUIRE(!boundarySphereHit->StartsInside);
        REQUIRE(boundarySphereHit->EntryDistance == 0.0F);
        REQUIRE(boundarySphereHit->EntryNormal == Normal3f::AxisX());
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
            REQUIRE(box.has_value());

            const Vec3f directionValue(NextValue(state), NextValue(state),
                                       NextValue(state));
            const auto direction = Direction3f::TryFrom(directionValue);
            REQUIRE(direction.has_value());
            const Ray3f ray = MakeRay(
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                *direction);

            const bool reference = ReferenceRayAabb(ray, *box);
            REQUIRE(Intersects(ray, *box) == reference);
            REQUIRE(Intersects(PreparedRay3f(ray), *box) == reference);
        }
    }

    void CheckTriangleClosestReferenceParity()
    {
        using namespace mv::math;

        std::uint32_t state = 0x51A7C105U;
        for (std::size_t index = 0U; index < 1024U; ++index)
        {
            const Triangle3f triangle = MakeTriangle(
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                Point3f(NextValue(state), NextValue(state), NextValue(state)),
                Point3f(NextValue(state), NextValue(state), NextValue(state)));
            const Point3f point(NextValue(state), NextValue(state),
                                NextValue(state));
            const PointTriangleClosest3f result =
                ClosestPoints(point, triangle);
            const Double3 reference = ReferencePointTriangle(point, triangle);
            REQUIRE(NearlyEqual(result.PointOnTriangle.Vector(),
                                Vec3f(static_cast<float>(reference.X),
                                      static_cast<float>(reference.Y),
                                      static_cast<float>(reference.Z)),
                                2.0e-4F));
            REQUIRE(NearlyEqual(result.Barycentric.X() +
                                    result.Barycentric.Y() +
                                    result.Barycentric.Z(),
                                1.0F, 2.0e-4F));
            const Point3f reconstructed = Point3f::FromVector(
                triangle.First().Vector() * result.Barycentric.X() +
                triangle.Second().Vector() * result.Barycentric.Y() +
                triangle.Third().Vector() * result.Barycentric.Z());
            REQUIRE(NearlyEqual(reconstructed.Vector(),
                                result.PointOnTriangle.Vector(), 2.0e-4F));
        }
    }

    void CheckSegmentPairReferenceParity()
    {
        using namespace mv::math;

        std::uint32_t state = 0x5E6A3E17U;
        for (std::size_t index = 0U; index < 2048U; ++index)
        {
            Point3f firstStart(NextValue(state), NextValue(state),
                               NextValue(state));
            Point3f firstEnd(NextValue(state), NextValue(state),
                             NextValue(state));
            Point3f secondStart(NextValue(state), NextValue(state),
                                NextValue(state));
            Point3f secondEnd(NextValue(state), NextValue(state),
                              NextValue(state));
            if (index % 31U == 0U)
            {
                firstEnd = firstStart;
            }
            if (index % 47U == 0U)
            {
                secondEnd = secondStart;
            }

            const Segment3f first(firstStart, firstEnd);
            const Segment3f second(secondStart, secondEnd);
            const SegmentSegmentClosest3f result = ClosestPoints(first, second);
            const ReferenceSegmentPairResult reference =
                ReferenceSegmentPair(first, second);
            REQUIRE(NearlyEqual(result.SquaredDistance,
                                static_cast<float>(reference.SquaredDistance),
                                3.0e-4F));
            REQUIRE((result.FirstFraction >= 0.0F &&
                     result.FirstFraction <= 1.0F));
            REQUIRE((result.SecondFraction >= 0.0F &&
                     result.SecondFraction <= 1.0F));
            REQUIRE(NearlyEqual(
                result.PointOnFirst.Vector(),
                first.PointAtFraction(result.FirstFraction).Vector(), 2.0e-5F));
            REQUIRE(NearlyEqual(
                result.PointOnSecond.Vector(),
                second.PointAtFraction(result.SecondFraction).Vector(),
                2.0e-5F));
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
        static_assert(sizeof(Capsule3f) == 32);
        static_assert(sizeof(Aabb3f) == 32);
        static_assert(!std::is_constructible_v<Ray3f, Point3f, Direction3f>);
        static_assert(
            !std::is_constructible_v<Triangle3f, Point3f, Point3f, Point3f>);
        static_assert(std::is_trivially_copyable_v<Ray3f>);
        static_assert(std::is_trivially_copyable_v<Triangle3f>);
        static_assert(std::is_trivially_copyable_v<PreparedRay3f>);
    }

    void CheckGenericGeometry()
    {
        using namespace mv::math;

        const auto direction = Direction3d::TryFrom(Vec3d(0.0, 0.0, 2.0));
        REQUIRE(direction.has_value());
        const auto ray = Ray3d::TryFromOriginDirection(Point3d(), *direction);
        REQUIRE(ray.has_value());
        const auto plane = Plane3d::TryFromPointNormal(Point3d(0.0, 0.0, 5.0),
                                                       Normal3d::AxisZ());
        REQUIRE(plane.has_value());
        const auto planeHit = Intersect(*ray, *plane);
        REQUIRE(planeHit.has_value());
        REQUIRE(IsNearlyEqual(planeHit->Distance, 5.0));

        const auto triangle = Triangle3d::TryFromPoints(
            Point3d(-1.0, -1.0, 5.0), Point3d(1.0, -1.0, 5.0),
            Point3d(0.0, 1.0, 5.0));
        REQUIRE(triangle.has_value());
        REQUIRE(Intersect(*ray, *triangle).has_value());
        REQUIRE(IsNearlyEqual(
            ClosestPoints(Point3d(0.0, 0.0, 7.0), *triangle).SquaredDistance,
            4.0));

        const auto sphere =
            Sphere3d::TryFromCenterRadius(Point3d(0.0, 0.0, 5.0), 2.0);
        REQUIRE(sphere.has_value());
        const auto sphereHit = Intersect(*ray, *sphere);
        REQUIRE(sphereHit.has_value());
        REQUIRE(IsNearlyEqual(sphereHit->EntryDistance, 3.0));
        REQUIRE(IsNearlyEqual(sphereHit->ExitDistance, 7.0));

        const auto box = Aabb3d::TryFromMinMax(Point3d(-1.0, -1.0, 4.0),
                                               Point3d(1.0, 1.0, 6.0));
        REQUIRE(box.has_value());
        REQUIRE(Intersect(PreparedRay3d(*ray), *box).has_value());
        REQUIRE(Intersects(*sphere, *box));

        const Segment3d first(Point3d(0.0, 0.0, 0.0), Point3d(2.0, 0.0, 0.0));
        const Segment3d second(Point3d(1.0, -1.0, 1.0), Point3d(1.0, 1.0, 1.0));
        REQUIRE(IsNearlyEqual(DistanceSquared(first, second), 1.0));

        const auto integerBox =
            Aabb3i::TryFromMinMax(Point3i(-4, -3, -2), Point3i(4, 3, 2));
        REQUIRE(integerBox.has_value());
        REQUIRE(integerBox->Contains(Point3i(0, 0, 0)));
        REQUIRE(integerBox->TrySize() == Vec3i(8, 6, 4));
        REQUIRE(integerBox->TryClosestPoint(Point3i(10, 0, -10)) ==
                Point3i(4, 0, -2));
        REQUIRE(integerBox->Intersection(Aabb3i::Empty()).IsEmpty());

        static_assert(sizeof(Ray3d) == 64);
        static_assert(sizeof(Aabb3d) == 64);
        static_assert(std::is_trivially_copyable_v<PreparedRay3d>);
        static_assert(std::is_trivially_copyable_v<Aabb3i>);
    }
}  // namespace

TEST_CASE("mv::math ray and plane geometry", "[mv][geometry][ray][plane]")
{
    CheckRayAndPlane();
}

TEST_CASE("mv::math line and segment geometry", "[mv][geometry][linear]")
{
    CheckLineAndSegment();
}

TEST_CASE("mv::math triangle and sphere geometry",
          "[mv][geometry][triangle][sphere]")
{
    CheckTriangleAndSphere();
}

TEST_CASE("mv::math AABB geometry", "[mv][geometry][aabb]")
{
    CheckAabb();
}

TEST_CASE("mv::math linear queries", "[mv][query][linear]")
{
    CheckLinearQueries();
}

TEST_CASE("mv::math closest-point queries", "[mv][query][closest]")
{
    CheckClosestPointQueries();
    CheckBoundsClosestAndSegmentPairs();
}

TEST_CASE("mv::math capsule queries", "[mv][query][capsule]")
{
    CheckCapsule();
}

TEST_CASE("mv::math bounds queries", "[mv][query][bounds]")
{
    CheckBoundsQueries();
}

TEST_CASE("mv::math AABB reference parity", "[mv][query][oracle][aabb]")
{
    CheckAabbReferenceParity();
}

TEST_CASE("mv::math triangle closest-point reference parity",
          "[mv][query][oracle][triangle]")
{
    CheckTriangleClosestReferenceParity();
}

TEST_CASE("mv::math segment-pair reference parity",
          "[mv][query][oracle][segment]")
{
    CheckSegmentPairReferenceParity();
}

TEST_CASE("mv::math geometry layouts", "[mv][geometry][layout]")
{
    CheckLayouts();
}

TEST_CASE("mv::math generic geometry", "[mv][geometry][double][integer]")
{
    CheckGenericGeometry();
}
