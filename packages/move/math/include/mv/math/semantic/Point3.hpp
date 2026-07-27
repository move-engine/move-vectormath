#pragma once

#include <type_traits>

#include <mv/math/Vec3.hpp>

namespace mv::math
{
    class Point3f
    {
    public:
        Point3f() noexcept = default;

        Point3f(float x, float y, float z) noexcept : Value_(x, y, z)
        {
        }

        [[nodiscard]] static Point3f Origin() noexcept
        {
            return Point3f();
        }

        [[nodiscard]] static Point3f FromVector(Vec3f value) noexcept
        {
            return Point3f(value);
        }

        [[nodiscard]] const Vec3f& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] float X() const noexcept
        {
            return Value_.X();
        }

        [[nodiscard]] float Y() const noexcept
        {
            return Value_.Y();
        }

        [[nodiscard]] float Z() const noexcept
        {
            return Value_.Z();
        }

        [[nodiscard]] Point3f operator+(
            const Vec3f& displacement) const noexcept
        {
            return Point3f(Value_ + displacement);
        }

        [[nodiscard]] Point3f operator-(
            const Vec3f& displacement) const noexcept
        {
            return Point3f(Value_ - displacement);
        }

        [[nodiscard]] Vec3f operator-(const Point3f& other) const noexcept
        {
            return Value_ - other.Value_;
        }

        Point3f& operator+=(const Vec3f& displacement) noexcept
        {
            Value_ += displacement;
            return *this;
        }

        Point3f& operator-=(const Vec3f& displacement) noexcept
        {
            Value_ -= displacement;
            return *this;
        }

        [[nodiscard]] friend bool operator==(const Point3f& left,
                                             const Point3f& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        explicit Point3f(Vec3f value) noexcept : Value_(value)
        {
        }

        Vec3f Value_;
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Point3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Point3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Point3f>);
static_assert(std::is_standard_layout_v<mv::math::Point3f>);
