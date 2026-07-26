#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mv/math/Vec3.hpp>
#include <mv/math/packed/Vectors.hpp>

namespace mv::math::gpu
{
    using Float3Packed = PackedVec3f;

    struct alignas(16) Float3Slot16
    {
        float X{};
        float Y{};
        float Z{};
        std::uint32_t Padding{};
    };

    [[nodiscard]] inline Vec3f Load(const Float3Slot16& value) noexcept
    {
        return Vec3f(value.X, value.Y, value.Z);
    }

    inline void Store(Float3Slot16& destination, Vec3f value) noexcept
    {
        destination = {value.X(), value.Y(), value.Z(), 0U};
    }

    [[nodiscard]] inline Float3Slot16 Store(Vec3f value) noexcept
    {
        return {value.X(), value.Y(), value.Z(), 0U};
    }
}  // namespace mv::math::gpu

static_assert(sizeof(mv::math::gpu::Float3Slot16) == 16);
static_assert(alignof(mv::math::gpu::Float3Slot16) == 16);
static_assert(offsetof(mv::math::gpu::Float3Slot16, X) == 0);
static_assert(offsetof(mv::math::gpu::Float3Slot16, Y) == 4);
static_assert(offsetof(mv::math::gpu::Float3Slot16, Z) == 8);
static_assert(offsetof(mv::math::gpu::Float3Slot16, Padding) == 12);
static_assert(std::is_trivially_copyable_v<mv::math::gpu::Float3Slot16>);
static_assert(std::is_standard_layout_v<mv::math::gpu::Float3Slot16>);
