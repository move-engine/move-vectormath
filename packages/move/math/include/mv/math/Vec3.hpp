#pragma once

#include <type_traits>

#include <mv/math/detail/VectorOps.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    class alignas(sizeof(T) * 4U) Vec3
    {
    private:
        using Ops = detail::SelectedVectorOps<T>;

    public:
        using Component = T;

        Vec3() noexcept : Native_(Ops::Load4(Zero_))
        {
        }

        Vec3(T x, T y, T z) noexcept
        {
            const T values[3] = {x, y, z};
            Native_ = Ops::Load3(values);
        }

        [[nodiscard]] T X() const noexcept
        {
            return Ops::GetX(Native_);
        }

        [[nodiscard]] T Y() const noexcept
        {
            return Ops::GetY(Native_);
        }

        [[nodiscard]] T Z() const noexcept
        {
            return Ops::GetZ(Native_);
        }

        void SetX(T value) noexcept
        {
            Native_ = Ops::SetX(Native_, value);
        }

        void SetY(T value) noexcept
        {
            Native_ = Ops::SetY(Native_, value);
        }

        void SetZ(T value) noexcept
        {
            Native_ = Ops::SetZ(Native_, value);
        }

        [[nodiscard]] Vec3 operator+(const Vec3& other) const noexcept
        {
            return Vec3(Ops::Add(Native_, other.Native_));
        }

        [[nodiscard]] Vec3 operator-(const Vec3& other) const noexcept
        {
            return Vec3(Ops::Subtract(Native_, other.Native_));
        }

        [[nodiscard]] Vec3 operator*(const Vec3& other) const noexcept
        {
            return Vec3(Ops::Multiply(Native_, other.Native_));
        }

        [[nodiscard]] Vec3 operator*(T scalar) const noexcept
        {
            return Vec3(Ops::Multiply(Native_, scalar));
        }

        Vec3& operator+=(const Vec3& other) noexcept
        {
            Native_ = Ops::Add(Native_, other.Native_);
            return *this;
        }

        Vec3& operator-=(const Vec3& other) noexcept
        {
            Native_ = Ops::Subtract(Native_, other.Native_);
            return *this;
        }

        [[nodiscard]] friend bool operator==(const Vec3& left,
                                             const Vec3& right) noexcept
        {
            return left.X() == right.X() && left.Y() == right.Y() &&
                   left.Z() == right.Z();
        }

    private:
        explicit Vec3(typename Ops::Native native) noexcept : Native_(native)
        {
        }

        inline static constexpr T Zero_[4] = {T(0), T(0), T(0), T(0)};
        typename Ops::Native Native_;

        template <typename U>
            requires std::is_arithmetic_v<U>
        friend U Dot(const Vec3<U>&, const Vec3<U>&) noexcept;

        template <typename U>
            requires std::is_arithmetic_v<U>
        friend Vec3<U> Cross(const Vec3<U>&, const Vec3<U>&) noexcept;
    };

    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;

    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] T Dot(const Vec3<T>& left, const Vec3<T>& right) noexcept
    {
        using Ops = detail::SelectedVectorOps<T>;
        return Ops::Dot3(left.Native_, right.Native_);
    }

    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] Vec3<T> Cross(const Vec3<T>& left,
                                const Vec3<T>& right) noexcept
    {
        using Ops = detail::SelectedVectorOps<T>;
        return Vec3<T>(Ops::Cross3(left.Native_, right.Native_));
    }

    template <typename T>
    [[nodiscard]] T LengthSquared(const Vec3<T>& value) noexcept
    {
        return Dot(value, value);
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec3f) == 16);
static_assert(alignof(mv::math::Vec3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Vec3f>);
static_assert(std::is_standard_layout_v<mv::math::Vec3f>);
