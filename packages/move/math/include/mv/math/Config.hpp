#pragma once

#include <cstdint>

namespace mv::math
{
    enum class BackendKind : std::uint8_t
    {
        Scalar,
        Rtm
    };

#if defined(MV_MATH_FORCE_SCALAR)
    inline constexpr BackendKind SelectedBackend = BackendKind::Scalar;
#else
    inline constexpr BackendKind SelectedBackend = BackendKind::Rtm;
#endif
}  // namespace mv::math
