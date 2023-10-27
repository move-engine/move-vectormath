#pragma once
#include <nlohmann/json.hpp>

#include "vmatherr.hpp"

namespace move::vectormath
{
    template <typename T, size_t L>
    struct json_serializer_template
    {
        static inline void to_json(nlohmann::json& j, const T& data)
        {
            j = nlohmann::json::array();
            for (size_t i = 0; i < L; ++i)
            {
                j.push_back(data[i]);
            }
        }

        static inline void from_json(const nlohmann::json& j, T& data)
        {
            // MOVE_ERROR_IF(!j.is_array(),
            //     "Attempted to load a vector from a JSON object that was "
            //     "not an array");

            for (size_t i = 0; i < L; ++i)
            {
                typename T::component_type val;
                j.at(i).get_to(val);
                data.set_component(i, val);
            }
        }
    };
}  // namespace move::vectormath

#define MOVE_VECTORMATH_JSON_SERIALIZER(T)                         \
    namespace nlohmann                                             \
    {                                                              \
        template <>                                                \
        struct adl_serializer<T>                                   \
            : public move::vectormath::json_serializer_template<T, \
                  T::num_components>                               \
        {                                                          \
        };                                                         \
    }
