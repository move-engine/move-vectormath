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

    namespace detail
    {
        template <typename T, typename = void>
        struct component_type
        {
            using type = T;
        };

        template <typename T>
        struct component_type<T, std::void_t<typename T::component_type>>
        {
            using type = typename T::component_type;
        };
    }  // namespace detail

    // Use a math type's declared component type, or the type itself for
    // scalars.
    template <typename T>
    using component_type_t = typename detail::component_type<T>::type;
}  // namespace move::math::traits
