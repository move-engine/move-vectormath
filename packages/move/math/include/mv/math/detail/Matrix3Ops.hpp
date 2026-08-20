#pragma once

#include <type_traits>

#include <mv/math/Config.hpp>
#include <mv/math/detail/VectorOps.hpp>

#if !defined(MV_MATH_FORCE_SCALAR)
#include <rtm/matrix3x3f.h>
#endif

namespace mv::math::detail
{
    template <typename T>
    struct Matrix3Rows
    {
        using Native = typename SelectedVectorOps<T>::Native;

        Native Row0;
        Native Row1;
        Native Row2;
    };

    template <typename T>
    struct Matrix3Ops
    {
        inline static constexpr bool HasNativeInverse = false;
        inline static constexpr bool HasNativeFromQuaternion = false;
    };

#if !defined(MV_MATH_FORCE_SCALAR)
    template <>
    struct Matrix3Ops<float>
    {
        inline static constexpr bool HasNativeInverse = true;
        inline static constexpr bool HasNativeFromQuaternion = true;
        using Rows = Matrix3Rows<float>;
        using Native = typename Rows::Native;

        [[nodiscard]] static Rows FromQuaternion(
            const float* components) noexcept
        {
            // Direct delegation to RTM 2.3.1 matrix_from_quat (MIT,
            // commit 745bd25673d93b46941eda55e0993327dbc12b53b).
            const rtm::matrix3x3f matrix =
                rtm::matrix_from_quat(rtm::quat_load(components));
            return {rtm::matrix_get_axis(matrix, rtm::axis3::x),
                    rtm::matrix_get_axis(matrix, rtm::axis3::y),
                    rtm::matrix_get_axis(matrix, rtm::axis3::z)};
        }

        [[nodiscard]] static Rows InverseUnchecked(Native row0,
                                                   Native row1,
                                                   Native row2) noexcept
        {
            // Direct delegation to RTM 2.3.1 matrix_inverse (MIT, commit
            // 745bd25673d93b46941eda55e0993327dbc12b53b).
            const rtm::matrix3x3f matrix = rtm::matrix_set(row0, row1, row2);
            const rtm::matrix3x3f inverse = rtm::matrix_inverse(matrix);
            return {rtm::matrix_get_axis(inverse, rtm::axis3::x),
                    rtm::matrix_get_axis(inverse, rtm::axis3::y),
                    rtm::matrix_get_axis(inverse, rtm::axis3::z)};
        }

        [[nodiscard]] static bool IsFinite(const Rows& rows) noexcept
        {
            return rtm::vector_is_finite3(rows.Row0) &&
                   rtm::vector_is_finite3(rows.Row1) &&
                   rtm::vector_is_finite3(rows.Row2);
        }
    };
#endif
}  // namespace mv::math::detail
