#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Standard three-finite-point triangle; degeneracy and optional-normal
    // policy are Move-specific.
    template <typename T>
        requires std::is_floating_point_v<T>
    class Triangle3
    {
    public:
        using Component = T;

        Triangle3() noexcept = default;

        [[nodiscard]] static std::optional<Triangle3> TryFromPoints(
            const Point3<T>& first,
            const Point3<T>& second,
            const Point3<T>& third) noexcept
        {
            if (!IsFinitePoint(first) || !IsFinitePoint(second) ||
                !IsFinitePoint(third))
            {
                return std::nullopt;
            }
            return Triangle3(first, second, third, UncheckedTag{});
        }

        [[nodiscard]] const Point3<T>& First() const noexcept
        {
            return Vertices_[0];
        }

        [[nodiscard]] const Point3<T>& Second() const noexcept
        {
            return Vertices_[1];
        }

        [[nodiscard]] const Point3<T>& Third() const noexcept
        {
            return Vertices_[2];
        }

        [[nodiscard]] Vec3<T> Edge01() const noexcept
        {
            return Second() - First();
        }

        [[nodiscard]] Vec3<T> Edge02() const noexcept
        {
            return Third() - First();
        }

        [[nodiscard]] Vec3<T> Edge12() const noexcept
        {
            return Third() - Second();
        }

        [[nodiscard]] Point3<T> Centroid() const noexcept
        {
            return Point3<T>::FromVector(
                (First().Vector() + Second().Vector() + Third().Vector()) *
                (T(1) / T(3)));
        }

        [[nodiscard]] T DoubleArea() const noexcept
        {
            return std::sqrt(LengthSquared(Cross(Edge01(), Edge02())));
        }

        [[nodiscard]] T Area() const noexcept
        {
            return DoubleArea() * T(0.5);
        }

        [[nodiscard]] std::optional<Normal3<T>> TryNormal() const noexcept
        {
            return Normal3<T>::TryFrom(Cross(Edge01(), Edge02()));
        }

        [[nodiscard]] friend bool operator==(const Triangle3& left,
                                             const Triangle3& right) noexcept
        {
            return left.Vertices_[0] == right.Vertices_[0] &&
                   left.Vertices_[1] == right.Vertices_[1] &&
                   left.Vertices_[2] == right.Vertices_[2];
        }

    private:
        struct UncheckedTag
        {
        };

        Triangle3(const Point3<T>& first,
                  const Point3<T>& second,
                  const Point3<T>& third,
                  UncheckedTag) noexcept :
            Vertices_{first, second, third}
        {
        }

        [[nodiscard]] static bool IsFinitePoint(const Point3<T>& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        Point3<T> Vertices_[3];
    };

    using Triangle3f = Triangle3<float>;
    using Triangle3d = Triangle3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Triangle3f) == 48);
static_assert(alignof(mv::math::Triangle3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Triangle3f>);
static_assert(std::is_standard_layout_v<mv::math::Triangle3f>);
static_assert(sizeof(mv::math::Triangle3d) == 96);
static_assert(alignof(mv::math::Triangle3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Triangle3d>);
static_assert(std::is_standard_layout_v<mv::math::Triangle3d>);
