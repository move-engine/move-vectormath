#pragma once

#include <algorithm>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mv/math/Vec2.hpp>
#include <mv/math/detail/VectorOps.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Mat3;

    template <typename T>
        requires std::is_arithmetic_v<T>
    class Vec3;

    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] T Dot(const Vec3<T>& left, const Vec3<T>& right) noexcept;

    template <typename T>
        requires std::is_arithmetic_v<T>
    [[nodiscard]] Vec3<T> Cross(const Vec3<T>& left,
                                const Vec3<T>& right) noexcept;

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

        template <typename U>
            requires(std::is_arithmetic_v<U> && !std::is_same_v<T, U>)
        explicit Vec3(const Vec3<U>& other) noexcept :
            Vec3(static_cast<T>(other.X()),
                 static_cast<T>(other.Y()),
                 static_cast<T>(other.Z()))
        {
        }

        template <typename U>
            requires std::is_arithmetic_v<U>
        explicit Vec3(const Vec2<U>& xy, T z = T(0)) noexcept :
            Vec3(static_cast<T>(xy.X()), static_cast<T>(xy.Y()), z)
        {
        }

        [[nodiscard]] static Vec3 Zero() noexcept
        {
            return Vec3(T(0), T(0), T(0));
        }

        [[nodiscard]] static Vec3 One() noexcept
        {
            return Vec3(T(1), T(1), T(1));
        }

        [[nodiscard]] static Vec3 Filled(T value) noexcept
        {
            return Vec3(value, value, value);
        }

        [[nodiscard]] static Vec3 AxisX() noexcept
        {
            return Vec3(T(1), T(0), T(0));
        }

        [[nodiscard]] static Vec3 AxisY() noexcept
        {
            return Vec3(T(0), T(1), T(0));
        }

        [[nodiscard]] static Vec3 AxisZ() noexcept
        {
            return Vec3(T(0), T(0), T(1));
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

        [[nodiscard]] T operator[](std::size_t index) const noexcept
        {
            assert(index < 3U);
            switch (index)
            {
                case 0U:
                    return X();
                case 1U:
                    return Y();
                default:
                    return Z();
            }
        }

        template <std::size_t Index>
        [[nodiscard]] T Get() const noexcept
        {
            static_assert(Index < 3U);
            if constexpr (Index == 0U)
            {
                return X();
            }
            else if constexpr (Index == 1U)
            {
                return Y();
            }
            else
            {
                return Z();
            }
        }

        [[nodiscard]] Vec2<T> XY() const noexcept
        {
            return Vec2<T>(X(), Y());
        }

        [[nodiscard]] Vec2<T> XZ() const noexcept
        {
            return Vec2<T>(X(), Z());
        }

        [[nodiscard]] Vec2<T> YZ() const noexcept
        {
            return Vec2<T>(Y(), Z());
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

        [[nodiscard]] bool SetComponent(std::size_t index, T value) noexcept
        {
            switch (index)
            {
                case 0U:
                    SetX(value);
                    return true;
                case 1U:
                    SetY(value);
                    return true;
                case 2U:
                    SetZ(value);
                    return true;
                default:
                    return false;
            }
        }

        template <std::size_t Index>
        void Set(T value) noexcept
        {
            static_assert(Index < 3U);
            if constexpr (Index == 0U)
            {
                SetX(value);
            }
            else if constexpr (Index == 1U)
            {
                SetY(value);
            }
            else
            {
                SetZ(value);
            }
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

        [[nodiscard]] Vec3 operator/(const Vec3& other) const noexcept
        {
            return Vec3(Ops::Divide3(Native_, other.Native_));
        }

        [[nodiscard]] Vec3 operator+(T scalar) const noexcept
        {
            return Vec3(Ops::Add(Native_, scalar));
        }

        [[nodiscard]] Vec3 operator-(T scalar) const noexcept
        {
            return Vec3(Ops::Subtract(Native_, scalar));
        }

        [[nodiscard]] Vec3 operator/(T scalar) const noexcept
        {
            return Vec3(Ops::Divide(Native_, scalar));
        }

        [[nodiscard]] Vec3 operator-() const noexcept
        {
            return Vec3(Ops::Negate(Native_));
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

        Vec3& operator*=(const Vec3& other) noexcept
        {
            Native_ = Ops::Multiply(Native_, other.Native_);
            return *this;
        }

        Vec3& operator/=(const Vec3& other) noexcept
        {
            Native_ = Ops::Divide3(Native_, other.Native_);
            return *this;
        }

        Vec3& operator+=(T scalar) noexcept
        {
            Native_ = Ops::Add(Native_, scalar);
            return *this;
        }

        Vec3& operator-=(T scalar) noexcept
        {
            Native_ = Ops::Subtract(Native_, scalar);
            return *this;
        }

        Vec3& operator*=(T scalar) noexcept
        {
            Native_ = Ops::Multiply(Native_, scalar);
            return *this;
        }

        Vec3& operator/=(T scalar) noexcept
        {
            Native_ = Ops::Divide(Native_, scalar);
            return *this;
        }

        [[nodiscard]] friend Vec3 operator*(T scalar,
                                            const Vec3& vector) noexcept
        {
            return vector * scalar;
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

        friend T Dot<T>(const Vec3&, const Vec3&) noexcept;
        friend Vec3 Cross<T>(const Vec3&, const Vec3&) noexcept;
        template <typename U>
            requires std::is_floating_point_v<U>
        friend class Mat3;
    };

    using Vec3f = Vec3<float>;
    using Vec3d = Vec3<double>;
    using Vec3i8 = Vec3<std::int8_t>;
    using Vec3u8 = Vec3<std::uint8_t>;
    using Vec3i16 = Vec3<std::int16_t>;
    using Vec3u16 = Vec3<std::uint16_t>;
    using Vec3i32 = Vec3<std::int32_t>;
    using Vec3u32 = Vec3<std::uint32_t>;
    using Vec3i64 = Vec3<std::int64_t>;
    using Vec3u64 = Vec3<std::uint64_t>;
    using Vec3i = Vec3i32;
    using Vec3u = Vec3u32;

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

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyEqual(
        const Vec3<T>& left,
        const Vec3<T>& right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        return IsNearlyEqual(left.X(), right.X(), tolerance) &&
               IsNearlyEqual(left.Y(), right.Y(), tolerance) &&
               IsNearlyEqual(left.Z(), right.Z(), tolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] bool IsNearlyZero(
        const Vec3<T>& value,
        T absoluteTolerance = Tolerance<T>::Default().Absolute) noexcept
    {
        return IsNearlyZero(value.X(), absoluteTolerance) &&
               IsNearlyZero(value.Y(), absoluteTolerance) &&
               IsNearlyZero(value.Z(), absoluteTolerance);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Length(const Vec3<T>& value) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's length operation.
        return std::sqrt(LengthSquared(value));
    }

    template <typename T>
    [[nodiscard]] T DistanceSquared(const Vec3<T>& left,
                                    const Vec3<T>& right) noexcept
    {
        return LengthSquared(left - right);
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] T Distance(const Vec3<T>& left, const Vec3<T>& right) noexcept
    {
        // Semantics follow GLSL 4.60 section 8.5's distance operation.
        return Length(left - right);
    }

    template <typename T>
    [[nodiscard]] Vec3<T> Min(const Vec3<T>& left,
                              const Vec3<T>& right) noexcept
    {
        return Vec3<T>(std::min(left.X(), right.X()),
                       std::min(left.Y(), right.Y()),
                       std::min(left.Z(), right.Z()));
    }

    template <typename T>
    [[nodiscard]] Vec3<T> Max(const Vec3<T>& left,
                              const Vec3<T>& right) noexcept
    {
        return Vec3<T>(std::max(left.X(), right.X()),
                       std::max(left.Y(), right.Y()),
                       std::max(left.Z(), right.Z()));
    }

    template <typename T>
    [[nodiscard]] Vec3<T> Clamp(const Vec3<T>& value,
                                const Vec3<T>& minimum,
                                const Vec3<T>& maximum) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's clamp operation.
        return Min(Max(value, minimum), maximum);
    }

    template <typename T>
    [[nodiscard]] Vec3<T> Clamp(const Vec3<T>& value,
                                T minimum,
                                T maximum) noexcept
    {
        return Clamp(value, Vec3<T>::Filled(minimum), Vec3<T>::Filled(maximum));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec3<T> Abs(const Vec3<T>& value) noexcept
    {
        return Vec3<T>(static_cast<T>(std::abs(value.X())),
                       static_cast<T>(std::abs(value.Y())),
                       static_cast<T>(std::abs(value.Z())));
    }

    template <typename T>
        requires(std::is_signed_v<T> || std::is_floating_point_v<T>)
    [[nodiscard]] Vec3<T> Sign(const Vec3<T>& value) noexcept
    {
        // Component semantics follow GLSL 4.60 section 8.3's sign operation.
        const auto sign = [](T component)
        {
            return static_cast<T>((T(0) < component) - (component < T(0)));
        };
        return Vec3<T>(sign(value.X()), sign(value.Y()), sign(value.Z()));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec3<T> LerpUnclamped(const Vec3<T>& from,
                                        const Vec3<T>& to,
                                        T amount) noexcept
    {
        // Algebra follows GLSL 4.60 section 8.3's mix operation.
        return from + (to - from) * amount;
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec3<T> Lerp(const Vec3<T>& from,
                               const Vec3<T>& to,
                               T amount) noexcept
    {
        return LerpUnclamped(from, to, std::clamp(amount, T(0), T(1)));
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec3f) == 16);
static_assert(alignof(mv::math::Vec3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Vec3f>);
static_assert(std::is_standard_layout_v<mv::math::Vec3f>);
