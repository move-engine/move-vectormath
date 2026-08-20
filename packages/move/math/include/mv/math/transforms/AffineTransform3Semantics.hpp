#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>
#include <mv/math/transforms/AffineTransform3.hpp>
#include <mv/math/transforms/RigidTransform3.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Point3<T> TransformPoint(
        const AffineTransform3<T>& transform, const Point3<T>& point) noexcept
    {
        return Point3<T>::FromVector(TransformPoint(transform, point.Vector()));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Direction3<T>> TryTransformDirection(
        const AffineTransform3<T>& transform,
        const Direction3<T>& direction) noexcept
    {
        return Direction3<T>::TryFrom(
            TransformVector(transform, direction.Vector()));
    }

    // Cofactor/inverse-transpose derivation: Eric Lengyel, "Transforming
    // Normals" (2024). Reflection semantics remain an explicit Move decision.
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Normal3<T>> TryTransformNormal(
        const AffineTransform3<T>& transform, const Normal3<T>& normal) noexcept
    {
        using Calculation =
            std::conditional_t<(sizeof(T) < sizeof(double)), double, T>;

        const Vec3<T>& axisX = transform.AxisX();
        const Vec3<T>& axisY = transform.AxisY();
        const Vec3<T>& axisZ = transform.AxisZ();

        const Calculation ax = static_cast<Calculation>(axisX.X());
        const Calculation ay = static_cast<Calculation>(axisX.Y());
        const Calculation az = static_cast<Calculation>(axisX.Z());
        const Calculation bx = static_cast<Calculation>(axisY.X());
        const Calculation by = static_cast<Calculation>(axisY.Y());
        const Calculation bz = static_cast<Calculation>(axisY.Z());
        const Calculation cx = static_cast<Calculation>(axisZ.X());
        const Calculation cy = static_cast<Calculation>(axisZ.Y());
        const Calculation cz = static_cast<Calculation>(axisZ.Z());

        const Calculation cofactorXx = by * cz - bz * cy;
        const Calculation cofactorXy = bz * cx - bx * cz;
        const Calculation cofactorXz = bx * cy - by * cx;
        const Calculation cofactorYx = cy * az - cz * ay;
        const Calculation cofactorYy = cz * ax - cx * az;
        const Calculation cofactorYz = cx * ay - cy * ax;
        const Calculation cofactorZx = ay * bz - az * by;
        const Calculation cofactorZy = az * bx - ax * bz;
        const Calculation cofactorZz = ax * by - ay * bx;
        const Calculation determinant =
            ax * cofactorXx + ay * cofactorXy + az * cofactorXz;

        if (determinant == Calculation(0) || !std::isfinite(determinant))
        {
            return std::nullopt;
        }

        const Vec3<T>& value = normal.Vector();
        const Calculation nx = static_cast<Calculation>(value.X());
        const Calculation ny = static_cast<Calculation>(value.Y());
        const Calculation nz = static_cast<Calculation>(value.Z());
        const Calculation orientation =
            determinant < Calculation(0) ? Calculation(-1) : Calculation(1);
        Calculation transformedX =
            orientation * (cofactorXx * nx + cofactorYx * ny + cofactorZx * nz);
        Calculation transformedY =
            orientation * (cofactorXy * nx + cofactorYy * ny + cofactorZy * nz);
        Calculation transformedZ =
            orientation * (cofactorXz * nx + cofactorYz * ny + cofactorZz * nz);

        const Calculation maximum =
            std::max({std::abs(transformedX), std::abs(transformedY),
                      std::abs(transformedZ)});
        if (!(maximum > Calculation(0)) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }

        transformedX /= maximum;
        transformedY /= maximum;
        transformedZ /= maximum;
        const Calculation length = std::sqrt(transformedX * transformedX +
                                             transformedY * transformedY +
                                             transformedZ * transformedZ);
        if (!(length > Calculation(0)) || !std::isfinite(length))
        {
            return std::nullopt;
        }

        return Normal3<T>::TryFrom(
            Vec3<T>(static_cast<T>(transformedX / length),
                    static_cast<T>(transformedY / length),
                    static_cast<T>(transformedZ / length)));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline AffineTransform3<T> ToAffine(
        const RigidTransform3<T>& transform) noexcept
    {
        return AffineTransform3<T>(
            Rotate(transform.Rotation(), Vec3<T>(T(1), T(0), T(0))),
            Rotate(transform.Rotation(), Vec3<T>(T(0), T(1), T(0))),
            Rotate(transform.Rotation(), Vec3<T>(T(0), T(0), T(1))),
            transform.Translation());
    }
}  // namespace mv::math
