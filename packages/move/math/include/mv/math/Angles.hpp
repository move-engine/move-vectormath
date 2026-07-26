#pragma once

#include <type_traits>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Radians
    {
    public:
        constexpr explicit Radians(T value) noexcept : Value_(value)
        {
        }

        [[nodiscard]] constexpr T Value() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] friend constexpr bool operator==(
            Radians, Radians) noexcept = default;

    private:
        T Value_;
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    class Degrees
    {
    public:
        constexpr explicit Degrees(T value) noexcept : Value_(value)
        {
        }

        [[nodiscard]] constexpr T Value() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] friend constexpr bool operator==(
            Degrees, Degrees) noexcept = default;

    private:
        T Value_;
    };

    using Radiansf = Radians<float>;
    using Radiansd = Radians<double>;
    using Degreesf = Degrees<float>;
    using Degreesd = Degrees<double>;

    template <typename T>
    [[nodiscard]] constexpr Radians<T> ToRadians(Degrees<T> value) noexcept
    {
        return Radians<T>(
            value.Value() *
            static_cast<T>(3.14159265358979323846264338327950288L / 180.0L));
    }

    template <typename T>
    [[nodiscard]] constexpr Degrees<T> ToDegrees(Radians<T> value) noexcept
    {
        return Degrees<T>(
            value.Value() *
            static_cast<T>(180.0L / 3.14159265358979323846264338327950288L));
    }

    namespace literals
    {
        [[nodiscard]] constexpr Degreesf operator""_deg(
            long double value) noexcept
        {
            return Degreesf(static_cast<float>(value));
        }

        [[nodiscard]] constexpr Degreesf operator""_deg(
            unsigned long long value) noexcept
        {
            return Degreesf(static_cast<float>(value));
        }

        [[nodiscard]] constexpr Radiansf operator""_rad(
            long double value) noexcept
        {
            return Radiansf(static_cast<float>(value));
        }

        [[nodiscard]] constexpr Radiansf operator""_rad(
            unsigned long long value) noexcept
        {
            return Radiansf(static_cast<float>(value));
        }
    }  // namespace literals
}  // namespace mv::math

static_assert(sizeof(mv::math::Radiansf) == sizeof(float));
static_assert(sizeof(mv::math::Degreesf) == sizeof(float));
static_assert(std::is_trivially_copyable_v<mv::math::Radiansf>);
static_assert(std::is_trivially_copyable_v<mv::math::Degreesf>);
