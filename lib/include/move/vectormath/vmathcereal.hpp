#pragma once
#include "vmatherr.hpp"

#define MOVE_VECTORMATH_CEREAL_SERIALIZER(T)               \
    namespace move::vectormath                             \
    {                                                      \
        template <typename Archive>                        \
        inline void serialize(Archive& archive, T& vec)    \
        {                                                  \
            for (uint32_t i = 0; i < T::num_elements; ++i) \
            {                                              \
                /* If is reading */                        \
                if constexpr (Archive::is_loading::value)  \
                {                                          \
                    typename T::component_type val;        \
                    archive(val);                          \
                    vec[i] = val;                          \
                }                                          \
                else                                       \
                {                                          \
                    archive(vec[i]);                       \
                }                                          \
            }                                              \
        }                                                  \
    }
