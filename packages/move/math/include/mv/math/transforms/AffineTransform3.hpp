#pragma once

#include <type_traits>

#include <mv/math/Vec3.hpp>

namespace mv::math
{
    // Standard affine point/vector transform; storage and semantic separation
    // are Move-specific.
    template <typename T>
        requires std::is_floating_point_v<T>
    class AffineTransform3
    {
    public:
        using Component = T;

        AffineTransform3() noexcept :
            AxisX_(T(1), T(0), T(0)),
            AxisY_(T(0), T(1), T(0)),
            AxisZ_(T(0), T(0), T(1)),
            Translation_(T(0), T(0), T(0))
        {
        }

        AffineTransform3(const Vec3<T>& axisX,
                         const Vec3<T>& axisY,
                         const Vec3<T>& axisZ,
                         const Vec3<T>& translation) noexcept :
            AxisX_(axisX),
            AxisY_(axisY),
            AxisZ_(axisZ),
            Translation_(translation)
        {
        }

        [[nodiscard]] const Vec3<T>& AxisX() const noexcept
        {
            return AxisX_;
        }
        [[nodiscard]] const Vec3<T>& AxisY() const noexcept
        {
            return AxisY_;
        }
        [[nodiscard]] const Vec3<T>& AxisZ() const noexcept
        {
            return AxisZ_;
        }
        [[nodiscard]] const Vec3<T>& Translation() const noexcept
        {
            return Translation_;
        }

    private:
        Vec3<T> AxisX_;
        Vec3<T> AxisY_;
        Vec3<T> AxisZ_;
        Vec3<T> Translation_;
    };

    using AffineTransform3f = AffineTransform3<float>;
    using AffineTransform3d = AffineTransform3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> TransformVector(
        const AffineTransform3<T>& transform, const Vec3<T>& vector) noexcept
    {
        return transform.AxisX() * vector.X() + transform.AxisY() * vector.Y() +
               transform.AxisZ() * vector.Z();
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> TransformPoint(
        const AffineTransform3<T>& transform, const Vec3<T>& point) noexcept
    {
        return TransformVector(transform, point) + transform.Translation();
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::AffineTransform3f) == 64);
static_assert(alignof(mv::math::AffineTransform3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::AffineTransform3f>);
static_assert(sizeof(mv::math::AffineTransform3d) == 128);
static_assert(alignof(mv::math::AffineTransform3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::AffineTransform3d>);
