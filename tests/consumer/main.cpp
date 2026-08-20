#include <mv/math/Math.hpp>

int main()
{
    using namespace mv::math;

    const RigidTransform3f rigid(Rotation3f::Identity(),
                                 Vec3f(4.0F, 5.0F, 6.0F));
    const Point3f semanticPoint(1.0F, 2.0F, 3.0F);
    const Point3f semanticResult = TransformPoint(rigid, semanticPoint);

    if (semanticResult != Point3f(5.0F, 7.0F, 9.0F))
    {
        return 1;
    }

    const auto bounds = Aabb3f::TryFromMinMax(Point3f(-1.0F, -1.0F, -1.0F),
                                              Point3f(1.0F, 1.0F, 1.0F));
    if (!bounds)
    {
        return 1;
    }

    const auto ray = Ray3f::TryFromOriginDirection(Point3f(-3.0F, 0.0F, 0.0F),
                                                   Direction3f::AxisX());
    return ray && Intersects(*ray, *bounds) ? 0 : 1;
}
