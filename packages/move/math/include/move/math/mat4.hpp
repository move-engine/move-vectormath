#pragma once

#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <move/math/common.hpp>
#include <move/math/vec4.hpp>
#include <type_traits>
#include "rtm/impl/matrix_common.h"

namespace move::math
{
    namespace simd::detail
    {
        template <typename T>
        requires std::is_floating_point_v<T>
        struct mat4_storage
        {
            using type = std::conditional_t<std::is_same_v<T, float>,
                                            rtm::matrix4x4f,
                                            rtm::matrix4x4d>;
            type data;

            MVM_INLINE mat4_storage() : data(rtm::matrix_identity())
            {
            }

            MVM_INLINE mat4_storage(const type& data) : data(data)
            {
            }

            MVM_INLINE mat4_storage(const mat4_storage& other) :
                data(other.data)
            {
            }

            MVM_INLINE mat4_storage(const T& _11,
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
                data(rtm::matrix_set(rtm::vector_set(_11, _12, _13, _14),
                                     rtm::vector_set(_21, _22, _23, _24),
                                     rtm::vector_set(_31, _32, _33, _34),
                                     rtm::vector_set(_41, _42, _43, _44)))
            {
            }

            MVM_INLINE mat4_storage& operator=(const mat4_storage& other)
            {
                data = other.data;
                return *this;
            }
        };
    }  // namespace simd::detail

    // mat4 always uses RTM under the hood
    template <typename T>
    requires std::is_floating_point_v<T>
    struct mat4
    {
    public:
        constexpr static auto acceleration = Acceleration::SIMD;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

        using storage_t = typename simd::detail::mat4_storage<T>;
        using underlying_storage_t = typename storage_t::type;

    public:
        // Constructors
        MVM_INLINE mat4()
        {
        }

        MVM_INLINE mat4(const underlying_storage_t& data) : data(data)
        {
        }

        MVM_INLINE mat4(const mat4& other) : data(other.data)
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
            data(_11,
                 _12,
                 _13,
                 _14,
                 _21,
                 _22,
                 _23,
                 _24,
                 _31,
                 _32,
                 _33,
                 _34,
                 _41,
                 _42,
                 _43,
                 _44)
        {
        }

        MVM_INLINE mat4& operator=(const mat4& other)
        {
            data = other.data;
            return *this;
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[16];
            store_array(data);
        }

        // Pointers
    public:
        MVM_INLINE void store_array(T* out)
        {
        }

    private:
        storage_t data;
    };

    using float4x4 = mat4<float>;
    using double4x4 = mat4<double>;
}  // namespace move::math