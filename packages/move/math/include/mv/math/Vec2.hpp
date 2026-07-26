#pragma once

#include <cstddef>
#include <type_traits>

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

        [[nodiscard]] constexpr T X() const noexcept
        {
            return Components_[0];
        }

        [[nodiscard]] constexpr T Y() const noexcept
        {
            return Components_[1];
        }

        constexpr void SetX(T value) noexcept
        {
            Components_[0] = value;
        }

        constexpr void SetY(T value) noexcept
        {
            Components_[1] = value;
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

        [[nodiscard]] Vec2 operator*(T scalar) const noexcept
        {
            return FromNative(Ops::Multiply(Ops::Load2(Components_), scalar));
        }

        Vec2& operator+=(const Vec2& other) noexcept
        {
            *this = *this + other;
            return *this;
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Vec2&, const Vec2&) noexcept = default;

    private:
        using Ops = detail::SelectedVectorOps<T>;

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

    template <typename T>
    [[nodiscard]] T Dot(const Vec2<T>& left, const Vec2<T>& right) noexcept
    {
        using Ops = detail::SelectedVectorOps<T>;
        const T leftValues[2] = {left.X(), left.Y()};
        const T rightValues[2] = {right.X(), right.Y()};
        return Ops::Dot2(Ops::Load2(leftValues), Ops::Load2(rightValues));
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec2f) == 8);
static_assert(alignof(mv::math::Vec2f) == 8);
static_assert(std::is_trivially_copyable_v<mv::math::Vec2f>);
static_assert(std::is_standard_layout_v<mv::math::Vec2f>);
