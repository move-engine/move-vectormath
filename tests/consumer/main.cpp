#include <move/vectormath.hpp>
#include <mv/math/PhaseC.hpp>

int main()
{
    using namespace move::math;

    const float3 position(1.0F, 2.0F, 3.0F);
    const mat4x4f transform = mat4x4f::trs(float3(4.0F, 5.0F, 6.0F),
                                           quatf::identity(), float3::one());
    const float3 transformed = transform.transform_point(position);

    if (!approx_equal(transformed, float3(5.0F, 7.0F, 9.0F), 0.0001F))
    {
        return 1;
    }

    const mv::math::RigidTransform3f rigid(mv::math::Rotation3f::Identity(),
                                           mv::math::Vec3f(4.0F, 5.0F, 6.0F));
    const mv::math::Point3f semanticPoint(1.0F, 2.0F, 3.0F);
    const mv::math::Point3f semanticResult =
        mv::math::TransformPoint(rigid, semanticPoint);

    if (semanticResult != mv::math::Point3f(5.0F, 7.0F, 9.0F))
    {
        return 1;
    }

    const auto bounds =
        mv::math::Aabb3f::TryFromMinMax(mv::math::Point3f(-1.0F, -1.0F, -1.0F),
                                        mv::math::Point3f(1.0F, 1.0F, 1.0F));
    if (!bounds)
    {
        return 1;
    }

    const mv::math::Ray3f ray(mv::math::Point3f(-3.0F, 0.0F, 0.0F),
                              mv::math::Direction3f::AxisX());
    return mv::math::Intersects(ray, *bounds) ? 0 : 1;
}
