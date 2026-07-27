#pragma once

#include <type_traits>

#include <mv/math/Rotation3.hpp>
#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    class RigidTransform3f
    {
    public:
        constexpr RigidTransform3f() noexcept = default;

        RigidTransform3f(Rotation3f rotation, Vec3f translation) noexcept :
            Rotation_(rotation), Translation_(translation)
        {
        }

        [[nodiscard]] static RigidTransform3f Identity() noexcept
        {
            return RigidTransform3f();
        }

        [[nodiscard]] constexpr const Rotation3f& Rotation() const noexcept
        {
            return Rotation_;
        }

        [[nodiscard]] const Vec3f& Translation() const noexcept
        {
            return Translation_;
        }

        [[nodiscard]] RigidTransform3f Inverse() const noexcept
        {
            const Rotation3f inverseRotation = Rotation_.Inverse();
            return RigidTransform3f(
                inverseRotation, Rotate(inverseRotation, Translation_ * -1.0F));
        }

        [[nodiscard]] friend bool operator==(
            const RigidTransform3f& left,
            const RigidTransform3f& right) noexcept
        {
            return left.Rotation_ == right.Rotation_ &&
                   left.Translation_ == right.Translation_;
        }

    private:
        Rotation3f Rotation_{};
        Vec3f Translation_{};
    };

    [[nodiscard]] inline Vec3f TransformVector(
        const RigidTransform3f& transform, const Vec3f& vector) noexcept
    {
        return Rotate(transform.Rotation(), vector);
    }

    [[nodiscard]] inline Point3f TransformPoint(
        const RigidTransform3f& transform, Point3f point) noexcept
    {
        return Point3f::FromVector(
            Rotate(transform.Rotation(), point.Vector()) +
            transform.Translation());
    }

    [[nodiscard]] inline Direction3f TransformDirection(
        const RigidTransform3f& transform, Direction3f direction) noexcept
    {
        return Rotate(transform.Rotation(), direction);
    }

    [[nodiscard]] inline Normal3f TransformNormal(
        const RigidTransform3f& transform, Normal3f normal) noexcept
    {
        return Rotate(transform.Rotation(), normal);
    }

    [[nodiscard]] inline RigidTransform3f Compose(
        const RigidTransform3f& first, const RigidTransform3f& second) noexcept
    {
        return RigidTransform3f(second.Rotation() * first.Rotation(),
                                Rotate(second.Rotation(), first.Translation()) +
                                    second.Translation());
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::RigidTransform3f) == 32);
static_assert(alignof(mv::math::RigidTransform3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::RigidTransform3f>);
static_assert(std::is_standard_layout_v<mv::math::RigidTransform3f>);
