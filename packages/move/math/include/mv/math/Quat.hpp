#pragma once

#include <type_traits>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Mat3;

    template <typename T>
        requires std::is_floating_point_v<T>
    class alignas(sizeof(T) * 4U) Quat
    {
    public:
        constexpr Quat() noexcept : Components_{T(0), T(0), T(0), T(1)}
        {
        }

        constexpr Quat(T x, T y, T z, T w) noexcept : Components_{x, y, z, w}
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

        [[nodiscard]] friend constexpr bool operator==(
            const Quat&, const Quat&) noexcept = default;

    private:
        T Components_[4];

        template <typename U>
            requires std::is_floating_point_v<U>
        friend class Mat3;
    };

    using Quatf = Quat<float>;
    using Quatd = Quat<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Quatf) == 16);
static_assert(alignof(mv::math::Quatf) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Quatf>);
static_assert(std::is_standard_layout_v<mv::math::Quatf>);
