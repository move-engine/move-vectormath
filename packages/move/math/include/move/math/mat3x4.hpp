#pragma once
#include <type_traits>

#include <rtm/impl/matrix_common.h>
#include <rtm/matrix3x4d.h>
#include <rtm/matrix3x4f.h>
#include <rtm/types.h>

#include <move/math/rtm/rtm_common.hpp>

namespace move::math
{
    namespace simd_rtm::detail
    {
        MVM_TYPE_WRAPPER(m3x4f, rtm::matrix3x4f)
        MVM_TYPE_WRAPPER(m3x4d, rtm::matrix3x4d)

        template <typename T>
        using m3x4 = std::conditional_t<std::is_same_v<T, float>, m3x4f, m3x4d>;
    }  // namespace simd_rtm::detail
}  // namespace move::math