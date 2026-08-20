#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <optional>
#include <type_traits>

#include <mv/math/Tolerance.hpp>
#include <mv/math/transforms/AffineTransform3.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class TrsTransform3
    {
    public:
        using Component = T;

        TrsTransform3() noexcept = default;

        TrsTransform3(const Vec3<T>& translation,
                      const Rotation3<T>& rotation,
                      const Vec3<T>& scale) noexcept :
            Translation_(translation), Rotation_(rotation), Scale_(scale)
        {
        }

        [[nodiscard]] static TrsTransform3 Identity() noexcept
        {
            return TrsTransform3();
        }

        [[nodiscard]] const Vec3<T>& Translation() const noexcept
        {
            return Translation_;
        }

        [[nodiscard]] const Rotation3<T>& Rotation() const noexcept
        {
            return Rotation_;
        }

        [[nodiscard]] const Vec3<T>& Scale() const noexcept
        {
            return Scale_;
        }

        [[nodiscard]] AffineTransform3<T> ToAffine() const noexcept
        {
            // A TRS value applies scale, then rotation, then translation.
            // This is the row-vector transpose of the standard Graphics Gems
            // affine construction and is cross-checked against GLM's MIT
            // scale/rotation/translation composition.
            const Mat3<T> rotation = Mat3<T>::FromRotation(Rotation_);
            return AffineTransform3<T>(
                rotation.Row(0U) * Scale_.X(), rotation.Row(1U) * Scale_.Y(),
                rotation.Row(2U) * Scale_.Z(), Translation_);
        }

        [[nodiscard]] friend bool operator==(
            const TrsTransform3& left, const TrsTransform3& right) noexcept
        {
            return left.Translation_ == right.Translation_ &&
                   left.Rotation_ == right.Rotation_ &&
                   left.Scale_ == right.Scale_;
        }

    private:
        Vec3<T> Translation_{};
        Rotation3<T> Rotation_{};
        Vec3<T> Scale_{T(1), T(1), T(1)};
    };

    using TrsTransform3f = TrsTransform3<float>;
    using TrsTransform3d = TrsTransform3<double>;

    enum class TrsDecompositionStatus : std::uint8_t
    {
        Success,
        Singular,
        Sheared,
        NumericalFailure
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct TrsDecomposition3
    {
        TrsDecompositionStatus Status;
        std::optional<TrsTransform3<T>> Transform;
        Vec3<T> Shear;

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return Status == TrsDecompositionStatus::Success;
        }
    };

    using TrsDecomposition3f = TrsDecomposition3<float>;
    using TrsDecomposition3d = TrsDecomposition3<double>;

    template <typename T>
    [[nodiscard]] inline TrsDecomposition3<T> DecomposeTrs(
        const AffineTransform3<T>& transform,
        T orthogonalityTolerance = static_cast<T>(1.0e-5)) noexcept
    {
        const Vec3<T>& axisX = transform.AxisX();
        const Vec3<T>& axisY = transform.AxisY();
        const Vec3<T>& axisZ = transform.AxisZ();
        T scaleX = Length(axisX);
        T scaleY = Length(axisY);
        T scaleZ = Length(axisZ);
        if (!(scaleX > T(0)) || !(scaleY > T(0)) || !(scaleZ > T(0)))
        {
            return {TrsDecompositionStatus::Singular, std::nullopt,
                    Vec3<T>::Zero()};
        }
        if (!std::isfinite(scaleX) || !std::isfinite(scaleY) ||
            !std::isfinite(scaleZ))
        {
            return {TrsDecompositionStatus::NumericalFailure, std::nullopt,
                    Vec3<T>::Zero()};
        }

        Vec3<T> normalizedX = axisX / scaleX;
        const Vec3<T> normalizedY = axisY / scaleY;
        const Vec3<T> normalizedZ = axisZ / scaleZ;
        const Vec3<T> shear(Dot(normalizedX, normalizedY),
                            Dot(normalizedX, normalizedZ),
                            Dot(normalizedY, normalizedZ));
        const T tolerance = std::abs(orthogonalityTolerance);
        if (std::abs(shear.X()) > tolerance ||
            std::abs(shear.Y()) > tolerance || std::abs(shear.Z()) > tolerance)
        {
            return {TrsDecompositionStatus::Sheared, std::nullopt, shear};
        }

        // A unit quaternion represents only proper rotations. Preserve an
        // affine reflection by assigning its sign to the X scale before
        // extracting the proper orthonormal basis. This is the deterministic
        // Move policy; equivalent decompositions with another negative axis
        // are intentionally not produced.
        if (Dot(Cross(normalizedX, normalizedY), normalizedZ) < T(0))
        {
            normalizedX = -normalizedX;
            scaleX = -scaleX;
        }

        const auto directionX = Direction3<T>::TryFrom(normalizedX);
        const auto directionY = Direction3<T>::TryFrom(normalizedY);
        const auto directionZ = Direction3<T>::TryFrom(normalizedZ);
        if (!directionX || !directionY || !directionZ)
        {
            return {TrsDecompositionStatus::NumericalFailure, std::nullopt,
                    shear};
        }
        const auto rotation = Rotation3<T>::TryFromOrthonormalAxes(
            *directionX, *directionY, *directionZ, tolerance);
        if (!rotation)
        {
            return {TrsDecompositionStatus::NumericalFailure, std::nullopt,
                    shear};
        }

        return {TrsDecompositionStatus::Success,
                TrsTransform3<T>(transform.Translation(), *rotation,
                                 Vec3<T>(scaleX, scaleY, scaleZ)),
                shear};
    }

    template <typename T>
    [[nodiscard]] inline std::optional<TrsTransform3<T>> TryToTrs(
        const AffineTransform3<T>& transform,
        T orthogonalityTolerance = static_cast<T>(1.0e-5)) noexcept
    {
        return DecomposeTrs(transform, orthogonalityTolerance).Transform;
    }
}  // namespace mv::math

static_assert(std::is_trivially_copyable_v<mv::math::TrsTransform3f>);
static_assert(std::is_standard_layout_v<mv::math::TrsTransform3f>);
static_assert(std::is_trivially_copyable_v<mv::math::TrsTransform3d>);
static_assert(std::is_standard_layout_v<mv::math::TrsTransform3d>);
