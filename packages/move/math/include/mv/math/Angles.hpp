#pragma once

#include <type_traits>

#include <mv/math/Constants.hpp>

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
        return Radians<T>(value.Value() * (Pi<T> / T(180)));
    }

    template <typename T>
    [[nodiscard]] constexpr Degrees<T> ToDegrees(Radians<T> value) noexcept
    {
        return Degrees<T>(value.Value() * (T(180) / Pi<T>));
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
