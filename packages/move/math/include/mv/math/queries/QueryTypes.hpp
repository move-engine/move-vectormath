#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    enum class FaceOrientation
    {
        Front,
        Back
    };

    struct RayPlaneHit3f
    {
        float Distance;
        Point3f Point;
        Normal3f Normal;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(
            const RayPlaneHit3f&, const RayPlaneHit3f&) noexcept = default;
    };

    struct RayTriangleHit3f
    {
        float Distance;
        Point3f Point;
        Normal3f GeometricNormal;
        Vec3f Barycentric;
        FaceOrientation Face;

        [[nodiscard]] friend bool operator==(const RayTriangleHit3f&,
                                             const RayTriangleHit3f&) noexcept =
            default;
    };

    struct RaySphereHit3f
    {
        float EntryDistance;
        float ExitDistance;
        bool StartsInside;
        std::optional<Normal3f> EntryNormal;
        std::optional<Normal3f> ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RaySphereHit3f&, const RaySphereHit3f&) noexcept = default;
    };

    struct RayAabbHit3f
    {
        float EntryDistance;
        float ExitDistance;
        bool StartsInside;
        std::optional<Normal3f> EntryNormal;
        Normal3f ExitNormal;

        [[nodiscard]] friend bool operator==(
            const RayAabbHit3f&, const RayAabbHit3f&) noexcept = default;
    };
}  // namespace mv::math

static_assert(std::is_trivially_copyable_v<mv::math::RayPlaneHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayTriangleHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RaySphereHit3f>);
static_assert(std::is_trivially_copyable_v<mv::math::RayAabbHit3f>);
