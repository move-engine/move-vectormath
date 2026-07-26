#include <mv/math/PhaseA.hpp>

extern "C" void CompileFixturePhaseAUmbrella(
    const mv::math::AffineTransform3f* transform,
    const mv::math::PackedVec3f* input,
    mv::math::gpu::Float3Slot16* output) noexcept
{
    const mv::math::Vec3f point = mv::math::Load(*input);
    mv::math::gpu::Store(*output, mv::math::TransformPoint(*transform, point));
}
