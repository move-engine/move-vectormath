#pragma once

#include <cstdint>
#include <type_traits>

#include <mv/math/Vec3.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    class Point3
    {
    public:
        using Component = T;

        Point3() noexcept = default;

        Point3(T x, T y, T z) noexcept : Value_(x, y, z)
        {
        }

        [[nodiscard]] static Point3 Origin() noexcept
        {
            return Point3();
        }

        [[nodiscard]] static Point3 FromVector(const Vec3<T>& value) noexcept
        {
            return Point3(value);
        }

        [[nodiscard]] const Vec3<T>& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] T X() const noexcept
        {
            return Value_.X();
        }

        [[nodiscard]] T Y() const noexcept
        {
            return Value_.Y();
        }

        [[nodiscard]] T Z() const noexcept
        {
            return Value_.Z();
        }

        [[nodiscard]] Point3 operator+(
            const Vec3<T>& displacement) const noexcept
        {
            return Point3(Value_ + displacement);
        }

        [[nodiscard]] Point3 operator-(
            const Vec3<T>& displacement) const noexcept
        {
            return Point3(Value_ - displacement);
        }

        [[nodiscard]] Vec3<T> operator-(const Point3& other) const noexcept
        {
            return Value_ - other.Value_;
        }

        Point3& operator+=(const Vec3<T>& displacement) noexcept
        {
            Value_ += displacement;
            return *this;
        }

        Point3& operator-=(const Vec3<T>& displacement) noexcept
        {
            Value_ -= displacement;
            return *this;
        }

        [[nodiscard]] friend bool operator==(const Point3& left,
                                             const Point3& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        explicit Point3(const Vec3<T>& value) noexcept : Value_(value)
        {
        }

        Vec3<T> Value_;
    };

    using Point3f = Point3<float>;
    using Point3d = Point3<double>;
    using Point3i8 = Point3<std::int8_t>;
    using Point3u8 = Point3<std::uint8_t>;
    using Point3i16 = Point3<std::int16_t>;
    using Point3u16 = Point3<std::uint16_t>;
    using Point3i32 = Point3<std::int32_t>;
    using Point3u32 = Point3<std::uint32_t>;
    using Point3i64 = Point3<std::int64_t>;
    using Point3u64 = Point3<std::uint64_t>;
    using Point3i = Point3i32;
    using Point3u = Point3u32;
}  // namespace mv::math

static_assert(sizeof(mv::math::Point3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Point3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Point3f>);
static_assert(std::is_standard_layout_v<mv::math::Point3f>);
static_assert(sizeof(mv::math::Point3d) == sizeof(mv::math::Vec3d));
static_assert(alignof(mv::math::Point3d) == alignof(mv::math::Vec3d));
static_assert(std::is_trivially_copyable_v<mv::math::Point3d>);
static_assert(std::is_standard_layout_v<mv::math::Point3d>);
