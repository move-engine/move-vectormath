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
    enum class EulerOrder
    {
        XYZ,
        XZY,
        YXZ,
        YZX,
        ZXY,
        ZYX
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct EulerAngles3
    {
        Radians<T> X;
        Radians<T> Y;
        Radians<T> Z;
        EulerOrder Order;
    };

    using EulerAngles3f = EulerAngles3<float>;
    using EulerAngles3d = EulerAngles3<double>;

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
            const auto normalized = TryNormalize(value);
            if (!normalized)
            {
                return std::nullopt;
            }
            return Rotation3(*normalized, UncheckedTag{});
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

        // Euler order names the application order to a local vector. This is
        // intentionally explicit instead of inheriting an engine-specific
        // implicit convention. Composition is cross-checked against GLM's MIT
        // quaternion multiplication convention.
        [[nodiscard]] static Rotation3 FromEuler(
            const EulerAngles3<T>& angles) noexcept
        {
            const Rotation3 x = FromAxisAngle(Direction3<T>::AxisX(), angles.X);
            const Rotation3 y = FromAxisAngle(Direction3<T>::AxisY(), angles.Y);
            const Rotation3 z = FromAxisAngle(Direction3<T>::AxisZ(), angles.Z);
            const auto apply = [](const Rotation3& first,
                                  const Rotation3& second,
                                  const Rotation3& third) noexcept
            {
                return third * second * first;
            };
            switch (angles.Order)
            {
                case EulerOrder::XYZ:
                    return apply(x, y, z);
                case EulerOrder::XZY:
                    return apply(x, z, y);
                case EulerOrder::YXZ:
                    return apply(y, x, z);
                case EulerOrder::YZX:
                    return apply(y, z, x);
                case EulerOrder::ZXY:
                    return apply(z, x, y);
                case EulerOrder::ZYX:
                    return apply(z, y, x);
            }
            return Identity();
        }

        [[nodiscard]] static std::optional<Rotation3> TryFromOrthonormalAxes(
            const Direction3<T>& axisX,
            const Direction3<T>& axisY,
            const Direction3<T>& axisZ,
            T orthogonalityTolerance = static_cast<T>(1.0e-5)) noexcept
        {
            const Vec3<T>& x = axisX.Vector();
            const Vec3<T>& y = axisY.Vector();
            const Vec3<T>& z = axisZ.Vector();
            const T tolerance = std::abs(orthogonalityTolerance);
            if (std::abs(Dot(x, y)) > tolerance ||
                std::abs(Dot(x, z)) > tolerance ||
                std::abs(Dot(y, z)) > tolerance || Dot(Cross(x, y), z) <= T(0))
            {
                return std::nullopt;
            }

            // Shoemake's matrix-to-quaternion branch selection, transposed
            // for Move's row-vector matrix convention; cross-checked against
            // DirectXMath XMQuaternionRotationMatrix (MIT).
            const T m00 = x.X();
            const T m01 = x.Y();
            const T m02 = x.Z();
            const T m10 = y.X();
            const T m11 = y.Y();
            const T m12 = y.Z();
            const T m20 = z.X();
            const T m21 = z.Y();
            const T m22 = z.Z();
            Quat<T> quaternion;
            const T trace = m00 + m11 + m22;
            if (trace > T(0))
            {
                const T scale = std::sqrt(trace + T(1)) * T(2);
                quaternion = Quat<T>((m12 - m21) / scale, (m20 - m02) / scale,
                                     (m01 - m10) / scale, scale * T(0.25));
            }
            else if (m00 > m11 && m00 > m22)
            {
                const T scale = std::sqrt(T(1) + m00 - m11 - m22) * T(2);
                quaternion = Quat<T>(scale * T(0.25), (m01 + m10) / scale,
                                     (m02 + m20) / scale, (m12 - m21) / scale);
            }
            else if (m11 > m22)
            {
                const T scale = std::sqrt(T(1) + m11 - m00 - m22) * T(2);
                quaternion = Quat<T>((m01 + m10) / scale, scale * T(0.25),
                                     (m12 + m21) / scale, (m20 - m02) / scale);
            }
            else
            {
                const T scale = std::sqrt(T(1) + m22 - m00 - m11) * T(2);
                quaternion = Quat<T>((m02 + m20) / scale, (m12 + m21) / scale,
                                     scale * T(0.25), (m01 - m10) / scale);
            }
            return TryFrom(quaternion);
        }

        [[nodiscard]] static std::optional<Rotation3> TryLookTowards(
            const Direction3<T>& forward,
            const Direction3<T>& approximateUp) noexcept
        {
            // Standard Gram-Schmidt camera/object basis, cross-checked against
            // DirectXMath XMMatrixLookToLH (MIT). +Z is Move's local forward.
            const auto right = Direction3<T>::TryFrom(
                Cross(approximateUp.Vector(), forward.Vector()));
            if (!right)
            {
                return std::nullopt;
            }
            const auto up = Direction3<T>::TryFrom(
                Cross(forward.Vector(), right->Vector()));
            return up ? TryFromOrthonormalAxes(*right, *up, forward)
                      : std::nullopt;
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
            return *TryFrom(Value_ * right.Value_);
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
    [[nodiscard]] inline bool IsNearlyEquivalent(
        const Rotation3<T>& left,
        const Rotation3<T>& right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        const Quat<T>& leftValue = left.Quaternion();
        const Quat<T>& rightValue = right.Quaternion();
        return IsNearlyEqual(leftValue, rightValue, tolerance) ||
               IsNearlyEqual(leftValue, -rightValue, tolerance);
    }

    template <typename T>
    [[nodiscard]] inline Rotation3<T> NlerpUnclamped(const Rotation3<T>& start,
                                                     const Rotation3<T>& end,
                                                     T amount) noexcept
    {
        const Quat<T>& startValue = start.Quaternion();
        Quat<T> endValue = end.Quaternion();
        if (Dot(startValue, endValue) < T(0))
        {
            endValue = -endValue;
        }
        return *Rotation3<T>::TryFrom(startValue * (T(1) - amount) +
                                      endValue * amount);
    }

    template <typename T>
    [[nodiscard]] inline Rotation3<T> Nlerp(const Rotation3<T>& start,
                                            const Rotation3<T>& end,
                                            T amount) noexcept
    {
        return NlerpUnclamped(start, end, std::clamp(amount, T(0), T(1)));
    }

    template <typename T>
    [[nodiscard]] inline Rotation3<T> SlerpUnclamped(const Rotation3<T>& start,
                                                     const Rotation3<T>& end,
                                                     T amount) noexcept
    {
        const Quat<T>& startValue = start.Quaternion();
        Quat<T> endValue = end.Quaternion();
        T cosine = Dot(startValue, endValue);
        if (cosine < T(0))
        {
            cosine = -cosine;
            endValue = -endValue;
        }
        cosine = std::clamp(cosine, T(-1), T(1));
        if (cosine > static_cast<T>(0.9995))
        {
            return NlerpUnclamped(start, end, amount);
        }

        // Shoemake, "Animating Rotation with Quaternion Curves" (SIGGRAPH
        // 1985), shortest-arc spherical interpolation.
        const T angle = std::acos(cosine);
        const T inverseSine = T(1) / std::sin(angle);
        const T startWeight = std::sin((T(1) - amount) * angle) * inverseSine;
        const T endWeight = std::sin(amount * angle) * inverseSine;
        return *Rotation3<T>::TryFrom(startValue * startWeight +
                                      endValue * endWeight);
    }

    template <typename T>
    [[nodiscard]] inline Rotation3<T> Slerp(const Rotation3<T>& start,
                                            const Rotation3<T>& end,
                                            T amount) noexcept
    {
        return SlerpUnclamped(start, end, std::clamp(amount, T(0), T(1)));
    }

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
