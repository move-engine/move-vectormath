#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mv/math/Vec2.hpp>
#include <mv/math/Vec3.hpp>
#include <mv/math/detail/VectorOps.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Mat4;

    template <typename T>
        requires std::is_arithmetic_v<T>
    class alignas(sizeof(T) * 4U) Vec4
    {
    public:
        using Component = T;

        constexpr Vec4() noexcept = default;

        constexpr Vec4(T x, T y, T z, T w) noexcept : Components_{x, y, z, w}
        {
        }

        template <typename U>
            requires(std::is_arithmetic_v<U> && !std::is_same_v<T, U>)
        explicit constexpr Vec4(const Vec4<U>& other) noexcept :
            Components_{static_cast<T>(other.X()), static_cast<T>(other.Y()),
                        static_cast<T>(other.Z()), static_cast<T>(other.W())}
        {
        }

        template <typename U>
            requires std::is_arithmetic_v<U>
        explicit constexpr Vec4(const Vec2<U>& xy,
                                T z = T(0),
                                T w = T(0)) noexcept :
            Components_{static_cast<T>(xy.X()), static_cast<T>(xy.Y()), z, w}
        {
        }

        template <typename U>
            requires std::is_arithmetic_v<U>
        explicit Vec4(const Vec3<U>& xyz, T w = T(0)) noexcept :
            Components_{static_cast<T>(xyz.X()), static_cast<T>(xyz.Y()),
                        static_cast<T>(xyz.Z()), w}
        {
        }

        [[nodiscard]] static constexpr Vec4 Zero() noexcept
        {
            return Vec4(T(0), T(0), T(0), T(0));
        }

        [[nodiscard]] static constexpr Vec4 One() noexcept
        {
            return Vec4(T(1), T(1), T(1), T(1));
        }

        [[nodiscard]] static constexpr Vec4 Filled(T value) noexcept
        {
            return Vec4(value, value, value, value);
        }

        [[nodiscard]] static constexpr Vec4 AxisX() noexcept
        {
            return Vec4(T(1), T(0), T(0), T(0));
        }

        [[nodiscard]] static constexpr Vec4 AxisY() noexcept
        {
            return Vec4(T(0), T(1), T(0), T(0));
        }

        [[nodiscard]] static constexpr Vec4 AxisZ() noexcept
        {
            return Vec4(T(0), T(0), T(1), T(0));
        }

        [[nodiscard]] static constexpr Vec4 AxisW() noexcept
        {
            return Vec4(T(0), T(0), T(0), T(1));
        }

        [[nodiscard]] constexpr T X() const noexcept
        {
            return Components_[0];
        }
        [[nodiscard]] constexpr T Y() const noexcept
        {
            return Components_[1];
        }
        [[nodiscard]] constexpr T Z() const noexcept
        {
            return Components_[2];
        }
        [[nodiscard]] constexpr T W() const noexcept
        {
            return Components_[3];
        }

        [[nodiscard]] constexpr T operator[](std::size_t index) const noexcept
        {
            assert(index < 4U);
            return Components_[index];
        }

        template <std::size_t Index>
        [[nodiscard]] constexpr T Get() const noexcept
        {
            static_assert(Index < 4U);
            return Components_[Index];
        }

        [[nodiscard]] constexpr bool SetComponent(std::size_t index,
                                                  T value) noexcept
        {
            if (index >= 4U)
            {
                return false;
            }
            Components_[index] = value;
            return true;
        }

        template <std::size_t Index>
        constexpr void Set(T value) noexcept
        {
            static_assert(Index < 4U);
            Components_[Index] = value;
        }

        [[nodiscard]] constexpr Vec2<T> XY() const noexcept
        {
            return Vec2<T>(X(), Y());
        }

        [[nodiscard]] constexpr Vec2<T> ZW() const noexcept
        {
            return Vec2<T>(Z(), W());
        }

        [[nodiscard]] Vec3<T> XYZ() const noexcept
        {
            return Vec3<T>(X(), Y(), Z());
        }

        [[nodiscard]] Vec4 operator+(const Vec4& other) const noexcept
        {
            return FromNative(Ops::Add(Ops::Load4(Components_),
                                       Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator-(const Vec4& other) const noexcept
        {
            return FromNative(Ops::Subtract(Ops::Load4(Components_),
                                            Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator*(const Vec4& other) const noexcept
        {
            return FromNative(Ops::Multiply(Ops::Load4(Components_),
                                            Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator/(const Vec4& other) const noexcept
        {
            return FromNative(Ops::Divide(Ops::Load4(Components_),
                                          Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator+(T scalar) const noexcept
        {
            return FromNative(Ops::Add(Ops::Load4(Components_), scalar));
        }

        [[nodiscard]] Vec4 operator-(T scalar) const noexcept
        {
            return FromNative(Ops::Subtract(Ops::Load4(Components_), scalar));
        }

        [[nodiscard]] Vec4 operator*(T scalar) const noexcept
        {
            return FromNative(Ops::Multiply(Ops::Load4(Components_), scalar));
        }

        [[nodiscard]] Vec4 operator/(T scalar) const noexcept
        {
            return FromNative(Ops::Divide(Ops::Load4(Components_), scalar));
        }

        [[nodiscard]] Vec4 operator-() const noexcept
        {
            return FromNative(Ops::Negate(Ops::Load4(Components_)));
        }

        Vec4& operator+=(const Vec4& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        Vec4& operator-=(const Vec4& other) noexcept
        {
            *this = *this - other;
            return *this;
        }

        Vec4& operator*=(const Vec4& other) noexcept
        {
            *this = *this * other;
            return *this;
        }

        Vec4& operator/=(const Vec4& other) noexcept
        {
            *this = *this / other;
            return *this;
        }

        Vec4& operator+=(T scalar) noexcept
        {
            *this = *this + scalar;
            return *this;
        }

        Vec4& operator-=(T scalar) noexcept
        {
            *this = *this - scalar;
            return *this;
        }

        Vec4& operator*=(T scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        Vec4& operator/=(T scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] friend Vec4 operator*(T scalar,
                                            const Vec4& vector) noexcept
        {
            return vector * scalar;
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Vec4&, const Vec4&) noexcept = default;

    private:
        using Ops = detail::SelectedVectorOps<T>;

        [[nodiscard]] static Vec4 FromNative(
            typename Ops::Native value) noexcept
        {
            Vec4 result;
            Ops::Store4(value, result.Components_);
            return result;
        }

        T Components_[4]{};

        template <typename U>
            requires std::is_floating_point_v<U>
        friend class Mat4;
    };

    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;
    using Vec4i8 = Vec4<std::int8_t>;
    using Vec4u8 = Vec4<std::uint8_t>;
    using Vec4i16 = Vec4<std::int16_t>;
    using Vec4u16 = Vec4<std::uint16_t>;
    using Vec4i32 = Vec4<std::int32_t>;
    using Vec4u32 = Vec4<std::uint32_t>;
    using Vec4i64 = Vec4<std::int64_t>;
    using Vec4u64 = Vec4<std::uint64_t>;
    using Vec4i = Vec4i32;
    using Vec4u = Vec4u32;

    template <typename T>
    [[nodiscard]] T Dot(const Vec4<T>& left, const Vec4<T>& right) noexcept
    {
        using Ops = detail::SelectedVectorOps<T>;
        const T leftValues[4] = {left.X(), left.Y(), left.Z(), left.W()};
        const T rightValues[4] = {right.X(), right.Y(), right.Z(), right.W()};
        return Ops::Dot4(Ops::Load4(leftValues), Ops::Load4(rightValues));
    }

    template <typename T>
    [[nodiscard]] T LengthSquared(const Vec4<T>& value) noexcept
    {
        return Dot(value, value);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyEqual(
        const Vec4<T>& left,
        const Vec4<T>& right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        return IsNearlyEqual(left.X(), right.X(), tolerance) &&
               IsNearlyEqual(left.Y(), right.Y(), tolerance) &&
               IsNearlyEqual(left.Z(), right.Z(), tolerance) &&
               IsNearlyEqual(left.W(), right.W(), tolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyZero(
        const Vec4<T>& value,
        T absoluteTolerance = Tolerance<T>::Default().Absolute) noexcept
    {
        return IsNearlyZero(value.X(), absoluteTolerance) &&
               IsNearlyZero(value.Y(), absoluteTolerance) &&
               IsNearlyZero(value.Z(), absoluteTolerance) &&
               IsNearlyZero(value.W(), absoluteTolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Length(const Vec4<T>& value) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's length operation.
        return std::sqrt(LengthSquared(value));
    }

    template <typename T>
    [[nodiscard]] T DistanceSquared(const Vec4<T>& left,
                                    const Vec4<T>& right) noexcept
    {
        return LengthSquared(left - right);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Distance(const Vec4<T>& left, const Vec4<T>& right) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's distance operation.
        return Length(left - right);
    }

    template <typename T>
    [[nodiscard]] Vec4<T> Min(const Vec4<T>& left,
                              const Vec4<T>& right) noexcept
    {
        return Vec4<T>(
            std::min(left.X(), right.X()), std::min(left.Y(), right.Y()),
            std::min(left.Z(), right.Z()), std::min(left.W(), right.W()));
    }

    template <typename T>
    [[nodiscard]] Vec4<T> Max(const Vec4<T>& left,
                              const Vec4<T>& right) noexcept
    {
        return Vec4<T>(
            std::max(left.X(), right.X()), std::max(left.Y(), right.Y()),
            std::max(left.Z(), right.Z()), std::max(left.W(), right.W()));
    }

    template <typename T>
    [[nodiscard]] Vec4<T> Clamp(const Vec4<T>& value,
                                const Vec4<T>& minimum,
                                const Vec4<T>& maximum) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's clamp operation.
        return Min(Max(value, minimum), maximum);
    }

    template <typename T>
    [[nodiscard]] Vec4<T> Clamp(const Vec4<T>& value,
                                T minimum,
                                T maximum) noexcept
    {
        return Clamp(value, Vec4<T>::Filled(minimum), Vec4<T>::Filled(maximum));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec4<T> Abs(const Vec4<T>& value) noexcept
    {
        return Vec4<T>(static_cast<T>(std::abs(value.X())),
                       static_cast<T>(std::abs(value.Y())),
                       static_cast<T>(std::abs(value.Z())),
                       static_cast<T>(std::abs(value.W())));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec4<T> Sign(const Vec4<T>& value) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's sign operation.
        const auto sign = [](T component)
        {
            return static_cast<T>((T(0) < component) - (component < T(0)));
        };
        return Vec4<T>(sign(value.X()), sign(value.Y()), sign(value.Z()),
                       sign(value.W()));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec4<T> LerpUnclamped(const Vec4<T>& from,
                                        const Vec4<T>& to,
                                        T amount) noexcept
    {
        // Algebra follows GLSL 4.60 section 8.3's mix operation.
        return from + (to - from) * amount;
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec4<T> Lerp(const Vec4<T>& from,
                               const Vec4<T>& to,
                               T amount) noexcept
    {
        return LerpUnclamped(from, to, std::clamp(amount, T(0), T(1)));
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec4f) == 16);
static_assert(alignof(mv::math::Vec4f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Vec4f>);
static_assert(std::is_standard_layout_v<mv::math::Vec4f>);
