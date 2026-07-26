#pragma once

#include <cstddef>
#include <type_traits>

#include <mv/math/gpu/Vectors.hpp>

namespace mv::math::gpu
{
    struct alignas(16) PositionRadius16
    {
        PackedVec3f Position{};
        float Radius{};
    };

    struct alignas(16) Aabb3fSlots16
    {
        Float3Slot16 Minimum{};
        Float3Slot16 Maximum{};
    };
}  // namespace mv::math::gpu

static_assert(sizeof(mv::math::gpu::PositionRadius16) == 16);
static_assert(alignof(mv::math::gpu::PositionRadius16) == 16);
static_assert(offsetof(mv::math::gpu::PositionRadius16, Position) == 0);
static_assert(offsetof(mv::math::gpu::PositionRadius16, Radius) == 12);
static_assert(std::is_trivially_copyable_v<mv::math::gpu::PositionRadius16>);
static_assert(std::is_standard_layout_v<mv::math::gpu::PositionRadius16>);

static_assert(sizeof(mv::math::gpu::Aabb3fSlots16) == 32);
static_assert(alignof(mv::math::gpu::Aabb3fSlots16) == 16);
static_assert(offsetof(mv::math::gpu::Aabb3fSlots16, Minimum) == 0);
static_assert(offsetof(mv::math::gpu::Aabb3fSlots16, Maximum) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::gpu::Aabb3fSlots16>);
static_assert(std::is_standard_layout_v<mv::math::gpu::Aabb3fSlots16>);
