#pragma once

#include <move/math/common.hpp>
#include <move/math/vec4.hpp>

namespace move::math
{
    // mat4 always uses RTM under the hood
    template <typename T>
    requires std::is_floating_point_v<T>
    struct mat4
    {
    public:
        constexpr static auto acceleration = Acceleration::SIMD;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

    public:
        // Constructors
    };

    using float4x4 = mat4<float>;
    using double4x4 = mat4<double>;
}  // namespace move::math