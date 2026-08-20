#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Tolerance.hpp>

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

        [[nodiscard]] constexpr Quat operator+(const Quat& other) const noexcept
        {
            return Quat(X() + other.X(), Y() + other.Y(), Z() + other.Z(),
                        W() + other.W());
        }

        [[nodiscard]] constexpr Quat operator-(const Quat& other) const noexcept
        {
            return Quat(X() - other.X(), Y() - other.Y(), Z() - other.Z(),
                        W() - other.W());
        }

        [[nodiscard]] constexpr Quat operator*(T scalar) const noexcept
        {
            return Quat(X() * scalar, Y() * scalar, Z() * scalar, W() * scalar);
        }

        [[nodiscard]] constexpr Quat operator/(T scalar) const noexcept
        {
            return Quat(X() / scalar, Y() / scalar, Z() / scalar, W() / scalar);
        }

        [[nodiscard]] constexpr Quat operator-() const noexcept
        {
            return Quat(-X(), -Y(), -Z(), -W());
        }

        // Hamilton product; cross-checked against RTM 2.3.1 quat_mul (MIT,
        // commit 745bd25673d93b46941eda55e0993327dbc12b53b).
        [[nodiscard]] constexpr Quat operator*(const Quat& right) const noexcept
        {
            return Quat(W() * right.X() + X() * right.W() + Y() * right.Z() -
                            Z() * right.Y(),
                        W() * right.Y() - X() * right.Z() + Y() * right.W() +
                            Z() * right.X(),
                        W() * right.Z() + X() * right.Y() - Y() * right.X() +
                            Z() * right.W(),
                        W() * right.W() - X() * right.X() - Y() * right.Y() -
                            Z() * right.Z());
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

    template <typename T>
    [[nodiscard]] constexpr T Dot(const Quat<T>& left,
                                  const Quat<T>& right) noexcept
    {
        return left.X() * right.X() + left.Y() * right.Y() +
               left.Z() * right.Z() + left.W() * right.W();
    }

    template <typename T>
    [[nodiscard]] constexpr T LengthSquared(const Quat<T>& value) noexcept
    {
        return Dot(value, value);
    }

    template <typename T>
    [[nodiscard]] inline T Length(const Quat<T>& value) noexcept
    {
        return std::sqrt(LengthSquared(value));
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Quat<T>> TryNormalize(
        const Quat<T>& value) noexcept
    {
        const T maximum = std::max({std::abs(value.X()), std::abs(value.Y()),
                                    std::abs(value.Z()), std::abs(value.W())});
        if (!(maximum > T(0)) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }
        const Quat<T> scaled = value / maximum;
        const T lengthSquared = LengthSquared(scaled);
        if (!(lengthSquared > T(0)) || !std::isfinite(lengthSquared))
        {
            return std::nullopt;
        }
        const T reciprocalLength = T(1) / std::sqrt(lengthSquared);
        const Quat<T> result = scaled * reciprocalLength;
        return std::isfinite(result.X()) && std::isfinite(result.Y()) &&
                       std::isfinite(result.Z()) && std::isfinite(result.W())
                   ? std::optional<Quat<T>>(result)
                   : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Quat<T>> TryInverse(
        const Quat<T>& value) noexcept
    {
        const T maximum = std::max({std::abs(value.X()), std::abs(value.Y()),
                                    std::abs(value.Z()), std::abs(value.W())});
        if (!(maximum > T(0)) || !std::isfinite(maximum))
        {
            return std::nullopt;
        }

        // Scaling before the squared norm follows LAPACK's overflow-avoiding
        // xLASSQ pattern; dividing by maximum last also preserves invertible
        // subnormal and near-maximum finite quaternions.
        const Quat<T> scaled = value / maximum;
        const T scaledLengthSquared = LengthSquared(scaled);
        if (!(scaledLengthSquared > T(0)) ||
            !std::isfinite(scaledLengthSquared))
        {
            return std::nullopt;
        }
        const Quat<T> result =
            Quat<T>(-scaled.X(), -scaled.Y(), -scaled.Z(), scaled.W()) /
            scaledLengthSquared / maximum;
        return std::isfinite(result.X()) && std::isfinite(result.Y()) &&
                       std::isfinite(result.Z()) && std::isfinite(result.W())
                   ? std::optional<Quat<T>>(result)
                   : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline bool IsNearlyEqual(
        const Quat<T>& left,
        const Quat<T>& right,
        Tolerance<T> tolerance = Tolerance<T>::Default()) noexcept
    {
        return IsNearlyEqual(left.X(), right.X(), tolerance) &&
               IsNearlyEqual(left.Y(), right.Y(), tolerance) &&
               IsNearlyEqual(left.Z(), right.Z(), tolerance) &&
               IsNearlyEqual(left.W(), right.W(), tolerance);
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Quatf) == 16);
static_assert(alignof(mv::math::Quatf) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Quatf>);
static_assert(std::is_standard_layout_v<mv::math::Quatf>);
