#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/Mat4.hpp>
#include <mv/math/semantic/Direction3.hpp>
#include <mv/math/semantic/Point3.hpp>
#include <mv/math/transforms/Projection.hpp>

namespace mv::math
{
    // Look-at basis construction follows the standard Gram-Schmidt form,
    // cross-checked against DirectXMath XMMatrixLookAtLH/RH (MIT) and GLM
    // matrix_transform.inl lookAtLH/RH (MIT). Failure is explicit when eye and
    // target coincide or the supplied up direction is parallel to the view.
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryLookAt(
        const Point3<T>& eye,
        const Point3<T>& target,
        const Direction3<T>& up,
        Handedness handedness) noexcept
    {
        auto forward = Direction3<T>::TryFrom(target - eye);
        if (!forward)
        {
            return std::nullopt;
        }

        const Direction3<T> viewZ =
            handedness == Handedness::Left ? *forward : -*forward;
        const auto viewX =
            Direction3<T>::TryFrom(Cross(up.Vector(), viewZ.Vector()));
        if (!viewX)
        {
            return std::nullopt;
        }
        const Vec3<T> viewY = Cross(viewZ.Vector(), viewX->Vector());
        const Vec3<T>& x = viewX->Vector();
        const Vec3<T>& z = viewZ.Vector();
        const Vec3<T>& position = eye.Vector();

        const Mat4<T> result(x.X(), viewY.X(), z.X(), T(0), x.Y(), viewY.Y(),
                             z.Y(), T(0), x.Z(), viewY.Z(), z.Z(), T(0),
                             -Dot(x, position), -Dot(viewY, position),
                             -Dot(z, position), T(1));
        return result.IsFinite() ? std::optional<Mat4<T>>(result)
                                 : std::nullopt;
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryLookAtLH(
        const Point3<T>& eye,
        const Point3<T>& target,
        const Direction3<T>& up) noexcept
    {
        return TryLookAt(eye, target, up, Handedness::Left);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryLookAtRH(
        const Point3<T>& eye,
        const Point3<T>& target,
        const Direction3<T>& up) noexcept
    {
        return TryLookAt(eye, target, up, Handedness::Right);
    }
}  // namespace mv::math
