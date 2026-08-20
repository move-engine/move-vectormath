#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mv/math/Tolerance.hpp>
#include <mv/math/detail/VectorOps.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_arithmetic_v<T>
    class alignas(sizeof(T) * 2U) Vec2
    {
    public:
        using Component = T;

        constexpr Vec2() noexcept = default;

        constexpr Vec2(T x, T y) noexcept : Components_{x, y}
        {
        }

        template <typename U>
            requires(std::is_arithmetic_v<U> && !std::is_same_v<T, U>)
        explicit constexpr Vec2(const Vec2<U>& other) noexcept :
            Components_{static_cast<T>(other.X()), static_cast<T>(other.Y())}
        {
        }

        [[nodiscard]] static constexpr Vec2 Zero() noexcept
        {
            return Vec2(T(0), T(0));
        }

        [[nodiscard]] static constexpr Vec2 One() noexcept
        {
            return Vec2(T(1), T(1));
        }

        [[nodiscard]] static constexpr Vec2 Filled(T value) noexcept
        {
            return Vec2(value, value);
        }

        [[nodiscard]] static constexpr Vec2 AxisX() noexcept
        {
            return Vec2(T(1), T(0));
        }

        [[nodiscard]] static constexpr Vec2 AxisY() noexcept
        {
            return Vec2(T(0), T(1));
        }

        [[nodiscard]] constexpr T X() const noexcept
        {
            return Components_[0];
        }

        [[nodiscard]] constexpr T Y() const noexcept
        {
            return Components_[1];
        }

        [[nodiscard]] constexpr T operator[](std::size_t index) const noexcept
        {
            assert(index < 2U);
            return Components_[index];
        }

        template <std::size_t Index>
        [[nodiscard]] constexpr T Get() const noexcept
        {
            static_assert(Index < 2U);
            return Components_[Index];
        }

        constexpr void SetX(T value) noexcept
        {
            Components_[0] = value;
        }

        constexpr void SetY(T value) noexcept
        {
            Components_[1] = value;
        }

        [[nodiscard]] constexpr bool SetComponent(std::size_t index,
                                                  T value) noexcept
        {
            if (index >= 2U)
            {
                return false;
            }
            Components_[index] = value;
            return true;
        }

        template <std::size_t Index>
        constexpr void Set(T value) noexcept
        {
            static_assert(Index < 2U);
            Components_[Index] = value;
        }

        [[nodiscard]] Vec2 operator+(const Vec2& other) const noexcept
        {
            return FromNative(Ops::Add(Ops::Load2(Components_),
                                       Ops::Load2(other.Components_)));
        }

        [[nodiscard]] Vec2 operator-(const Vec2& other) const noexcept
        {
            return FromNative(Ops::Subtract(Ops::Load2(Components_),
                                            Ops::Load2(other.Components_)));
        }

        [[nodiscard]] Vec2 operator*(const Vec2& other) const noexcept
        {
            return FromNative(Ops::Multiply(Ops::Load2(Components_),
                                            Ops::Load2(other.Components_)));
        }

        [[nodiscard]] Vec2 operator/(const Vec2& other) const noexcept
        {
            return FromNative(Ops::Divide2(Ops::Load2(Components_),
                                           Ops::Load2(other.Components_)));
        }

        [[nodiscard]] Vec2 operator+(T scalar) const noexcept
        {
            return FromNative(Ops::Add(Ops::Load2(Components_), scalar));
        }

        [[nodiscard]] Vec2 operator-(T scalar) const noexcept
        {
            return FromNative(Ops::Subtract(Ops::Load2(Components_), scalar));
        }

        [[nodiscard]] Vec2 operator*(T scalar) const noexcept
        {
            return FromNative(Ops::Multiply(Ops::Load2(Components_), scalar));
        }

        [[nodiscard]] Vec2 operator/(T scalar) const noexcept
        {
            return FromNative(Ops::Divide(Ops::Load2(Components_), scalar));
        }

        [[nodiscard]] Vec2 operator-() const noexcept
        {
            return FromNative(Ops::Negate(Ops::Load2(Components_)));
        }

        Vec2& operator+=(const Vec2& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        Vec2& operator-=(const Vec2& other) noexcept
        {
            *this = *this - other;
            return *this;
        }

        Vec2& operator*=(const Vec2& other) noexcept
        {
            *this = *this * other;
            return *this;
        }

        Vec2& operator/=(const Vec2& other) noexcept
        {
            *this = *this / other;
            return *this;
        }

        Vec2& operator+=(T scalar) noexcept
        {
            *this = *this + scalar;
            return *this;
        }

        Vec2& operator-=(T scalar) noexcept
        {
            *this = *this - scalar;
            return *this;
        }

        Vec2& operator*=(T scalar) noexcept
        {
            *this = *this * scalar;
            return *this;
        }

        Vec2& operator/=(T scalar) noexcept
        {
            *this = *this / scalar;
            return *this;
        }

        [[nodiscard]] friend Vec2 operator*(T scalar,
                                            const Vec2& vector) noexcept
        {
            return vector * scalar;
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Vec2&, const Vec2&) noexcept = default;

    private:
        // A compact two-lane value is intentionally scalar-backed. Expanding
        // every Vec2f into a four-lane SIMD register inhibits the compiler's
        // more effective loop-level vectorization for arrays of 8-byte values.
        using Ops = detail::ScalarVectorOps<T>;

        [[nodiscard]] static Vec2 FromNative(
            typename Ops::Native value) noexcept
        {
            Vec2 result;
            Ops::Store2(value, result.Components_);
            return result;
        }

        T Components_[2]{};
    };

    using Vec2f = Vec2<float>;
    using Vec2d = Vec2<double>;
    using Vec2i8 = Vec2<std::int8_t>;
    using Vec2u8 = Vec2<std::uint8_t>;
    using Vec2i16 = Vec2<std::int16_t>;
    using Vec2u16 = Vec2<std::uint16_t>;
    using Vec2i32 = Vec2<std::int32_t>;
    using Vec2u32 = Vec2<std::uint32_t>;
    using Vec2i64 = Vec2<std::int64_t>;
    using Vec2u64 = Vec2<std::uint64_t>;
    using Vec2i = Vec2i32;
    using Vec2u = Vec2u32;

    template <typename T>
    [[nodiscard]] T Dot(const Vec2<T>& left, const Vec2<T>& right) noexcept
    {
        using Ops = detail::ScalarVectorOps<T>;
        const T leftValues[2] = {left.X(), left.Y()};
        const T rightValues[2] = {right.X(), right.Y()};
        return Ops::Dot2(Ops::Load2(leftValues), Ops::Load2(rightValues));
    }

    template <typename T>
    [[nodiscard]] T LengthSquared(const Vec2<T>& value) noexcept
    {
        return Dot(value, value);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyEqual(
        const Vec2<T>& left,
        const Vec2<T>& right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        return IsNearlyEqual(left.X(), right.X(), tolerance) &&
               IsNearlyEqual(left.Y(), right.Y(), tolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyZero(
        const Vec2<T>& value,
        T absoluteTolerance = Tolerance<T>::Default().Absolute) noexcept
    {
        return IsNearlyZero(value.X(), absoluteTolerance) &&
               IsNearlyZero(value.Y(), absoluteTolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Length(const Vec2<T>& value) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's length operation.
        return std::sqrt(LengthSquared(value));
    }

    template <typename T>
    [[nodiscard]] T DistanceSquared(const Vec2<T>& left,
                                    const Vec2<T>& right) noexcept
    {
        return LengthSquared(left - right);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Distance(const Vec2<T>& left, const Vec2<T>& right) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's distance operation.
        return Length(left - right);
    }

    template <typename T>
    [[nodiscard]] Vec2<T> Min(const Vec2<T>& left,
                              const Vec2<T>& right) noexcept
    {
        return Vec2<T>(std::min(left.X(), right.X()),
                       std::min(left.Y(), right.Y()));
    }

    template <typename T>
    [[nodiscard]] Vec2<T> Max(const Vec2<T>& left,
                              const Vec2<T>& right) noexcept
    {
        return Vec2<T>(std::max(left.X(), right.X()),
                       std::max(left.Y(), right.Y()));
    }

    template <typename T>
    [[nodiscard]] Vec2<T> Clamp(const Vec2<T>& value,
                                const Vec2<T>& minimum,
                                const Vec2<T>& maximum) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's clamp operation.
        return Min(Max(value, minimum), maximum);
    }

    template <typename T>
    [[nodiscard]] Vec2<T> Clamp(const Vec2<T>& value,
                                T minimum,
                                T maximum) noexcept
    {
        return Clamp(value, Vec2<T>::Filled(minimum), Vec2<T>::Filled(maximum));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec2<T> Abs(const Vec2<T>& value) noexcept
    {
        return Vec2<T>(static_cast<T>(std::abs(value.X())),
                       static_cast<T>(std::abs(value.Y())));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec2<T> Sign(const Vec2<T>& value) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's sign operation.
        const auto sign = [](T component)
        {
            return static_cast<T>((T(0) < component) - (component < T(0)));
        };
        return Vec2<T>(sign(value.X()), sign(value.Y()));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec2<T> LerpUnclamped(const Vec2<T>& from,
                                        const Vec2<T>& to,
                                        T amount) noexcept
    {
        // Algebra follows GLSL 4.60 section 8.3's mix operation.
        return from + (to - from) * amount;
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec2<T> Lerp(const Vec2<T>& from,
                               const Vec2<T>& to,
                               T amount) noexcept
    {
        return LerpUnclamped(from, to, std::clamp(amount, T(0), T(1)));
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec2f) == 8);
static_assert(alignof(mv::math::Vec2f) == 8);
static_assert(std::is_trivially_copyable_v<mv::math::Vec2f>);
static_assert(std::is_standard_layout_v<mv::math::Vec2f>);
