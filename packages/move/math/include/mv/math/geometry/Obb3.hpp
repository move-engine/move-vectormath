#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <optional>
#include <span>
#include <type_traits>

#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/transforms/AffineTransform3Semantics.hpp>

namespace mv::math
{
    // Center/extents/orientation representation follows DirectXCollision's
    // BoundingOrientedBox (MIT); Move replaces its unchecked quaternion with
    // invariant-bearing Rotation3 and validates finite nonnegative extents.
    template <typename T>
        requires std::is_floating_point_v<T>
    class Obb3
    {
    public:
        using Component = T;

        Obb3() noexcept = default;

        [[nodiscard]] static std::optional<Obb3> TryFromCenterHalfExtents(
            const Point3<T>& center,
            const Vec3<T>& halfExtents,
            const Rotation3<T>& orientation = Rotation3<T>::Identity()) noexcept
        {
            if (!std::isfinite(center.X()) || !std::isfinite(center.Y()) ||
                !std::isfinite(center.Z()) || !std::isfinite(halfExtents.X()) ||
                !std::isfinite(halfExtents.Y()) ||
                !std::isfinite(halfExtents.Z()) || halfExtents.X() < T(0) ||
                halfExtents.Y() < T(0) || halfExtents.Z() < T(0))
            {
                return std::nullopt;
            }
            return Obb3(center, halfExtents, orientation, UncheckedTag{});
        }

        [[nodiscard]] const Point3<T>& Center() const noexcept
        {
            return Center_;
        }

        [[nodiscard]] const Vec3<T>& HalfExtents() const noexcept
        {
            return HalfExtents_;
        }

        [[nodiscard]] const Rotation3<T>& Orientation() const noexcept
        {
            return Orientation_;
        }

        [[nodiscard]] Direction3<T> AxisX() const noexcept
        {
            return Rotate(Orientation_, Direction3<T>::AxisX());
        }

        [[nodiscard]] Direction3<T> AxisY() const noexcept
        {
            return Rotate(Orientation_, Direction3<T>::AxisY());
        }

        [[nodiscard]] Direction3<T> AxisZ() const noexcept
        {
            return Rotate(Orientation_, Direction3<T>::AxisZ());
        }

        [[nodiscard]] Point3<T> ClosestPoint(
            const Point3<T>& point) const noexcept
        {
            const Vec3<T> displacement = point - Center_;
            const Vec3<T> x = AxisX().Vector();
            const Vec3<T> y = AxisY().Vector();
            const Vec3<T> z = AxisZ().Vector();
            return Center_ +
                   x * std::clamp(Dot(displacement, x), -HalfExtents_.X(),
                                  HalfExtents_.X()) +
                   y * std::clamp(Dot(displacement, y), -HalfExtents_.Y(),
                                  HalfExtents_.Y()) +
                   z * std::clamp(Dot(displacement, z), -HalfExtents_.Z(),
                                  HalfExtents_.Z());
        }

        [[nodiscard]] bool Contains(const Point3<T>& point,
                                    T tolerance = T(0)) const noexcept
        {
            const Vec3<T> displacement = point - Center_;
            const T margin = std::abs(tolerance);
            return std::abs(Dot(displacement, AxisX().Vector())) <=
                       HalfExtents_.X() + margin &&
                   std::abs(Dot(displacement, AxisY().Vector())) <=
                       HalfExtents_.Y() + margin &&
                   std::abs(Dot(displacement, AxisZ().Vector())) <=
                       HalfExtents_.Z() + margin;
        }

        [[nodiscard]] Point3<T> Support(const Vec3<T>& direction) const noexcept
        {
            const Vec3<T> x = AxisX().Vector();
            const Vec3<T> y = AxisY().Vector();
            const Vec3<T> z = AxisZ().Vector();
            return Center_ +
                   x * (Dot(direction, x) >= T(0) ? HalfExtents_.X()
                                                  : -HalfExtents_.X()) +
                   y * (Dot(direction, y) >= T(0) ? HalfExtents_.Y()
                                                  : -HalfExtents_.Y()) +
                   z * (Dot(direction, z) >= T(0) ? HalfExtents_.Z()
                                                  : -HalfExtents_.Z());
        }

        [[nodiscard]] std::array<Point3<T>, 8> Corners() const noexcept
        {
            const Vec3<T> x = AxisX().Vector() * HalfExtents_.X();
            const Vec3<T> y = AxisY().Vector() * HalfExtents_.Y();
            const Vec3<T> z = AxisZ().Vector() * HalfExtents_.Z();
            return {Center_ - x - y - z, Center_ + x - y - z,
                    Center_ - x + y - z, Center_ + x + y - z,
                    Center_ - x - y + z, Center_ + x - y + z,
                    Center_ - x + y + z, Center_ + x + y + z};
        }

        [[nodiscard]] friend bool operator==(const Obb3& left,
                                             const Obb3& right) noexcept
        {
            return left.Center_ == right.Center_ &&
                   left.HalfExtents_ == right.HalfExtents_ &&
                   left.Orientation_ == right.Orientation_;
        }

    private:
        struct UncheckedTag
        {
        };

        Obb3(const Point3<T>& center,
             const Vec3<T>& halfExtents,
             const Rotation3<T>& orientation,
             UncheckedTag) noexcept :
            Center_(center),
            HalfExtents_(halfExtents),
            Orientation_(orientation)
        {
        }

        Point3<T> Center_{};
        Vec3<T> HalfExtents_{};
        Rotation3<T> Orientation_{};
    };

    using Obb3f = Obb3<float>;
    using Obb3d = Obb3<double>;

    template <typename T>
    [[nodiscard]] inline Obb3<T> TransformObb(
        const RigidTransform3<T>& transform, const Obb3<T>& box) noexcept
    {
        return *Obb3<T>::TryFromCenterHalfExtents(
            TransformPoint(transform, box.Center()), box.HalfExtents(),
            transform.Rotation() * box.Orientation());
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Aabb3<T>> TryTransformConservativeAabb(
        const AffineTransform3<T>& transform, const Obb3<T>& box) noexcept
    {
        std::array<Point3<T>, 8> corners = box.Corners();
        for (Point3<T>& corner : corners)
        {
            corner = TransformPoint(transform, corner);
        }
        return Aabb3<T>::TryFromPoints(std::span<const Point3<T>>(corners));
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Obb3f) == 48);
static_assert(alignof(mv::math::Obb3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Obb3f>);
static_assert(std::is_standard_layout_v<mv::math::Obb3f>);
static_assert(sizeof(mv::math::Obb3d) == 96);
static_assert(alignof(mv::math::Obb3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Obb3d>);
static_assert(std::is_standard_layout_v<mv::math::Obb3d>);
