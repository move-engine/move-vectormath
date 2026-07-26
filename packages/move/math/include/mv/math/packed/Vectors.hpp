#pragma once

#include <cstddef>
#include <type_traits>

#include <mv/math/Vec2.hpp>
#include <mv/math/Vec3.hpp>
#include <mv/math/Vec4.hpp>

namespace mv::math
{
    struct PackedVec2f
    {
        float X{};
        float Y{};

        [[nodiscard]] friend constexpr bool operator==(
            PackedVec2f, PackedVec2f) noexcept = default;
    };

    struct PackedVec3f
    {
        float X{};
        float Y{};
        float Z{};

        [[nodiscard]] friend constexpr bool operator==(
            PackedVec3f, PackedVec3f) noexcept = default;
    };

    struct PackedVec4f
    {
        float X{};
        float Y{};
        float Z{};
        float W{};

        [[nodiscard]] friend constexpr bool operator==(
            PackedVec4f, PackedVec4f) noexcept = default;
    };

    [[nodiscard]] inline Vec2f Load(const PackedVec2f& value) noexcept
    {
        return Vec2f(value.X, value.Y);
    }

    [[nodiscard]] inline Vec3f Load(const PackedVec3f& value) noexcept
    {
        return Vec3f(value.X, value.Y, value.Z);
    }

    [[nodiscard]] inline Vec4f Load(const PackedVec4f& value) noexcept
    {
        return Vec4f(value.X, value.Y, value.Z, value.W);
    }

    inline void Store(PackedVec2f& destination, Vec2f value) noexcept
    {
        destination = {value.X(), value.Y()};
    }

    inline void Store(PackedVec3f& destination, Vec3f value) noexcept
    {
        destination = {value.X(), value.Y(), value.Z()};
    }

    inline void Store(PackedVec4f& destination, Vec4f value) noexcept
    {
        destination = {value.X(), value.Y(), value.Z(), value.W()};
    }

    [[nodiscard]] inline PackedVec2f Store(Vec2f value) noexcept
    {
        return {value.X(), value.Y()};
    }

    [[nodiscard]] inline PackedVec3f Store(Vec3f value) noexcept
    {
        return {value.X(), value.Y(), value.Z()};
    }

    [[nodiscard]] inline PackedVec4f Store(Vec4f value) noexcept
    {
        return {value.X(), value.Y(), value.Z(), value.W()};
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::PackedVec2f) == 8);
static_assert(alignof(mv::math::PackedVec2f) == 4);
static_assert(offsetof(mv::math::PackedVec2f, X) == 0);
static_assert(offsetof(mv::math::PackedVec2f, Y) == 4);
static_assert(std::is_trivially_copyable_v<mv::math::PackedVec2f>);
static_assert(std::is_standard_layout_v<mv::math::PackedVec2f>);

static_assert(sizeof(mv::math::PackedVec3f) == 12);
static_assert(alignof(mv::math::PackedVec3f) == 4);
static_assert(offsetof(mv::math::PackedVec3f, X) == 0);
static_assert(offsetof(mv::math::PackedVec3f, Y) == 4);
static_assert(offsetof(mv::math::PackedVec3f, Z) == 8);
static_assert(std::is_trivially_copyable_v<mv::math::PackedVec3f>);
static_assert(std::is_standard_layout_v<mv::math::PackedVec3f>);

static_assert(sizeof(mv::math::PackedVec4f) == 16);
static_assert(alignof(mv::math::PackedVec4f) == 4);
static_assert(std::is_trivially_copyable_v<mv::math::PackedVec4f>);
static_assert(std::is_standard_layout_v<mv::math::PackedVec4f>);
