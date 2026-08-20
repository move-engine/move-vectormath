#pragma once

#include <cassert>
#include <cmath>
#include <cstddef>
#include <optional>
#include <type_traits>

#include <mv/math/Rotation3.hpp>
#include <mv/math/Vec3.hpp>
#include <mv/math/detail/Matrix3Ops.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Mat3
    {
    public:
        using Component = T;
        using RowVector = Vec3<T>;

        Mat3() noexcept :
            Rows_{RowVector(T(1), T(0), T(0)), RowVector(T(0), T(1), T(0)),
                  RowVector(T(0), T(0), T(1))}
        {
        }

        Mat3(RowVector row0, RowVector row1, RowVector row2) noexcept :
            Rows_{row0, row1, row2}
        {
        }

        Mat3(T m11,
             T m12,
             T m13,
             T m21,
             T m22,
             T m23,
             T m31,
             T m32,
             T m33) noexcept :
            Mat3(RowVector(m11, m12, m13),
                 RowVector(m21, m22, m23),
                 RowVector(m31, m32, m33))
        {
        }

        [[nodiscard]] static Mat3 Identity() noexcept
        {
            return Mat3();
        }

        [[nodiscard]] static Mat3 Zero() noexcept
        {
            return Mat3(RowVector(), RowVector(), RowVector());
        }

        [[nodiscard]] static Mat3 Scale(RowVector scale) noexcept
        {
            return Mat3(scale.X(), T(0), T(0), T(0), scale.Y(), T(0), T(0),
                        T(0), scale.Z());
        }

        [[nodiscard]] static Mat3 FromRotation(
            const Rotation3f& rotation) noexcept
            requires std::is_same_v<T, float>
        {
            const Quatf& quaternion = rotation.Quaternion();
            using Ops = detail::Matrix3Ops<T>;
            if constexpr (Ops::HasNativeFromQuaternion)
            {
                const typename Ops::Rows rows =
                    Ops::FromQuaternion(quaternion.Components_);
                return Mat3(RowVector(rows.Row0), RowVector(rows.Row1),
                            RowVector(rows.Row2));
            }

            const float x = quaternion.X();
            const float y = quaternion.Y();
            const float z = quaternion.Z();
            const float w = quaternion.W();
            const float xx = x * x;
            const float yy = y * y;
            const float zz = z * z;
            const float xy = x * y;
            const float xz = x * z;
            const float yz = y * z;
            const float xw = x * w;
            const float yw = y * w;
            const float zw = z * w;

            // Quaternion-to-matrix equation follows Szeliski,
            // MSR-TR-2004-92 eq. 22, transposed for Move's row vectors;
            // cross-checked against RTM 2.3.1 matrix_from_quat (MIT).
            return Mat3(
                1.0F - 2.0F * (yy + zz), 2.0F * (xy + zw), 2.0F * (xz - yw),
                2.0F * (xy - zw), 1.0F - 2.0F * (xx + zz), 2.0F * (yz + xw),
                2.0F * (xz + yw), 2.0F * (yz - xw), 1.0F - 2.0F * (xx + yy));
        }

        [[nodiscard]] const RowVector& Row(std::size_t index) const noexcept
        {
            assert(index < 3U);
            return Rows_[index];
        }

        [[nodiscard]] RowVector Column(std::size_t index) const noexcept
        {
            assert(index < 3U);
            if (index == 0U)
            {
                return RowVector(Rows_[0].X(), Rows_[1].X(), Rows_[2].X());
            }
            if (index == 1U)
            {
                return RowVector(Rows_[0].Y(), Rows_[1].Y(), Rows_[2].Y());
            }
            return RowVector(Rows_[0].Z(), Rows_[1].Z(), Rows_[2].Z());
        }

        [[nodiscard]] T Element(std::size_t row,
                                std::size_t column) const noexcept
        {
            const RowVector& value = Row(row);
            assert(column < 3U);
            if (column == 0U)
            {
                return value.X();
            }
            if (column == 1U)
            {
                return value.Y();
            }
            return value.Z();
        }

        [[nodiscard]] Mat3 Transposed() const noexcept
        {
            return Mat3(Column(0U), Column(1U), Column(2U));
        }

        [[nodiscard]] T Determinant() const noexcept
        {
            const T a = Rows_[0].X();
            const T b = Rows_[0].Y();
            const T c = Rows_[0].Z();
            const T d = Rows_[1].X();
            const T e = Rows_[1].Y();
            const T f = Rows_[1].Z();
            const T g = Rows_[2].X();
            const T h = Rows_[2].Y();
            const T i = Rows_[2].Z();
            return a * (e * i - f * h) - b * (d * i - f * g) +
                   c * (d * h - e * g);
        }

        [[nodiscard]] std::optional<Mat3> TryInverse() const noexcept
        {
            using Ops = detail::Matrix3Ops<T>;
            if constexpr (Ops::HasNativeInverse)
            {
                // RTM's unchecked inverse produces non-finite lanes for an
                // exact singular input; validating its result preserves this
                // API's failure contract without computing the determinant a
                // second time in the facade.
                const typename Ops::Rows inverse = Ops::InverseUnchecked(
                    Rows_[0].Native_, Rows_[1].Native_, Rows_[2].Native_);
                if (!Ops::IsFinite(inverse))
                {
                    return std::nullopt;
                }
                return Mat3(RowVector(inverse.Row0), RowVector(inverse.Row1),
                            RowVector(inverse.Row2));
            }

            const T a = Rows_[0].X();
            const T b = Rows_[0].Y();
            const T c = Rows_[0].Z();
            const T d = Rows_[1].X();
            const T e = Rows_[1].Y();
            const T f = Rows_[1].Z();
            const T g = Rows_[2].X();
            const T h = Rows_[2].Y();
            const T i = Rows_[2].Z();

            const T cofactor00 = e * i - f * h;
            const T cofactor01 = f * g - d * i;
            const T cofactor02 = d * h - e * g;
            const T determinant =
                a * cofactor00 + b * cofactor01 + c * cofactor02;
            if (determinant == T(0) || !std::isfinite(determinant))
            {
                return std::nullopt;
            }

            // Adjugate/cofactor formula independently derived and
            // cross-checked against RTM 2.3.1 matrix_inverse (MIT, commit
            // 745bd25673d93b46941eda55e0993327dbc12b53b).
            const T reciprocalDeterminant = T(1) / determinant;
            const T inverse00 = cofactor00 * reciprocalDeterminant;
            const T inverse01 = (c * h - b * i) * reciprocalDeterminant;
            const T inverse02 = (b * f - c * e) * reciprocalDeterminant;
            const T inverse10 = cofactor01 * reciprocalDeterminant;
            const T inverse11 = (a * i - c * g) * reciprocalDeterminant;
            const T inverse12 = (c * d - a * f) * reciprocalDeterminant;
            const T inverse20 = cofactor02 * reciprocalDeterminant;
            const T inverse21 = (b * g - a * h) * reciprocalDeterminant;
            const T inverse22 = (a * e - b * d) * reciprocalDeterminant;

            const bool finite =
                std::isfinite(inverse00) && std::isfinite(inverse01) &&
                std::isfinite(inverse02) && std::isfinite(inverse10) &&
                std::isfinite(inverse11) && std::isfinite(inverse12) &&
                std::isfinite(inverse20) && std::isfinite(inverse21) &&
                std::isfinite(inverse22);
            if (!finite)
            {
                return std::nullopt;
            }

            return Mat3(inverse00, inverse01, inverse02, inverse10, inverse11,
                        inverse12, inverse20, inverse21, inverse22);
        }

        [[nodiscard]] bool IsFinite() const noexcept
        {
            // Bitwise conjunction keeps this validation branchless on common
            // optimizing compilers while still checking every component.
            return std::isfinite(Rows_[0].X()) && std::isfinite(Rows_[0].Y()) &&
                   std::isfinite(Rows_[0].Z()) && std::isfinite(Rows_[1].X()) &&
                   std::isfinite(Rows_[1].Y()) && std::isfinite(Rows_[1].Z()) &&
                   std::isfinite(Rows_[2].X()) && std::isfinite(Rows_[2].Y()) &&
                   std::isfinite(Rows_[2].Z());
        }

        [[nodiscard]] RowVector TransformVector(
            const RowVector& vector) const noexcept
        {
            return Rows_[0] * vector.X() + Rows_[1] * vector.Y() +
                   Rows_[2] * vector.Z();
        }

        [[nodiscard]] Mat3 operator*(const Mat3& right) const noexcept
        {
            // Row-vector composition matches RTM 2.3.1 matrix_mul (MIT,
            // commit 745bd25673d93b46941eda55e0993327dbc12b53b).
            return Mat3(right.TransformVector(Rows_[0]),
                        right.TransformVector(Rows_[1]),
                        right.TransformVector(Rows_[2]));
        }

        [[nodiscard]] friend bool operator==(const Mat3& left,
                                             const Mat3& right) noexcept
        {
            return left.Rows_[0] == right.Rows_[0] &&
                   left.Rows_[1] == right.Rows_[1] &&
                   left.Rows_[2] == right.Rows_[2];
        }

    private:
        RowVector Rows_[3];
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Vec3<T> operator*(const Vec3<T>& vector,
                                    const Mat3<T>& matrix) noexcept
    {
        // Public value * matrix spelling intentionally preserves Move's
        // row-vector convention and RTM's matrix_mul_vector3 semantics.
        return matrix.TransformVector(vector);
    }

    using Mat3f = Mat3<float>;
    using Mat3d = Mat3<double>;
}  // namespace mv::math

static_assert(sizeof(mv::math::Mat3f) == 48);
static_assert(alignof(mv::math::Mat3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Mat3f>);
static_assert(std::is_standard_layout_v<mv::math::Mat3f>);
