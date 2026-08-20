#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/Mat4.hpp>
#include <mv/math/Tolerance.hpp>
#include <mv/math/transforms/AffineTransform3Semantics.hpp>
#include <mv/math/transforms/TrsTransform3.hpp>

namespace mv::math
{
    template <typename T>
    [[nodiscard]] inline Mat4<T> ToMat4(
        const AffineTransform3<T>& transform) noexcept
    {
        const Vec3<T>& x = transform.AxisX();
        const Vec3<T>& y = transform.AxisY();
        const Vec3<T>& z = transform.AxisZ();
        const Vec3<T>& translation = transform.Translation();
        return Mat4<T>(x.X(), x.Y(), x.Z(), T(0), y.X(), y.Y(), y.Z(), T(0),
                       z.X(), z.Y(), z.Z(), T(0), translation.X(),
                       translation.Y(), translation.Z(), T(1));
    }

    template <typename T>
    [[nodiscard]] inline Mat4<T> ToMat4(
        const RigidTransform3<T>& transform) noexcept
    {
        return ToMat4(ToAffine(transform));
    }

    template <typename T>
    [[nodiscard]] inline Mat4<T> ToMat4(
        const TrsTransform3<T>& transform) noexcept
    {
        return ToMat4(transform.ToAffine());
    }

    template <typename T>
    [[nodiscard]] inline std::optional<AffineTransform3<T>> TryToAffine(
        const Mat4<T>& matrix,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        if (!matrix.IsFinite() ||
            !IsNearlyZero(matrix.Element(0U, 3U), tolerance.Absolute) ||
            !IsNearlyZero(matrix.Element(1U, 3U), tolerance.Absolute) ||
            !IsNearlyZero(matrix.Element(2U, 3U), tolerance.Absolute) ||
            !IsNearlyEqual(matrix.Element(3U, 3U), T(1), tolerance))
        {
            return std::nullopt;
        }
        return AffineTransform3<T>(
            Vec3<T>(matrix.Element(0U, 0U), matrix.Element(0U, 1U),
                    matrix.Element(0U, 2U)),
            Vec3<T>(matrix.Element(1U, 0U), matrix.Element(1U, 1U),
                    matrix.Element(1U, 2U)),
            Vec3<T>(matrix.Element(2U, 0U), matrix.Element(2U, 1U),
                    matrix.Element(2U, 2U)),
            Vec3<T>(matrix.Element(3U, 0U), matrix.Element(3U, 1U),
                    matrix.Element(3U, 2U)));
    }

    template <typename T>
    [[nodiscard]] inline std::optional<RigidTransform3<T>> TryToRigid(
        const AffineTransform3<T>& transform,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        const auto decomposition =
            DecomposeTrs(transform, tolerance.Absolute * T(8));
        if (!decomposition.Transform)
        {
            return std::nullopt;
        }
        const Vec3<T>& scale = decomposition.Transform->Scale();
        if (!IsNearlyEqual(scale.X(), T(1), tolerance) ||
            !IsNearlyEqual(scale.Y(), T(1), tolerance) ||
            !IsNearlyEqual(scale.Z(), T(1), tolerance))
        {
            return std::nullopt;
        }
        return RigidTransform3<T>(decomposition.Transform->Rotation(),
                                  transform.Translation());
    }

    template <typename T>
    [[nodiscard]] inline std::optional<RigidTransform3<T>> TryToRigid(
        const Mat4<T>& matrix,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        const auto affine = TryToAffine(matrix, tolerance);
        return affine ? TryToRigid(*affine, tolerance) : std::nullopt;
    }
}  // namespace mv::math
