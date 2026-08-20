#pragma once

#include <memory>

#include <mv/math/Config.hpp>
#include <mv/math/detail/VectorOps.hpp>

#if !defined(MV_MATH_FORCE_SCALAR)
#include <rtm/matrix4x4f.h>
#endif

namespace mv::math::detail
{
    template <typename T>
    struct Matrix4Rows
    {
        using Native = typename SelectedVectorOps<T>::Native;

        Native Row0;
        Native Row1;
        Native Row2;
        Native Row3;
    };

    template <typename T>
    struct Matrix4Ops
    {
        inline static constexpr bool HasNative = false;
    };

#if !defined(MV_MATH_FORCE_SCALAR)
    template <>
    struct Matrix4Ops<float>
    {
        inline static constexpr bool HasNative = true;
        using Rows = Matrix4Rows<float>;
        using Native = typename Rows::Native;

        [[nodiscard]] static rtm::matrix4x4f Load(const float* row0,
                                                  const float* row1,
                                                  const float* row2,
                                                  const float* row3) noexcept
        {
            return {rtm::vector_load(row0), rtm::vector_load(row1),
                    rtm::vector_load(row2), rtm::vector_load(row3)};
        }

        [[nodiscard]] static Rows StoreRows(
            const rtm::matrix4x4f& matrix) noexcept
        {
            return {matrix.x_axis, matrix.y_axis, matrix.z_axis, matrix.w_axis};
        }

        [[nodiscard]] static Native Transform(const float* vector,
                                              const float* row0,
                                              const float* row1,
                                              const float* row2,
                                              const float* row3) noexcept
        {
            // Expanded directly from RTM 2.3.1 matrix_mul_vector (MIT,
            // commit 745bd25673d93b46941eda55e0993327dbc12b53b). Keeping the
            // row loads adjacent to their arithmetic lets compilers fold them
            // into memory operands instead of first materializing a matrix.
            vector = std::assume_aligned<16>(vector);
            row0 = std::assume_aligned<16>(row0);
            row1 = std::assume_aligned<16>(row1);
            row2 = std::assume_aligned<16>(row2);
            row3 = std::assume_aligned<16>(row3);
            const Native input = rtm::vector_load(vector);
            Native result = rtm::vector_mul(rtm::vector_dup_x(input),
                                            rtm::vector_load(row0));
            result = rtm::vector_mul_add(rtm::vector_dup_y(input),
                                         rtm::vector_load(row1), result);
            result = rtm::vector_mul_add(rtm::vector_dup_z(input),
                                         rtm::vector_load(row2), result);
            return rtm::vector_mul_add(rtm::vector_dup_w(input),
                                       rtm::vector_load(row3), result);
        }

        [[nodiscard]] static Rows Multiply(const float* left0,
                                           const float* left1,
                                           const float* left2,
                                           const float* left3,
                                           const float* right0,
                                           const float* right1,
                                           const float* right2,
                                           const float* right3) noexcept
        {
            // Direct delegation to RTM 2.3.1 matrix_mul (MIT, same commit).
            return StoreRows(
                rtm::matrix_mul(Load(left0, left1, left2, left3),
                                Load(right0, right1, right2, right3)));
        }

        [[nodiscard]] static Rows Transpose(const float* row0,
                                            const float* row1,
                                            const float* row2,
                                            const float* row3) noexcept
        {
            return StoreRows(
                rtm::matrix_transpose(Load(row0, row1, row2, row3)));
        }

        [[nodiscard]] static float Determinant(const float* row0,
                                               const float* row1,
                                               const float* row2,
                                               const float* row3) noexcept
        {
            return rtm::scalar_cast(
                rtm::matrix_determinant(Load(row0, row1, row2, row3)));
        }

        [[nodiscard]] static Rows InverseUnchecked(const float* row0,
                                                   const float* row1,
                                                   const float* row2,
                                                   const float* row3) noexcept
        {
            // Direct delegation to RTM 2.3.1 matrix_inverse (MIT, same
            // commit); the facade validates every returned lane.
            return StoreRows(rtm::matrix_inverse(Load(row0, row1, row2, row3)));
        }

        [[nodiscard]] static bool IsFinite(const Rows& rows) noexcept
        {
            return rtm::vector_is_finite(rows.Row0) &&
                   rtm::vector_is_finite(rows.Row1) &&
                   rtm::vector_is_finite(rows.Row2) &&
                   rtm::vector_is_finite(rows.Row3);
        }
    };
#endif
}  // namespace mv::math::detail
