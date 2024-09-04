#pragma once
#include <cstdint>

#include <move/math/macros.hpp>
#include <move/math/rtm/base_vec3.hpp>
#include <move/math/scalar/base_vec3.hpp>
#include <move/math/vec2.hpp>

namespace move::math
{
    template <move::math::Acceleration Accel>
    static constexpr auto vec3_acceleration =
        Accel == Acceleration::Default ? Acceleration::RTM : Accel;

    template <typename T, move::math::Acceleration Accel>
    using base_vec3_t =
        std::conditional_t<vec3_acceleration<Accel> == Acceleration::RTM,
                           scalar::base_vec3<T>,  // TODO: Replace with SIMD
                                                  // type
                           scalar::base_vec3<T>>;

    template <typename T, move::math::Acceleration Accel>
    struct vec3 : public base_vec3_t<T, Accel>
    {
    public:
        using base_t = base_vec3_t<T, Accel>;
        using component_type = T;
        using rtm_t = simd_rtm::detail::v3<T>::type;

        constexpr static auto acceleration = base_t::acceleration;
        constexpr static bool has_fields = base_t::has_fields;
        constexpr static bool has_pointer_semantics =
            base_t::has_pointer_semantics;

        // Constructors
    public:
        MVM_INLINE vec3() : base_t()
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        MVM_INLINE vec3(vec3<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z())
        {
        }

        // implicit conversion from base_vec3_t
        MVM_INLINE vec3(base_t other) : base_t(other)
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        // implicit conversion from base_vec3_t
        MVM_INLINE vec3(base_vec3_t<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z())
        {
        }

        MVM_INLINE vec3(const T& x, const T& y = 0, const T& z = 0) :
            base_t(x, y, z)
        {
        }

        MVM_INLINE vec3(const vec3& other) : base_t(other)
        {
        }

        MVM_INLINE vec3& operator=(const vec3& other)
        {
            base_t::operator=(other);
            return *this;
        }

        // Vector length conversions
    public:
        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE vec3(const vec2<OtherT, OtherAccel>& vec, const T& z = 0) :
            base_t(vec.get_x(), vec.get_y(), z)
        {
        }

        // Swizzles
    public:
        using vec2_t = vec2<T, Accel>;
        MVM_INLINE_NODISCARD vec2_t xy() const
        {
            return vec2_t(base_t::get_x(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec2_t xz() const
        {
            return vec2_t(base_t::get_x(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t yx() const
        {
            return vec2_t(base_t::get_y(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t yz() const
        {
            return vec2_t(base_t::get_y(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t zx() const
        {
            return vec2_t(base_t::get_z(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t zy() const
        {
            return vec2_t(base_t::get_z(), base_t::get_y());
        }

        // Conversions
    public:
        MVM_INLINE_NODISCARD base_t storage() const
        {
            return *this;
        }

        MVM_INLINE_NODISCARD rtm_t to_rtm() const
        {
            // TODO: Fast mode for RTM vec3
            return rtm::vector_set(base_t::get_x(), base_t::get_y(),
                                   base_t::get_z(), 0);
        }

        MVM_INLINE_NODISCARD static vec3 from_rtm(const rtm_t& rtm_vec)
        {
            // TODO: Fast mode for RTM vec3
            return vec3(rtm::vector_get_x(rtm_vec), rtm::vector_get_y(rtm_vec),
                        rtm::vector_get_z(rtm_vec));
        }

        // Assignment operators
    public:
        MVM_INLINE vec3& operator+=(const vec3& other)
        {
            *this = *this + other;
            return *this;
        }

        MVM_INLINE vec3& operator-=(const vec3& other)
        {
            *this = *this - other;
            return *this;
        }

        MVM_INLINE vec3& operator*=(const vec3& other)
        {
            *this = *this * other;
            return *this;
        }

        MVM_INLINE vec3& operator/=(const vec3& other)
        {
            *this = *this / other;
            return *this;
        }

        MVM_INLINE vec3& operator+=(const T& scalar)
        {
            *this = *this + scalar;
            return *this;
        }

        MVM_INLINE vec3& operator-=(const T& scalar)
        {
            *this = *this - scalar;
            return *this;
        }

        MVM_INLINE vec3& operator*=(const T& scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        MVM_INLINE vec3& operator/=(const T& scalar)
        {
            *this = *this / scalar;
            return *this;
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            base_t::serialize(archive);
        }
    };

    using float3 = vec3<float, Acceleration::Default>;
    using double3 = vec3<double, Acceleration::Default>;

    using long3 = vec3<int64_t, Acceleration::Default>;
    using ulong3 = vec3<uint64_t, Acceleration::Default>;

    using int3 = vec3<int32_t, Acceleration::Default>;
    using uint3 = vec3<uint32_t, Acceleration::Default>;

    using short3 = vec3<int16_t, Acceleration::Default>;
    using ushort3 = vec3<uint16_t, Acceleration::Default>;

    using sbyte3 = vec3<int8_t, Acceleration::Default>;
    using byte3 = vec3<uint8_t, Acceleration::Default>;
}  // namespace move::math