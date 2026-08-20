#define MV_MATH_JOIN_IMPL(left, right) left##right
#define MV_MATH_JOIN(left, right) MV_MATH_JOIN_IMPL(left, right)

float MV_MATH_JOIN(MoveMathCompileWorkload, MV_MATH_COMPILE_UNIT)(float seed)
{
    using namespace mv::math;

    const auto direction =
        Direction3f::TryFrom(Vec3f(1.0F, seed * 0.01F, 1.0F));
    if (!direction)
    {
        return seed;
    }

    const Rotation3f rotation =
        Rotation3f::FromAxisAngle(Direction3f::AxisY(), Radiansf(seed));
    const RigidTransform3f transform(rotation, Vec3f(seed, 2.0F, 3.0F));
    const Point3f point = TransformPoint(transform, Point3f(0.5F, -1.0F, 2.0F));
    const auto ray = Ray3f::TryFromOriginDirection(point, *direction);
    const auto sphere =
        Sphere3f::TryFromCenterRadius(Point3f(seed + 4.0F, 2.0F, 3.0F), 2.0F);
    const auto projection = TryPerspectiveFov(
        Radiansf(1.0F), 16.0F / 9.0F, 0.1F, 1000.0F, Handedness::Left,
        ClipDepth::ZeroToOne, DepthDirection::Reverse);
    const auto bounds = Obb3f::TryFromCenterHalfExtents(
        Point3f(0.0F, 0.0F, 3.0F), Vec3f(1.0F, 2.0F, 1.0F));
    if (!ray || !sphere || !projection || !bounds)
    {
        return seed;
    }

    const auto hit = Intersect(*ray, *sphere);
    const auto frustum = Frustum3f::TryFromViewProjection(
        *projection, ClipDepth::ZeroToOne, DepthDirection::Reverse);
    const float hitDistance = hit ? hit->EntryDistance : 0.0F;
    const float visibility =
        frustum && Classify(*frustum, *bounds) != Containment::Outside ? 1.0F
                                                                       : 0.0F;
    return point.X() + hitDistance + visibility;
}

#undef MV_MATH_JOIN
#undef MV_MATH_JOIN_IMPL
#undef MV_MATH_COMPILE_UNIT
