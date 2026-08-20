#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <optional>
#include <type_traits>

#include <mv/math/Vec4.hpp>
#include <mv/math/detail/Matrix4Ops.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Mat4
    {
    public:
        using Component = T;
        using RowVector = Vec4<T>;

        constexpr Mat4() noexcept :
            Rows_{RowVector(T(1), T(0), T(0), T(0)),
                  RowVector(T(0), T(1), T(0), T(0)),
                  RowVector(T(0), T(0), T(1), T(0)),
                  RowVector(T(0), T(0), T(0), T(1))}
        {
        }

        constexpr Mat4(const RowVector& row0,
                       const RowVector& row1,
                       const RowVector& row2,
                       const RowVector& row3) noexcept :
            Rows_{row0, row1, row2, row3}
        {
        }

        constexpr Mat4(T m11,
                       T m12,
                       T m13,
                       T m14,
                       T m21,
                       T m22,
                       T m23,
                       T m24,
                       T m31,
                       T m32,
                       T m33,
                       T m34,
                       T m41,
                       T m42,
                       T m43,
                       T m44) noexcept :
            Mat4(RowVector(m11, m12, m13, m14),
                 RowVector(m21, m22, m23, m24),
                 RowVector(m31, m32, m33, m34),
                 RowVector(m41, m42, m43, m44))
        {
        }

        [[nodiscard]] static constexpr Mat4 Identity() noexcept
        {
            return Mat4();
        }

        [[nodiscard]] static constexpr Mat4 Zero() noexcept
        {
            return Mat4(RowVector(), RowVector(), RowVector(), RowVector());
        }

        [[nodiscard]] constexpr const RowVector& Row(
            std::size_t index) const noexcept
        {
            assert(index < 4U);
            return Rows_[index];
        }

        [[nodiscard]] constexpr RowVector Column(
            std::size_t index) const noexcept
        {
            assert(index < 4U);
            return RowVector(
                ComponentAt(Rows_[0], index), ComponentAt(Rows_[1], index),
                ComponentAt(Rows_[2], index), ComponentAt(Rows_[3], index));
        }

        [[nodiscard]] constexpr T Element(std::size_t row,
                                          std::size_t column) const noexcept
        {
            return ComponentAt(Row(row), column);
        }

        [[nodiscard]] RowVector TransformHomogeneous(
            const RowVector& vector) const noexcept
        {
            using Ops = detail::Matrix4Ops<T>;
            if constexpr (Ops::HasNative)
            {
                return RowVector::FromNative(
                    Ops::Transform(vector.Components_, Rows_[0].Components_,
                                   Rows_[1].Components_, Rows_[2].Components_,
                                   Rows_[3].Components_));
            }

            return Rows_[0] * vector.X() + Rows_[1] * vector.Y() +
                   Rows_[2] * vector.Z() + Rows_[3] * vector.W();
        }

        [[nodiscard]] Mat4 operator*(const Mat4& right) const noexcept
        {
            using Ops = detail::Matrix4Ops<T>;
            if constexpr (Ops::HasNative)
            {
                return FromNativeRows(Ops::Multiply(
                    Rows_[0].Components_, Rows_[1].Components_,
                    Rows_[2].Components_, Rows_[3].Components_,
                    right.Rows_[0].Components_, right.Rows_[1].Components_,
                    right.Rows_[2].Components_, right.Rows_[3].Components_));
            }

            return Mat4(right.TransformHomogeneous(Rows_[0]),
                        right.TransformHomogeneous(Rows_[1]),
                        right.TransformHomogeneous(Rows_[2]),
                        right.TransformHomogeneous(Rows_[3]));
        }

        [[nodiscard]] Mat4 Transposed() const noexcept
        {
            using Ops = detail::Matrix4Ops<T>;
            if constexpr (Ops::HasNative)
            {
                return FromNativeRows(
                    Ops::Transpose(Rows_[0].Components_, Rows_[1].Components_,
                                   Rows_[2].Components_, Rows_[3].Components_));
            }
            return Mat4(Column(0U), Column(1U), Column(2U), Column(3U));
        }

        [[nodiscard]] T Determinant() const noexcept
        {
            using Ops = detail::Matrix4Ops<T>;
            if constexpr (Ops::HasNative)
            {
                return Ops::Determinant(
                    Rows_[0].Components_, Rows_[1].Components_,
                    Rows_[2].Components_, Rows_[3].Components_);
            }

            return Element(0U, 0U) * MinorDeterminant(0U, 0U) -
                   Element(0U, 1U) * MinorDeterminant(0U, 1U) +
                   Element(0U, 2U) * MinorDeterminant(0U, 2U) -
                   Element(0U, 3U) * MinorDeterminant(0U, 3U);
        }

        [[nodiscard]] std::optional<Mat4> TryInverse() const noexcept
        {
            using Ops = detail::Matrix4Ops<T>;
            if constexpr (Ops::HasNative)
            {
                const typename Ops::Rows inverse = Ops::InverseUnchecked(
                    Rows_[0].Components_, Rows_[1].Components_,
                    Rows_[2].Components_, Rows_[3].Components_);
                if (!Ops::IsFinite(inverse))
                {
                    return std::nullopt;
                }
                return FromNativeRows(inverse);
            }

            const T determinant = Determinant();
            if (determinant == T(0) || !std::isfinite(determinant))
            {
                return std::nullopt;
            }

            // Adjugate/cofactor identity independently expressed and
            // cross-checked against RTM 2.3.1 matrix_inverse (MIT, commit
            // 745bd25673d93b46941eda55e0993327dbc12b53b).
            const T reciprocalDeterminant = T(1) / determinant;
            T values[16]{};
            bool finite = true;
            for (std::size_t row = 0U; row < 4U; ++row)
            {
                for (std::size_t column = 0U; column < 4U; ++column)
                {
                    const T sign = ((row + column) & 1U) == 0U ? T(1) : T(-1);
                    const T value = sign * MinorDeterminant(column, row) *
                                    reciprocalDeterminant;
                    values[row * 4U + column] = value;
                    finite &= std::isfinite(value);
                }
            }
            if (!finite)
            {
                return std::nullopt;
            }
            return Mat4(
                RowVector(values[0], values[1], values[2], values[3]),
                RowVector(values[4], values[5], values[6], values[7]),
                RowVector(values[8], values[9], values[10], values[11]),
                RowVector(values[12], values[13], values[14], values[15]));
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            bool finite = true;
            for (std::size_t row = 0U; row < 4U; ++row)
            {
                for (std::size_t column = 0U; column < 4U; ++column)
                {
                    finite &= std::isfinite(Element(row, column));
                }
            }
            return finite;
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Mat4&, const Mat4&) noexcept = default;

    private:
        [[nodiscard]] static constexpr T ComponentAt(const RowVector& value,
                                                     std::size_t index) noexcept
        {
            assert(index < 4U);
            if (index == 0U)
            {
                return value.X();
            }
            if (index == 1U)
            {
                return value.Y();
            }
            if (index == 2U)
            {
                return value.Z();
            }
            return value.W();
        }

        [[nodiscard]] T MinorDeterminant(
            std::size_t removedRow, std::size_t removedColumn) const noexcept
        {
            T minor[9]{};
            std::size_t destination = 0U;
            for (std::size_t row = 0U; row < 4U; ++row)
            {
                if (row == removedRow)
                {
                    continue;
                }
                for (std::size_t column = 0U; column < 4U; ++column)
                {
                    if (column != removedColumn)
                    {
                        minor[destination++] = Element(row, column);
                    }
                }
            }
            return minor[0] * (minor[4] * minor[8] - minor[5] * minor[7]) -
                   minor[1] * (minor[3] * minor[8] - minor[5] * minor[6]) +
                   minor[2] * (minor[3] * minor[7] - minor[4] * minor[6]);
        }

        template <typename Ops = detail::Matrix4Ops<T>>
        [[nodiscard]] static Mat4 FromNativeRows(
            const typename Ops::Rows& rows) noexcept
            requires Ops::HasNative
        {
            return Mat4(RowVector::FromNative(rows.Row0),
                        RowVector::FromNative(rows.Row1),
                        RowVector::FromNative(rows.Row2),
                        RowVector::FromNative(rows.Row3));
        }

        RowVector Rows_[4];
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec4<T> operator*(const Vec4<T>& vector,
                                    const Mat4<T>& matrix) noexcept
    {
        return matrix.TransformHomogeneous(vector);
    }

    using Mat4f = Mat4<float>;
    using Mat4d = Mat4<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Mat4f) == 64);
static_assert(alignof(mv::math::Mat4f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Mat4f>);
static_assert(std::is_standard_layout_v<mv::math::Mat4f>);
static_assert(sizeof(mv::math::Mat4d) == 128);
static_assert(alignof(mv::math::Mat4d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Mat4d>);
static_assert(std::is_standard_layout_v<mv::math::Mat4d>);
