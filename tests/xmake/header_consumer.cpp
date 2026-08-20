#include <mv/math/Math.hpp>

int main()
{
    using namespace mv::math;

    const auto projection = TryPerspectiveFov(
        Radiansf(1.0F), 16.0F / 9.0F, 0.1F, 1000.0F, Handedness::Left,
        ClipDepth::ZeroToOne, DepthDirection::Reverse);
    const auto bounds = Obb3f::TryFromCenterHalfExtents(
        Point3f(0.0F, 0.0F, 3.0F), Vec3f(1.0F, 2.0F, 1.0F));
    if (!projection || !bounds)
    {
        return 1;
    }

    const auto frustum = Frustum3f::TryFromViewProjection(
        *projection, ClipDepth::ZeroToOne, DepthDirection::Reverse);
    return frustum && Classify(*frustum, *bounds) != Containment::Outside ? 0
                                                                          : 1;
}
