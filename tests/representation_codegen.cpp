#include <cstddef>

#include <mv/math/Math.hpp>
#include "support/RepresentationFixtures.hpp"

#if defined(_MSC_VER)
#define MV_REPRESENTATION_NOINLINE __declspec(noinline)
#else
#define MV_REPRESENTATION_NOINLINE __attribute__((noinline))
#endif

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationPublicVec3Chain(
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

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationFixedStorageVec3Chain(
    const mv::math::PackedVec3f* left,
    const mv::math::PackedVec3f* right,
    mv::math::PackedVec3f* output,
    std::size_t count,
    float scale) noexcept
{
    using mv::math::test::FixedStorageVec3f;

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

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationNativeVec3Chain(
    const mv::math::PackedVec3f* left,
    const mv::math::PackedVec3f* right,
    mv::math::PackedVec3f* output,
    std::size_t count,
    float scale) noexcept
{
    using mv::math::test::NativeVec3f;

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

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationTransformPackedToGpu(
    const mv::math::PackedVec3f* input,
    mv::math::gpu::Float3Slot16* output,
    std::size_t count,
    const mv::math::AffineTransform3f* transform) noexcept
{
    mv::math::TransformPoints(
        std::span<const mv::math::PackedVec3f>(input, count), *transform,
        std::span<mv::math::gpu::Float3Slot16>(output, count));
}

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationCompactVec2Chain(
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

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationAlignedVec2Chain(
    mv::math::test::AlignedVec2f16* values,
    std::size_t count,
    mv::math::test::AlignedVec2f16 increment,
    float scale) noexcept
{
    for (std::size_t index = 0; index < count; ++index)
    {
        values[index] = (values[index] + increment) * scale;
    }
}

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationRotateVector(
    const mv::math::Rotation3f* rotation,
    const mv::math::Vec3f* vector,
    mv::math::PackedVec3f* output) noexcept
{
    mv::math::Store(*output, mv::math::Rotate(*rotation, *vector));
}

extern "C" MV_REPRESENTATION_NOINLINE void RepresentationVec3MinMax(
    const mv::math::Vec3f* left,
    const mv::math::Vec3f* right,
    mv::math::Vec3f* minimum,
    mv::math::Vec3f* maximum) noexcept
{
    *minimum = mv::math::Min(*left, *right);
    *maximum = mv::math::Max(*left, *right);
}
