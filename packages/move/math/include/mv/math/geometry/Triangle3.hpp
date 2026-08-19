#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/semantic/Normal3.hpp>
#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    class Triangle3f
    {
    public:
        Triangle3f() noexcept = default;

        Triangle3f(Point3f first, Point3f second, Point3f third) noexcept :
            Vertices_{first, second, third}
        {
        }

        [[nodiscard]] const Point3f& First() const noexcept
        {
            return Vertices_[0];
        }

        [[nodiscard]] const Point3f& Second() const noexcept
        {
            return Vertices_[1];
        }

        [[nodiscard]] const Point3f& Third() const noexcept
        {
            return Vertices_[2];
        }

        [[nodiscard]] Vec3f Edge01() const noexcept
        {
            return Second() - First();
        }

        [[nodiscard]] Vec3f Edge02() const noexcept
        {
            return Third() - First();
        }

        [[nodiscard]] Vec3f Edge12() const noexcept
        {
            return Third() - Second();
        }

        [[nodiscard]] Point3f Centroid() const noexcept
        {
            return Point3f::FromVector(
                (First().Vector() + Second().Vector() + Third().Vector()) *
                (1.0F / 3.0F));
        }

        [[nodiscard]] float DoubleArea() const noexcept
        {
            return std::sqrt(LengthSquared(Cross(Edge01(), Edge02())));
        }

        [[nodiscard]] float Area() const noexcept
        {
            return DoubleArea() * 0.5F;
        }

        [[nodiscard]] std::optional<Normal3f> TryNormal() const noexcept
        {
            return Normal3f::TryFrom(Cross(Edge01(), Edge02()));
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            return IsFinitePoint(First()) && IsFinitePoint(Second()) &&
                   IsFinitePoint(Third());
        }

        [[nodiscard]] friend bool operator==(const Triangle3f& left,
                                             const Triangle3f& right) noexcept
        {
            return left.Vertices_[0] == right.Vertices_[0] &&
                   left.Vertices_[1] == right.Vertices_[1] &&
                   left.Vertices_[2] == right.Vertices_[2];
        }

    private:
        [[nodiscard]] static bool IsFinitePoint(const Point3f& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        Point3f Vertices_[3];
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Triangle3f) == 48);
static_assert(alignof(mv::math::Triangle3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Triangle3f>);
static_assert(std::is_standard_layout_v<mv::math::Triangle3f>);
