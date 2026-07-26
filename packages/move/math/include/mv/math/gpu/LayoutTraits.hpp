#pragma once

#include <cstddef>
#include <span>
#include <type_traits>

#include <mv/math/Vec3.hpp>
#include <mv/math/gpu/Vectors.hpp>

namespace mv::math
{
    enum class GpuLayout
    {
        Float3Packed12,
        Float3Array16
    };

    template <typename T, GpuLayout Layout>
    inline constexpr bool IsGpuLayoutCompatible = false;

    template <>
    inline constexpr bool
        IsGpuLayoutCompatible<PackedVec3f, GpuLayout::Float3Packed12> = true;

    template <>
    inline constexpr bool
        IsGpuLayoutCompatible<gpu::Float3Slot16, GpuLayout::Float3Array16> =
            true;

    template <>
    inline constexpr bool
        IsGpuLayoutCompatible<Vec3f, GpuLayout::Float3Array16> = true;

    template <GpuLayout Layout, typename T>
        requires IsGpuLayoutCompatible<T, Layout>
    [[nodiscard]] std::span<const std::byte> AsGpuBytes(
        std::span<const T> values) noexcept
    {
        return std::as_bytes(values);
    }
}  // namespace mv::math

static_assert(
    mv::math::IsGpuLayoutCompatible<mv::math::PackedVec3f,
                                    mv::math::GpuLayout::Float3Packed12>);
static_assert(
    mv::math::IsGpuLayoutCompatible<mv::math::Vec3f,
                                    mv::math::GpuLayout::Float3Array16>);
static_assert(
    mv::math::IsGpuLayoutCompatible<mv::math::gpu::Float3Slot16,
                                    mv::math::GpuLayout::Float3Array16>);
