#include <cmath>
#include <cstdlib>

#include <mv/math/Culling.hpp>
#include <mv/math/Graphics.hpp>
#include <mv/math/Queries.hpp>
#include <mv/math/Transforms.hpp>

namespace
{
    void Require(bool condition)
    {
        if (!condition)
        {
            std::abort();
        }
    }

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
        Require(box.has_value());
        Require(box->Contains(Point3f(0.5F, -1.5F, 2.5F)));
        Require(!box->Contains(Point3f(1.1F, 0.0F, 0.0F)));
        Require(box->ClosestPoint(Point3f(5.0F, -3.0F, 1.0F)) ==
                Point3f(1.0F, -2.0F, 1.0F));
        Require(box->Corners().size() == 8U);

        const auto ray = Ray3f::TryFromOriginDirection(
            Point3f(0.0F, 0.0F, -5.0F), Direction3f::AxisZ());
        const auto hit = Intersect(*ray, *box);
        Require(hit.has_value());
        Require(NearlyEqual(hit->EntryDistance, 2.0F));
        Require(NearlyEqual(hit->ExitDistance, 8.0F));
        Require(hit->EntryNormal.has_value());
        Require(hit->EntryNormal->Vector() == Vec3f(0.0F, 0.0F, -1.0F));
        Require(hit->ExitNormal.Vector() == Vec3f(0.0F, 0.0F, 1.0F));

        const auto overlapping = Obb3f::TryFromCenterHalfExtents(
            Point3f(1.5F, 0.0F, 0.0F), Vec3f(1.0F, 1.0F, 1.0F),
            Rotation3f::FromAxisAngle(Direction3f::AxisY(), Radiansf(0.4F)));
        const auto separated = Obb3f::TryFromCenterHalfExtents(
            Point3f(8.0F, 0.0F, 0.0F), Vec3f(1.0F, 1.0F, 1.0F),
            Rotation3f::FromAxisAngle(Direction3f::AxisZ(), Radiansf(0.7F)));
        Require(Intersects(*box, *overlapping));
        Require(!Intersects(*box, *separated));

        const auto sphere =
            Sphere3f::TryFromCenterRadius(Point3f(1.5F, 0.0F, 0.0F), 0.6F);
        Require(sphere.has_value());
        Require(Intersects(*sphere, *box));

        const RigidTransform3f moved(
            Rotation3f::FromAxisAngle(Direction3f::AxisY(), Radiansf(0.5F)),
            Vec3f(5.0F, 1.0F, -2.0F));
        const Obb3f transformed = TransformObb(moved, *box);
        Require(NearlyEqual(transformed.Center().Vector(),
                            TransformPoint(moved, box->Center()).Vector()));

        const auto doubleBox = Obb3d::TryFromCenterHalfExtents(
            Point3d(1.0, 2.0, 3.0), Vec3d(2.0, 1.0, 4.0));
        Require(doubleBox.has_value());
        Require(doubleBox->Contains(Point3d(2.0, 2.0, 4.0)));
    }

    void CheckFrustumCulling()
    {
        using namespace mv::math;

        const auto projection = TryPerspectiveFov(
            Radiansf(1.0F), 16.0F / 9.0F, 0.1F, 100.0F, Handedness::Left,
            ClipDepth::ZeroToOne, DepthDirection::Forward);
        Require(projection.has_value());
        const auto frustum = Frustum3f::TryFromViewProjection(
            *projection, ClipDepth::ZeroToOne, DepthDirection::Forward);
        Require(frustum.has_value());
        Require(frustum->IsActive(FrustumPlane::Far));
        const PreparedFrustum3f prepared(*frustum);

        Require(Classify(prepared, Point3f(0.0F, 0.0F, 2.0F)).Classification ==
                Containment::Inside);
        Require(
            Classify(prepared, Point3f(100.0F, 0.0F, 2.0F)).Classification ==
            Containment::Outside);

        const auto insideBounds = Aabb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 4.0F), Vec3f(0.5F, 0.5F, 0.5F));
        const auto crossingBounds = Aabb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 0.1F), Vec3f(0.2F, 0.2F, 0.2F));
        Require(Classify(prepared, *insideBounds).Classification ==
                Containment::Inside);
        const CullingResult crossing = Classify(prepared, *crossingBounds);
        Require(crossing.Classification == Containment::Intersecting);
        Require(HasPlane(crossing.RemainingPlanes, FrustumPlane::Near));

        const auto obb = Obb3f::TryFromCenterHalfExtents(
            Point3f(0.0F, 0.0F, 3.0F), Vec3f(0.5F, 1.0F, 0.5F),
            Rotation3f::FromAxisAngle(Direction3f::AxisZ(), Radiansf(0.3F)));
        Require(Classify(prepared, *obb).Classification == Containment::Inside);

        const auto infinite = TryPerspectiveFovInfinite(
            Radiansf(1.0F), 1.0F, 0.1F, Handedness::Left, ClipDepth::ZeroToOne,
            DepthDirection::Reverse);
        const auto infiniteFrustum = Frustum3f::TryFromViewProjection(
            *infinite, ClipDepth::ZeroToOne, DepthDirection::Reverse);
        Require(infiniteFrustum.has_value());
        Require(!infiniteFrustum->IsActive(FrustumPlane::Far));
        Require(Classify(*infiniteFrustum, Point3f(0.0F, 0.0F, 100000.0F)) !=
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
        Require(inverse.has_value());
        Require(viewport.has_value());

        const Point3f world(0.0F, 0.0F, 2.0F);
        const auto ndc = TryProjectPoint(*projection, world);
        const auto unprojected = TryUnprojectPointWithInverse(*inverse, *ndc);
        Require(ndc.has_value());
        Require(unprojected.has_value());
        Require(NearlyEqual(unprojected->Vector(), world.Vector(), 2.0e-5F));

        const auto screen = TryProjectToViewport(*projection, world, *viewport,
                                                 ClipDepth::ZeroToOne);
        Require(screen.has_value());
        Require(NearlyEqual(screen->X(), 400.0F));
        Require(NearlyEqual(screen->Y(), 300.0F));
        const auto viewportWorld = TryUnprojectFromViewportWithInverse(
            *inverse, *screen, *viewport, ClipDepth::ZeroToOne);
        Require(viewportWorld.has_value());
        Require(NearlyEqual(viewportWorld->Vector(), world.Vector(), 2.0e-5F));

        const auto ray =
            TryMakeViewportRay(*inverse, Vec2f(400.0F, 300.0F), *viewport,
                               ClipDepth::ZeroToOne, DepthDirection::Forward);
        Require(ray.has_value());
        Require(NearlyEqual(ray->Origin().Vector(), Vec3f(0.0F, 0.0F, 0.1F),
                            2.0e-5F));
        Require(NearlyEqual(ray->Direction().Vector(), Vec3f(0.0F, 0.0F, 1.0F),
                            2.0e-5F));

        const auto perspectiveRay = TryMakePerspectiveViewportRay(
            *inverse, Point3f::Origin(), Vec2f(400.0F, 300.0F), *viewport,
            ClipDepth::ZeroToOne, DepthDirection::Forward);
        Require(perspectiveRay.has_value());
        Require(perspectiveRay->Origin() == Point3f::Origin());
        Require(NearlyEqual(perspectiveRay->Direction().Vector(),
                            Vec3f(0.0F, 0.0F, 1.0F), 2.0e-5F));
    }
}  // namespace

int main()
{
    CheckObbValuesAndQueries();
    CheckFrustumCulling();
    CheckProjectionAndCameraRays();
    return 0;
}
