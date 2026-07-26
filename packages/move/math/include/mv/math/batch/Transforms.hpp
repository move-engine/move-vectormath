#pragma once

#include <algorithm>
#include <cassert>
#include <span>

#include <mv/math/batch/StridedSpan.hpp>
#include <mv/math/gpu/Vectors.hpp>
#include <mv/math/packed/Vectors.hpp>
#include <mv/math/transforms/AffineTransform3.hpp>

namespace mv::math
{
    inline void TransformPoints(std::span<const PackedVec3f> input,
                                const AffineTransform3f& transform,
                                std::span<gpu::Float3Slot16> output) noexcept
    {
        assert(output.size() >= input.size());
        for (std::size_t index = 0; index < input.size(); ++index)
        {
            gpu::Store(output[index],
                       TransformPoint(transform, Load(input[index])));
        }
    }

    inline void TransformPoints(StridedSpan<const PackedVec3f> input,
                                const AffineTransform3f& transform,
                                StridedSpan<PackedVec3f> output) noexcept
    {
        assert(output.Size() >= input.Size());
        for (std::size_t index = 0; index < input.Size(); ++index)
        {
            Store(output[index], TransformPoint(transform, Load(input[index])));
        }
    }
}  // namespace mv::math
