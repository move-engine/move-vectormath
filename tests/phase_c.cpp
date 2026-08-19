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

    void CheckLayouts()
    {
        using namespace mv::math;
        static_assert(sizeof(Ray3f) == 32);
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
    CheckTriangleAndSphere();
    CheckAabb();
    CheckLinearQueries();
    CheckBoundsQueries();
    CheckAabbReferenceParity();
    CheckLayouts();
    return 0;
}
