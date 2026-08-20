#include <mv/math/Transforms.hpp>

mv::math::Normal3f RotateNormal(const mv::math::RigidTransform3f& transform,
                                mv::math::Normal3f normal)
{
    return mv::math::TransformNormal(transform, normal);
}
