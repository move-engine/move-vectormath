#include <mv/math/transforms/RigidTransform3.hpp>

mv::math::Point3f TransformPointFixture(
    const mv::math::RigidTransform3f& transform, mv::math::Point3f point)
{
    return mv::math::TransformPoint(transform, point);
}
