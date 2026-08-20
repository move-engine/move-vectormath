#include <cmath>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Culling.hpp>
#include <mv/math/Graphics.hpp>
#include <mv/math/Queries.hpp>
#include <mv/math/Transforms.hpp>

namespace
{
    template <typename T>
    [[nodiscard]] bool NearlyEqual(T left,
                                   T right,
                                   T epsilon = static_cast<T>(1.0e-5))
    {
        return std::abs(left - right) <= epsilon;
    }

    template <typename T>
    [[nodiscard]] bool NearlyEqual(const mv::math::Vec3<T>& left,
                                   const mv::math::Vec3<T>& right,
                                   T epsilon = static_cast<T>(1.0e-5))
    {
        return NearlyEqual(left.X(), right.X(), epsilon) &&
               NearlyEqual(left.Y(), right.Y(), epsilon) &&
               NearlyEqual(left.Z(), right.Z(), epsilon);
    }

    void CheckObbValuesAndQueries()
    {
        using namespace mv::math;

        const auto box = Obb3f::TryFromCenterHalfExtents(
            Point3f::Origin(), Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(box.has_value());
        REQUIRE(box->Contains(Point3f(0.5F, -1.5F, 2.5F)));
        REQUIRE(!box->Contains(Point3f(1.1F, 0.0F, 0.0F)));
        REQUIRE(box->ClosestPoint(Point3f(5.0F, -3.0F, 1.0F)) ==
                Point3f(1.0F, -2.0F, 1.0F));
        REQUIRE(box->Corners().size() == 8U);

        const auto ray = Ray3f::TryFromOriginDirection(
            Point3f(0.0F, 0.0F, -5.0F), Direction3f::AxisZ());
        const auto hit = Intersect(*ray, *box);
        REQUIRE(hit.has_value());
        REQUIRE(NearlyEqual(hit->EntryDistance, 2.0F));
        REQUIRE(NearlyEqual(hit->ExitDistance, 8.0F));
        REQUIRE(hit->EntryNormal.has_value());
        REQUIRE(hit->EntryNormal->Vector() == Vec3f(0.0F, 0.0F, -1.0F));
        REQUIRE(hit->ExitNormal.Vector() == Vec3f(0.0F, 0.0F, 1.0F));

        const auto overlapping = Obb3f::TryFromCenterHalfExtents(
            Point3f(1.5F, 0.0F, 0.0F), Vec3f(1.0F, 1.0F, 1.0F),
            Rotation3f::FromAxisAngle(Direction3f::AxisY(), Radiansf(0.4F)));
        const auto separated = Obb3f::TryFromCenterHalfExtents(
            Point3f(8.0F, 0.0F, 0.0F), Vec3f(1.0F, 1.0F, 1.0F),
            Rotation3f::FromAxisAngle(Direction3f::AxisZ(), Radiansf(0.7F)));
        REQUIRE(Intersects(*box, *overlapping));
        REQUIRE(!Intersects(*box, *separated));

        const auto sphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.5F, 0.0F, 0.0F), 0.6F);
        REQUIRE(sphere.has_value());
        REQUIRE(Intersects(*sphere, *box));

        const RigidTransform3f moved(
            Rotation3f::FromAxisAngle(Direction3f::AxisY(), Radiansf(0.5F)),
            Vec3f(5.0F, 1.0F, -2.0F));
        const Obb3f transformed = TransformObb(moved, *box);
        REQUIRE(NearlyEqual(transformed.Center().Vector(),
                            TransformPoint(moved, box->Center()).Vector()));

        const auto doubleBox = Obb3d::TryFromCenterHalfExtents(
            Point3d(1.0, 2.0, 3.0), Vec3d(2.0, 1.0, 4.0));
        REQUIRE(doubleBox.has_value());
        REQUIRE(doubleBox->Contains(Point3d(2.0, 2.0, 4.0)));
    }

    void CheckFrustumCulling()
    {
        using namespace mv::math;

        const auto projection = TryPerspectiveFov(
            Radiansf(1.0F), 16.0F / 9.0F, 0.1F, 100.0F, Handedness::Left,
            ClipDepth::ZeroToOne, DepthDirection::Forward);
        REQUIRE(projection.has_value());
        const auto frustum = Frustum3f::TryFromViewProjection(
            *projection, ClipDepth::ZeroToOne, DepthDirection::Forward);
        REQUIRE(frustum.has_value());
        REQUIRE(frustum->IsActive(FrustumPlane::Far));
        const PreparedFrustum3f prepared(*frustum);

        REQUIRE(Classify(prepared, Point3f(0.0F, 0.0F, 2.0F)).Classification ==
                Containment::Inside);
        REQUIRE(
            Classify(prepared, Point3f(100.0F, 0.0F, 2.0F)).Classification ==
            Containment::Outside);

        const auto insideBounds = Aabb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 4.0F), Vec3f(0.5F, 0.5F, 0.5F));
        const auto crossingBounds = Aabb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 0.1F), Vec3f(0.2F, 0.2F, 0.2F));
        REQUIRE(Classify(prepared, *insideBounds).Classification ==
                Containment::Inside);
        const CullingResult crossing = Classify(prepared, *crossingBounds);
        REQUIRE(crossing.Classification == Containment::Intersecting);
        REQUIRE(HasPlane(crossing.RemainingPlanes, FrustumPlane::Near));

        const auto obb = Obb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 3.0F), Vec3f(0.5F, 1.0F, 0.5F),
            Rotation3f::FromAxisAngle(Direction3f::AxisZ(), Radiansf(0.3F)));
        REQUIRE(Classify(prepared, *obb).Classification == Containment::Inside);

        const auto infinite = TryPerspectiveFovInfinite(
            Radiansf(1.0F), 1.0F, 0.1F, Handedness::Left, ClipDepth::ZeroToOne,
            DepthDirection::Reverse);
        const auto infiniteFrustum = Frustum3f::TryFromViewProjection(
            *infinite, ClipDepth::ZeroToOne, DepthDirection::Reverse);
        REQUIRE(infiniteFrustum.has_value());
        REQUIRE(!infiniteFrustum->IsActive(FrustumPlane::Far));
        REQUIRE(Classify(*infiniteFrustum, Point3f(0.0F, 0.0F, 100000.0F)) !=
                Containment::Outside);
    }

    void CheckProjectionAndCameraRays()
    {
        using namespace mv::math;

        const auto projection = TryPerspectiveFov(
            Radiansf(1.0F), 4.0F / 3.0F, 0.1F, 100.0F, Handedness::Left,
            ClipDepth::ZeroToOne, DepthDirection::Forward);
        const auto inverse = projection->TryInverse();
        const auto viewport = Viewportf::TryFrom(0.0F, 0.0F, 800.0F, 600.0F);
        REQUIRE(inverse.has_value());
        REQUIRE(viewport.has_value());

        const Point3f world(0.0F, 0.0F, 2.0F);
        const auto ndc = TryProjectPoint(*projection, world);
        const auto unprojected = TryUnprojectPointWithInverse(*inverse, *ndc);
        REQUIRE(ndc.has_value());
        REQUIRE(unprojected.has_value());
        REQUIRE(NearlyEqual(unprojected->Vector(), world.Vector(), 2.0e-5F));

        const auto screen = TryProjectToViewport(*projection, world, *viewport,
                                                 ClipDepth::ZeroToOne);
        REQUIRE(screen.has_value());
        REQUIRE(NearlyEqual(screen->X(), 400.0F));
        REQUIRE(NearlyEqual(screen->Y(), 300.0F));
        const auto viewportWorld = TryUnprojectFromViewportWithInverse(
            *inverse, *screen, *viewport, ClipDepth::ZeroToOne);
        REQUIRE(viewportWorld.has_value());
        REQUIRE(NearlyEqual(viewportWorld->Vector(), world.Vector(), 2.0e-5F));

        const auto ray =
            TryMakeViewportRay(*inverse, Vec2f(400.0F, 300.0F), *viewport,
                               ClipDepth::ZeroToOne, DepthDirection::Forward);
        REQUIRE(ray.has_value());
        REQUIRE(NearlyEqual(ray->Origin().Vector(), Vec3f(0.0F, 0.0F, 0.1F),
                            2.0e-5F));
        REQUIRE(NearlyEqual(ray->Direction().Vector(), Vec3f(0.0F, 0.0F, 1.0F),
                            2.0e-5F));

        const auto perspectiveRay = TryMakePerspectiveViewportRay(
            *inverse, Point3f::Origin(), Vec2f(400.0F, 300.0F), *viewport,
            ClipDepth::ZeroToOne, DepthDirection::Forward);
        REQUIRE(perspectiveRay.has_value());
        REQUIRE(perspectiveRay->Origin() == Point3f::Origin());
        REQUIRE(NearlyEqual(perspectiveRay->Direction().Vector(),
                            Vec3f(0.0F, 0.0F, 1.0F), 2.0e-5F));
    }
}  // namespace

TEST_CASE("mv::math OBB values and queries", "[mv][geometry][obb]")
{
    CheckObbValuesAndQueries();
}

TEST_CASE("mv::math frustum culling", "[mv][culling][frustum]")
{
    CheckFrustumCulling();
}

TEST_CASE("mv::math projection and camera rays", "[mv][graphics][camera]")
{
    CheckProjectionAndCameraRays();
}
