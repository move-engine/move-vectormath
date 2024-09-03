#pragma once

#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <move/math/common.hpp>
#include <move/math/vec4.hpp>
#include <type_traits>
#include "rtm/impl/matrix_common.h"
#include "rtm/types.h"

namespace move::math
{
    namespace simd::detail
    {
        MVM_TYPE_WRAPPER(m4x4f, rtm::matrix4x4f)
        MVM_TYPE_WRAPPER(m4x4d, rtm::matrix4x4d)

        template <typename T>
        using m4x4 = std::conditional_t<std::is_same_v<T, float>, m4x4f, m4x4d>;

        // template <typename T>
        // requires std::is_floating_point_v<T>
        // struct mat4_storage
        // {
        //     using type = std::conditional_t<std::is_same_v<T, float>,
        //                                     rtm::matrix4x4f,
        //                                     rtm::matrix4x4d>;
        //     type data;

        //     MVM_INLINE mat4_storage() : data(rtm::matrix_identity())
        //     {
        //     }

        //     MVM_INLINE mat4_storage(const type& data) : data(data)
        //     {
        //     }

        //     MVM_INLINE mat4_storage(const mat4_storage& other) :
        //         data(other.data)
        //     {
        //     }

        //     MVM_INLINE mat4_storage(const T& _11,
        //                             const T& _12,
        //                             const T& _13,
        //                             const T& _14,
        //                             const T& _21,
        //                             const T& _22,
        //                             const T& _23,
        //                             const T& _24,
        //                             const T& _31,
        //                             const T& _32,
        //                             const T& _33,
        //                             const T& _34,
        //                             const T& _41,
        //                             const T& _42,
        //                             const T& _43,
        //                             const T& _44) :
        //         data(rtm::matrix_set(rtm::vector_set(_11, _12, _13, _14),
        //                              rtm::vector_set(_21, _22, _23, _24),
        //                              rtm::vector_set(_31, _32, _33, _34),
        //                              rtm::vector_set(_41, _42, _43, _44)))
        //     {
        //     }

        //     MVM_INLINE mat4_storage& operator=(const mat4_storage& other)
        //     {
        //         data = other.data;
        //         return *this;
        //     }
        // };
    }  // namespace simd::detail

    // mat4 always uses RTM under the hood
    template <typename T, typename wrapper_type = simd::detail::m4x4<T>>
    requires std::is_floating_point_v<T>
    struct mat4
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

    private:
        using underlying_type = wrapper_type::type;
        underlying_type _value;

    public:
        // Constructors
        MVM_INLINE mat4() : _value(rtm::matrix_identity())
        {
        }

        MVM_INLINE mat4(const underlying_type& data) : _value(data)
        {
        }

        MVM_INLINE mat4(const mat4& other) : _value(other._value)
        {
        }

        MVM_INLINE mat4(const T& _11,
                        const T& _12,
                        const T& _13,
                        const T& _14,
                        const T& _21,
                        const T& _22,
                        const T& _23,
                        const T& _24,
                        const T& _31,
                        const T& _32,
                        const T& _33,
                        const T& _34,
                        const T& _41,
                        const T& _42,
                        const T& _43,
                        const T& _44) :
            _value(rtm::matrix_set(rtm::vector_set(_11, _12, _13, _14),
                                   rtm::vector_set(_21, _22, _23, _24),
                                   rtm::vector_set(_31, _32, _33, _34),
                                   rtm::vector_set(_41, _42, _43, _44)))
        {
        }

        MVM_INLINE mat4& operator=(const mat4& other)
        {
            _value = other._value;
            return *this;
        }

        // Pointers
    public:
        MVM_INLINE void store_array(T* out)
        {
            using namespace rtm;
            for (uint8_t i = 0; i < 4; i++)
            {
                vector_store(matrix_get_axis(_value, (axis4)i), out + i * 4);
            }
        }

        MVM_INLINE void load_array(const T* in)
        {
            using namespace rtm;
            vector4f x = vector_load(in);
            vector4f y = vector_load(in + 4);
            vector4f z = vector_load(in + 8);
            vector4f w = vector_load(in + 12);
            _value = matrix_set(x, y, z, w);
        }

        // Arithmetic operations
    public:
        MVM_INLINE_NODISCARD mat4 operator*(const mat4& other) const
        {
            return mat4(rtm::matrix_mul(_value, other._value));
        }

        using compatible_vec4 = vec4<T, acceleration>;

        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE_NODISCARD vec4<OtherT, OtherAccel> operator*(
            const vec4<OtherT, OtherAccel>& other) const
        {
            return vec4(rtm::matrix_mul(_value, other.storage()));
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[16];
            if constexpr (Archive::is_loading::value)
            {
                archive(data);
                load_array(data);
            }
            else
            {
                store_array(data);
                archive(data);
            }
        }
    };

    using float4x4 = mat4<float>;
    using double4x4 = mat4<double>;
}  // namespace move::math