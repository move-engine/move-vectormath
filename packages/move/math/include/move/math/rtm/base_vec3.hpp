#pragma once
#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_common.hpp>
#include <move/meta/type_utils.hpp>

namespace move::math::simd_rtm
{
    namespace detail
    {
        MVM_TYPE_WRAPPER(v3f, rtm::vector4f)
        MVM_TYPE_WRAPPER(v3d, rtm::vector4d)

        template <typename T>
        using v3 = std::conditional_t<std::is_same_v<T, float>, v3f, v3d>;
    }  // namespace detail
}  // namespace move::math::simd_rtm