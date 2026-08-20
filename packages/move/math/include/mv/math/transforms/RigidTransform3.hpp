#pragma once

#include <type_traits>

#include <mv/math/Rotation3.hpp>
#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Transform/composition equations cross-checked against RTM 2.3.1 qvf.h
    // (MIT); Compose uses Move's explicit application-order contract.
    template <typename T>
        requires std::is_floating_point_v<T>
    class RigidTransform3
    {
    public:
        using Component = T;

        constexpr RigidTransform3() noexcept = default;

        RigidTransform3(const Rotation3<T>& rotation,
                        const Vec3<T>& translation) noexcept :
            Rotation_(rotation), Translation_(translation)
        {
        }

        [[nodiscard]] static RigidTransform3 Identity() noexcept
        {
            return RigidTransform3();
        }

        [[nodiscard]] constexpr const Rotation3<T>& Rotation() const noexcept
        {
            return Rotation_;
        }

        [[nodiscard]] const Vec3<T>& Translation() const noexcept
        {
            return Translation_;
        }

        [[nodiscard]] RigidTransform3 Inverse() const noexcept
        {
            const Rotation3<T> inverseRotation = Rotation_.Inverse();
            return RigidTransform3(
                inverseRotation, Rotate(inverseRotation, Translation_ * T(-1)));
        }

        [[nodiscard]] friend bool operator==(
            const RigidTransform3& left, const RigidTransform3& right) noexcept
        {
            return left.Rotation_ == right.Rotation_ &&
                   left.Translation_ == right.Translation_;
        }

    private:
        Rotation3<T> Rotation_{};
        Vec3<T> Translation_{};
    };

    using RigidTransform3f = RigidTransform3<float>;
    using RigidTransform3d = RigidTransform3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> TransformVector(
        const RigidTransform3<T>& transform, const Vec3<T>& vector) noexcept
    {
        return Rotate(transform.Rotation(), vector);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Point3<T> TransformPoint(
        const RigidTransform3<T>& transform, const Point3<T>& point) noexcept
    {
        return Point3<T>::FromVector(
            Rotate(transform.Rotation(), point.Vector()) +
            transform.Translation());
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Direction3<T> TransformDirection(
        const RigidTransform3<T>& transform,
        const Direction3<T>& direction) noexcept
    {
        return Rotate(transform.Rotation(), direction);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Normal3<T> TransformNormal(
        const RigidTransform3<T>& transform, const Normal3<T>& normal) noexcept
    {
        return Rotate(transform.Rotation(), normal);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline RigidTransform3<T> Compose(
        const RigidTransform3<T>& first,
        const RigidTransform3<T>& second) noexcept
    {
        return RigidTransform3<T>(
            second.Rotation() * first.Rotation(),
            Rotate(second.Rotation(), first.Translation()) +
                second.Translation());
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::RigidTransform3f) == 32);
static_assert(alignof(mv::math::RigidTransform3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::RigidTransform3f>);
static_assert(std::is_standard_layout_v<mv::math::RigidTransform3f>);
static_assert(sizeof(mv::math::RigidTransform3d) == 64);
static_assert(alignof(mv::math::RigidTransform3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::RigidTransform3d>);
static_assert(std::is_standard_layout_v<mv::math::RigidTransform3d>);
