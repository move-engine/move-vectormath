#include <cstddef>

#include <mv/math/PhaseB.hpp>

extern "C" void mv_phase_b_transform_points(
    const mv::math::Point3f* input,
    mv::math::Point3f* output,
    std::size_t count,
    const mv::math::RigidTransform3f* transform)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = mv::math::TransformPoint(*transform, input[index]);
    }
}

extern "C" void mv_phase_b_transform_raw_points(
    const mv::math::Vec3f* input,
    mv::math::Vec3f* output,
    std::size_t count,
    const mv::math::RigidTransform3f* transform)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = mv::math::Rotate(transform->Rotation(), input[index]) +
                        transform->Translation();
    }
}

extern "C" void mv_phase_b_transform_directions(
    const mv::math::Direction3f* input,
    mv::math::Direction3f* output,
    std::size_t count,
    const mv::math::RigidTransform3f* transform)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = mv::math::TransformDirection(*transform, input[index]);
    }
}
