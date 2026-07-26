#pragma once

#include <type_traits>

#include <mv/math/detail/VectorOps.hpp>

namespace mv::math
{
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

        [[nodiscard]] Vec4 operator+(const Vec4& other) const noexcept
        {
            using Ops = detail::SelectedVectorOps<T>;
            return FromNative(Ops::Add(Ops::Load4(Components_),
                                       Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator-(const Vec4& other) const noexcept
        {
            using Ops = detail::SelectedVectorOps<T>;
            return FromNative(Ops::Subtract(Ops::Load4(Components_),
                                            Ops::Load4(other.Components_)));
        }

        [[nodiscard]] Vec4 operator*(T scalar) const noexcept
        {
            using Ops = detail::SelectedVectorOps<T>;
            return FromNative(Ops::Multiply(Ops::Load4(Components_), scalar));
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
    };

    using Vec4f = Vec4<float>;
    using Vec4d = Vec4<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Vec4f) == 16);
static_assert(alignof(mv::math::Vec4f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Vec4f>);
static_assert(std::is_standard_layout_v<mv::math::Vec4f>);
