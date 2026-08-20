#include <mv/math/Core.hpp>

extern "C" void CompileFixtureCoreUmbrella(const mv::math::Rotation3f* rotation,
                                           const mv::math::Vec3f* input,
                                           mv::math::Vec3f* output) noexcept
{
    *output = mv::math::Rotate(*rotation, *input);
}
