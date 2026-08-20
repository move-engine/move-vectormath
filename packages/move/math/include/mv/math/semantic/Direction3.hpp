#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Angles.hpp>
#include <mv/math/detail/Normalization.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Rotation3;

    template <typename T>
        requires std::is_floating_point_v<T>
    class Normal3;

    template <typename T>
        requires std::is_floating_point_v<T>
    class Direction3;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Direction3<T> Rotate(const Rotation3<T>&,
                                       const Direction3<T>&) noexcept;

    template <typename T>
        requires std::is_floating_point_v<T>
    class Direction3
    {
    public:
        using Component = T;

        Direction3() noexcept : Value_(T(1), T(0), T(0))
        {
        }

        [[nodiscard]] static std::optional<Direction3> TryFrom(
            const Vec3<T>& value) noexcept
        {
            const auto normalized = detail::TryNormalizeVector(value);
            if (!normalized)
            {
                return std::nullopt;
            }
            return Direction3(*normalized, UncheckedTag{});
        }

        [[nodiscard]] static Direction3 FromOr(const Vec3<T>& value,
                                               Direction3 fallback) noexcept
        {
            const auto result = TryFrom(value);
            return result ? *result : fallback;
        }

        [[nodiscard]] static Direction3 AxisX() noexcept
        {
            return Direction3(Vec3<T>(T(1), T(0), T(0)), UncheckedTag{});
        }

        [[nodiscard]] static Direction3 AxisY() noexcept
        {
            return Direction3(Vec3<T>(T(0), T(1), T(0)), UncheckedTag{});
        }

        [[nodiscard]] static Direction3 AxisZ() noexcept
        {
            return Direction3(Vec3<T>(T(0), T(0), T(1)), UncheckedTag{});
        }

        [[nodiscard]] static Direction3 Forward() noexcept
        {
            return AxisZ();
        }

        [[nodiscard]] const Vec3<T>& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] Direction3 operator-() const noexcept
        {
            return Direction3(Value_ * T(-1), UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Direction3& left,
                                             const Direction3& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        explicit Direction3(const Vec3<T>& value, UncheckedTag) noexcept :
            Value_(value)
        {
        }

        Vec3<T> Value_;

        friend class Normal3<T>;
        friend class Rotation3<T>;
        friend Direction3<T> Rotate<T>(const Rotation3<T>&,
                                       const Direction3<T>&) noexcept;
    };

    using Direction3f = Direction3<float>;
    using Direction3d = Direction3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Direction3<T>> TryNormalize(
        const Vec3<T>& value) noexcept
    {
        return Direction3<T>::TryFrom(value);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> ProjectOntoDirection(
        const Vec3<T>& value, const Direction3<T>& direction) noexcept
    {
        // Unit-direction projection follows Ericson, Real-Time Collision
        // Detection (2005), section 5.1.2.
        return direction.Vector() * Dot(value, direction.Vector());
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Radians<T> AngleBetween(
        const Direction3<T>& from, const Direction3<T>& to) noexcept
    {
        // atan2(|cross|, dot) follows Godot 4.5-stable Vector3::angle_to
        // (MIT, commit 876b290332ec6f2e6d173d08162a02aa7e6ca46d).
        return Radians<T>(std::atan2(Length(Cross(from.Vector(), to.Vector())),
                                     Dot(from.Vector(), to.Vector())));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Radians<T> SignedAngle(
        const Direction3<T>& from,
        const Direction3<T>& to,
        const Direction3<T>& positiveAxis) noexcept
    {
        // Sign selection follows Godot 4.5-stable
        // Vector3::signed_angle_to (MIT, commit
        // 876b290332ec6f2e6d173d08162a02aa7e6ca46d).
        const Vec3<T> cross = Cross(from.Vector(), to.Vector());
        const T unsignedAngle =
            std::atan2(Length(cross), Dot(from.Vector(), to.Vector()));
        return Radians<T>(Dot(cross, positiveAxis.Vector()) < T(0)
                              ? -unsignedAngle
                              : unsignedAngle);
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Direction3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Direction3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Direction3f>);
static_assert(std::is_standard_layout_v<mv::math::Direction3f>);
static_assert(sizeof(mv::math::Direction3d) == sizeof(mv::math::Vec3d));
static_assert(alignof(mv::math::Direction3d) == alignof(mv::math::Vec3d));
static_assert(std::is_trivially_copyable_v<mv::math::Direction3d>);
static_assert(std::is_standard_layout_v<mv::math::Direction3d>);
