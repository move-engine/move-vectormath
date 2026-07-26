#include <mv/math/Rotation3.hpp>

extern "C" void CompileFixturePhaseARotation(
    const mv::math::Rotation3f* rotation,
    const mv::math::Vec3f* input,
    mv::math::Vec3f* output) noexcept
{
    *output = mv::math::Rotate(*rotation, *input);
}
