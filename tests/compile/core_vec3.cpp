#include <mv/math/Vec3.hpp>

extern "C" void CompileFixtureCoreVec3(const mv::math::Vec3f* left,
                                       const mv::math::Vec3f* right,
                                       float scale,
                                       mv::math::Vec3f* output) noexcept
{
    *output = (*left + *right) * scale;
}
