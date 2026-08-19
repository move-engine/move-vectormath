#pragma once

#include <mv/math/Vec3.hpp>

namespace mv::math
{
    // Standard affine point/vector transform; storage and semantic separation
    // are Move-specific.
    class AffineTransform3f
    {
    public:
        AffineTransform3f() noexcept :
            AxisX_(1.0F, 0.0F, 0.0F),
            AxisY_(0.0F, 1.0F, 0.0F),
            AxisZ_(0.0F, 0.0F, 1.0F),
            Translation_(0.0F, 0.0F, 0.0F)
        {
        }

        AffineTransform3f(Vec3f axisX,
                          Vec3f axisY,
                          Vec3f axisZ,
                          Vec3f translation) noexcept :
            AxisX_(axisX),
            AxisY_(axisY),
            AxisZ_(axisZ),
            Translation_(translation)
        {
        }

        [[nodiscard]] Vec3f AxisX() const noexcept
        {
            return AxisX_;
        }
        [[nodiscard]] Vec3f AxisY() const noexcept
        {
            return AxisY_;
        }
        [[nodiscard]] Vec3f AxisZ() const noexcept
        {
            return AxisZ_;
        }
        [[nodiscard]] Vec3f Translation() const noexcept
        {
            return Translation_;
        }

    private:
        Vec3f AxisX_;
        Vec3f AxisY_;
        Vec3f AxisZ_;
        Vec3f Translation_;
    };

    [[nodiscard]] inline Vec3f TransformVector(
        const AffineTransform3f& transform, const Vec3f& vector) noexcept
    {
        return transform.AxisX() * vector.X() + transform.AxisY() * vector.Y() +
               transform.AxisZ() * vector.Z();
    }

    [[nodiscard]] inline Vec3f TransformPoint(
        const AffineTransform3f& transform, const Vec3f& point) noexcept
    {
        return TransformVector(transform, point) + transform.Translation();
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::AffineTransform3f) == 64);
static_assert(alignof(mv::math::AffineTransform3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::AffineTransform3f>);
