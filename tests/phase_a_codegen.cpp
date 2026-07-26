#include <cstddef>

#include <mv/math/PhaseA.hpp>
#include <mv/math/experimental/RepresentationProof.hpp>

#if defined(_MSC_VER)
#define MV_PHASE_A_NOINLINE __declspec(noinline)
#else
#define MV_PHASE_A_NOINLINE __attribute__((noinline))
#endif

extern "C" MV_PHASE_A_NOINLINE void PhaseAPublicVec3Chain(
    const mv::math::PackedVec3f* left,
    const mv::math::PackedVec3f* right,
    mv::math::PackedVec3f* output,
    std::size_t count,
    float scale) noexcept
{
    for (std::size_t index = 0; index < count; ++index)
    {
        const mv::math::Vec3f value =
            (mv::math::Load(left[index]) + mv::math::Load(right[index])) *
            scale;
        mv::math::Store(output[index], value);
    }
}

extern "C" MV_PHASE_A_NOINLINE void PhaseAFixedStorageVec3Chain(
    const mv::math::PackedVec3f* left,
    const mv::math::PackedVec3f* right,
    mv::math::PackedVec3f* output,
    std::size_t count,
    float scale) noexcept
{
    using mv::math::experimental::FixedStorageVec3f;

    for (std::size_t index = 0; index < count; ++index)
    {
        const FixedStorageVec3f leftValue(left[index].X, left[index].Y,
                                          left[index].Z);
        const FixedStorageVec3f rightValue(right[index].X, right[index].Y,
                                           right[index].Z);
        const FixedStorageVec3f value = (leftValue + rightValue) * scale;
        value.Store(&output[index].X);
    }
}

extern "C" MV_PHASE_A_NOINLINE void PhaseANativeVec3Chain(
    const mv::math::PackedVec3f* left,
    const mv::math::PackedVec3f* right,
    mv::math::PackedVec3f* output,
    std::size_t count,
    float scale) noexcept
{
    using mv::math::experimental::NativeVec3f;

    for (std::size_t index = 0; index < count; ++index)
    {
        const NativeVec3f leftValue(left[index].X, left[index].Y,
                                    left[index].Z);
        const NativeVec3f rightValue(right[index].X, right[index].Y,
                                     right[index].Z);
        const NativeVec3f value = (leftValue + rightValue) * scale;
        value.Store(&output[index].X);
    }
}

extern "C" MV_PHASE_A_NOINLINE void PhaseATransformPackedToGpu(
    const mv::math::PackedVec3f* input,
    mv::math::gpu::Float3Slot16* output,
    std::size_t count,
    const mv::math::AffineTransform3f* transform) noexcept
{
    mv::math::TransformPoints(
        std::span<const mv::math::PackedVec3f>(input, count), *transform,
        std::span<mv::math::gpu::Float3Slot16>(output, count));
}

extern "C" MV_PHASE_A_NOINLINE void PhaseACompactVec2Chain(
    mv::math::Vec2f* values,
    std::size_t count,
    mv::math::Vec2f increment,
    float scale) noexcept
{
    for (std::size_t index = 0; index < count; ++index)
    {
        values[index] = (values[index] + increment) * scale;
    }
}

extern "C" MV_PHASE_A_NOINLINE void PhaseAAlignedVec2Chain(
    mv::math::experimental::AlignedVec2f16* values,
    std::size_t count,
    mv::math::experimental::AlignedVec2f16 increment,
    float scale) noexcept
{
    for (std::size_t index = 0; index < count; ++index)
    {
        values[index] = (values[index] + increment) * scale;
    }
}

extern "C" MV_PHASE_A_NOINLINE void PhaseARotateVector(
    const mv::math::Rotation3f* rotation,
    const mv::math::Vec3f* vector,
    mv::math::PackedVec3f* output) noexcept
{
    mv::math::Store(*output, mv::math::Rotate(*rotation, *vector));
}
