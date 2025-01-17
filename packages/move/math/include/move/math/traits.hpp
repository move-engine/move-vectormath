#pragma once
#include <cstdint>
#include <type_traits>

namespace move::math::traits
{
    template <typename T>
    constexpr bool is_vector_type_v = false;

    template <typename T>
    constexpr bool is_matrix_type_v = false;

    template <typename T>
    constexpr uint32_t component_count_v = 1;

    // if this is a vector type, use T::component_type, otherwise use T
    template <typename T>
    using component_type_t =
        std::conditional_t<is_vector_type_v<T> || is_matrix_type_v<T>,
                           typename T::component_type,
                           T>;
}  // namespace move::math::traits