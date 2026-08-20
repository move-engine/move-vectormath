#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Angles.hpp>
#include <mv/math/Quat.hpp>
#include <mv/math/semantic/Direction3.hpp>

namespace mv::math
{
    // Quaternion equations follow Szeliski, MSR-TR-2004-92, eqs. 22/26/28;
    // conventions and results are cross-checked against RTM 2.3.1 (MIT).
    template <typename T>
        requires std::is_floating_point_v<T>
    class Rotation3
    {
    public:
        using Component = T;

        constexpr Rotation3() noexcept = default;

        [[nodiscard]] static constexpr Rotation3 Identity() noexcept
        {
            return Rotation3(Quat<T>(), UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Rotation3> TryFrom(
            const Quat<T>& value) noexcept
        {
            const T maximum =
                std::max({std::abs(value.X()), std::abs(value.Y()),
                          std::abs(value.Z()), std::abs(value.W())});
            if (!(maximum > T(0)) || !std::isfinite(maximum))
            {
                return std::nullopt;
            }

            const T x = value.X() / maximum;
            const T y = value.Y() / maximum;
            const T z = value.Z() / maximum;
            const T w = value.W() / maximum;
            const T lengthSquared = x * x + y * y + z * z + w * w;
            if (!(lengthSquared > T(0)) || !std::isfinite(lengthSquared))
            {
                return std::nullopt;
            }

            const T reciprocalLength = T(1) / std::sqrt(lengthSquared);
            return Rotation3(
                Quat<T>(x * reciprocalLength, y * reciprocalLength,
                        z * reciprocalLength, w * reciprocalLength),
                UncheckedTag{});
        }

        [[nodiscard]] static Rotation3 FromAxisAngle(const Direction3<T>& axis,
                                                     Radians<T> angle) noexcept
        {
            const T halfAngle = angle.Value() * T(0.5);
            const T sine = std::sin(halfAngle);
            const T cosine = std::cos(halfAngle);
            const Vec3<T>& vector = axis.Vector();
            return Rotation3(Quat<T>(vector.X() * sine, vector.Y() * sine,
                                     vector.Z() * sine, cosine),
                             UncheckedTag{});
        }

        [[nodiscard]] constexpr const Quat<T>& Quaternion() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] constexpr Rotation3 Inverse() const noexcept
        {
            return Rotation3(
                Quat<T>(-Value_.X(), -Value_.Y(), -Value_.Z(), Value_.W()),
                UncheckedTag{});
        }

        [[nodiscard]] Rotation3 operator*(const Rotation3& right) const noexcept
        {
            const Quat<T>& leftValue = Value_;
            const Quat<T>& rightValue = right.Value_;
            const Quat<T> composed(leftValue.W() * rightValue.X() +
                                       leftValue.X() * rightValue.W() +
                                       leftValue.Y() * rightValue.Z() -
                                       leftValue.Z() * rightValue.Y(),
                                   leftValue.W() * rightValue.Y() -
                                       leftValue.X() * rightValue.Z() +
                                       leftValue.Y() * rightValue.W() +
                                       leftValue.Z() * rightValue.X(),
                                   leftValue.W() * rightValue.Z() +
                                       leftValue.X() * rightValue.Y() -
                                       leftValue.Y() * rightValue.X() +
                                       leftValue.Z() * rightValue.W(),
                                   leftValue.W() * rightValue.W() -
                                       leftValue.X() * rightValue.X() -
                                       leftValue.Y() * rightValue.Y() -
                                       leftValue.Z() * rightValue.Z());

            return *TryFrom(composed);
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Rotation3& left, const Rotation3& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        constexpr explicit Rotation3(const Quat<T>& value,
                                     UncheckedTag) noexcept :
            Value_(value)
        {
        }

        Quat<T> Value_{};
    };

    using Rotation3f = Rotation3<float>;
    using Rotation3d = Rotation3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> Rotate(const Rotation3<T>& rotation,
                                        const Vec3<T>& vector) noexcept
    {
        const Quat<T>& quaternion = rotation.Quaternion();
        const Vec3<T> imaginary(quaternion.X(), quaternion.Y(), quaternion.Z());
        const Vec3<T> twiceCross = Cross(imaginary, vector) * T(2);
        return vector + twiceCross * quaternion.W() +
               Cross(imaginary, twiceCross);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Direction3<T> Rotate(
        const Rotation3<T>& rotation, const Direction3<T>& direction) noexcept
    {
        return Direction3<T>(Rotate(rotation, direction.Vector()),
                             typename Direction3<T>::UncheckedTag{});
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Rotation3f) == sizeof(mv::math::Quatf));
static_assert(alignof(mv::math::Rotation3f) == alignof(mv::math::Quatf));
static_assert(std::is_trivially_copyable_v<mv::math::Rotation3f>);
static_assert(std::is_standard_layout_v<mv::math::Rotation3f>);
static_assert(sizeof(mv::math::Rotation3d) == sizeof(mv::math::Quatd));
static_assert(alignof(mv::math::Rotation3d) == alignof(mv::math::Quatd));
static_assert(std::is_trivially_copyable_v<mv::math::Rotation3d>);
static_assert(std::is_standard_layout_v<mv::math::Rotation3d>);
