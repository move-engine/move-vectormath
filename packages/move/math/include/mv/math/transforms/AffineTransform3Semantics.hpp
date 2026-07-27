#pragma once

#include <algorithm>
#include <cmath>
#include <optional>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>
#include <mv/math/transforms/AffineTransform3.hpp>
#include <mv/math/transforms/RigidTransform3.hpp>

namespace mv::math
{
    [[nodiscard]] inline Point3f TransformPoint(
        const AffineTransform3f& transform, Point3f point) noexcept
    {
        return Point3f::FromVector(TransformPoint(transform, point.Vector()));
    }

    [[nodiscard]] inline std::optional<Direction3f> TryTransformDirection(
        const AffineTransform3f& transform, Direction3f direction) noexcept
    {
        return Direction3f::TryFrom(
            TransformVector(transform, direction.Vector()));
    }

    [[nodiscard]] inline std::optional<Normal3f> TryTransformNormal(
        const AffineTransform3f& transform, Normal3f normal) noexcept
    {
        const Vec3f axisX = transform.AxisX();
        const Vec3f axisY = transform.AxisY();
        const Vec3f axisZ = transform.AxisZ();

        const double ax = static_cast<double>(axisX.X());
        const double ay = static_cast<double>(axisX.Y());
        const double az = static_cast<double>(axisX.Z());
        const double bx = static_cast<double>(axisY.X());
        const double by = static_cast<double>(axisY.Y());
        const double bz = static_cast<double>(axisY.Z());
        const double cx = static_cast<double>(axisZ.X());
        const double cy = static_cast<double>(axisZ.Y());
        const double cz = static_cast<double>(axisZ.Z());

        const double cofactorXx = by * cz - bz * cy;
        const double cofactorXy = bz * cx - bx * cz;
        const double cofactorXz = bx * cy - by * cx;
        const double cofactorYx = cy * az - cz * ay;
        const double cofactorYy = cz * ax - cx * az;
        const double cofactorYz = cx * ay - cy * ax;
        const double cofactorZx = ay * bz - az * by;
        const double cofactorZy = az * bx - ax * bz;
        const double cofactorZz = ax * by - ay * bx;
        const double determinant =
            ax * cofactorXx + ay * cofactorXy + az * cofactorXz;

        if (determinant == 0.0 || !std::isfinite(determinant))
        {
            return std::nullopt;
        }

        const Vec3f& value = normal.Vector();
        const double nx = static_cast<double>(value.X());
        const double ny = static_cast<double>(value.Y());
        const double nz = static_cast<double>(value.Z());
        const double orientation = determinant < 0.0 ? -1.0 : 1.0;
        double transformedX =
            orientation * (cofactorXx * nx + cofactorYx * ny + cofactorZx * nz);
        double transformedY =
            orientation * (cofactorXy * nx + cofactorYy * ny + cofactorZy * nz);
        double transformedZ =
            orientation * (cofactorXz * nx + cofactorYz * ny + cofactorZz * nz);

        const double maximum =
            std::max({std::abs(transformedX), std::abs(transformedY),
                      std::abs(transformedZ)});
        if (!(maximum > 0.0) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }

        transformedX /= maximum;
        transformedY /= maximum;
        transformedZ /= maximum;
        const double length = std::sqrt(transformedX * transformedX +
                                        transformedY * transformedY +
                                        transformedZ * transformedZ);
        if (!(length > 0.0) || !std::isfinite(length))
        {
            return std::nullopt;
        }

        return Normal3f::TryFrom(
            Vec3f(static_cast<float>(transformedX / length),
                  static_cast<float>(transformedY / length),
                  static_cast<float>(transformedZ / length)));
    }

    [[nodiscard]] inline AffineTransform3f ToAffine(
        const RigidTransform3f& transform) noexcept
    {
        return AffineTransform3f(
            Rotate(transform.Rotation(), Vec3f(1.0F, 0.0F, 0.0F)),
            Rotate(transform.Rotation(), Vec3f(0.0F, 1.0F, 0.0F)),
            Rotate(transform.Rotation(), Vec3f(0.0F, 0.0F, 1.0F)),
            transform.Translation());
    }
}  // namespace mv::math
