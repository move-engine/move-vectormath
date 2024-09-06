#pragma once
#include <cstdint>

#include <move/math/common.hpp>
#include <move/math/scalar/base_vec2.hpp>

namespace move::math
{
    template <move::math::Acceleration Accel>
    static constexpr auto vec2_acceleration =
        Accel == Acceleration::Default ? Acceleration::Scalar : Accel;

    template <typename T, move::math::Acceleration Accel>
    using base_vec2_t =
        std::conditional_t<vec2_acceleration<Accel> == Acceleration::RTM,
                           scalar::base_vec2<T>,  // TODO: Replace with RTM
                                                  // type
                           scalar::base_vec2<T>>;

    template <typename T, move::math::Acceleration Accel>
    struct vec2 : public base_vec2_t<T, Accel>
    {
    public:
        using base_t = base_vec2_t<T, Accel>;
        using component_type = T;

        constexpr static auto acceleration = base_t::acceleration;
        constexpr static bool has_fields = base_t::has_fields;
        constexpr static bool has_pointer_semantics =
            base_t::has_pointer_semantics;

        // Constructors
    public:
        MVM_INLINE vec2() : base_t()
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        MVM_INLINE vec2(vec2<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y())
        {
        }

        // implicit conversion from base_vec2_t
        MVM_INLINE vec2(base_t other) : base_t(other.get_x(), other.get_y())
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        // implicit conversion from base_vec2_t
        MVM_INLINE vec2(base_vec2_t<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y())
        {
        }

        MVM_INLINE vec2(const T& x, const T& y) : base_t(x, y)
        {
        }

        MVM_INLINE vec2(const vec2& other) : base_t(other)
        {
        }

        MVM_INLINE vec2& operator=(const vec2& other)
        {
            base_t::operator=(other);
            return *this;
        }

        // Conversions
    public:
        // Assignment operators
    public:
        MVM_INLINE vec2& operator+=(const vec2& other)
        {
            *this = *this + other;
            return *this;
        }

        MVM_INLINE vec2& operator-=(const vec2& other)
        {
            *this = *this - other;
            return *this;
        }

        MVM_INLINE vec2& operator*=(const vec2& other)
        {
            *this = *this * other;
            return *this;
        }

        MVM_INLINE vec2& operator/=(const vec2& other)
        {
            *this = *this / other;
            return *this;
        }

        MVM_INLINE vec2& operator+=(const T& scalar)
        {
            *this = *this + scalar;
            return *this;
        }

        MVM_INLINE vec2& operator-=(const T& scalar)
        {
            *this = *this - scalar;
            return *this;
        }

        MVM_INLINE vec2& operator*=(const T& scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        MVM_INLINE vec2& operator/=(const T& scalar)
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

    using fast_float2 = vec2<float, Acceleration::RTM>;
    using fast_double2 = vec2<double, Acceleration::RTM>;
    using storage_float2 = vec2<float, Acceleration::Default>;
    using storage_double2 = vec2<double, Acceleration::Default>;

    using fast_long2 = vec2<int64_t, Acceleration::Default>;
    using fast_ulong2 = vec2<uint64_t, Acceleration::Default>;
    using storage_long2 = vec2<int64_t, Acceleration::Default>;
    using storage_ulong2 = vec2<uint64_t, Acceleration::Default>;

    using fast_int2 = vec2<int32_t, Acceleration::Default>;
    using fast_uint2 = vec2<uint32_t, Acceleration::Default>;
    using storage_int2 = vec2<int32_t, Acceleration::Default>;
    using storage_uint2 = vec2<uint32_t, Acceleration::Default>;

    using fast_short2 = vec2<int16_t, Acceleration::Default>;
    using fast_ushort2 = vec2<uint16_t, Acceleration::Default>;
    using storage_short2 = vec2<int16_t, Acceleration::Default>;
    using storage_ushort2 = vec2<uint16_t, Acceleration::Default>;

    using fast_sbyte2 = vec2<int8_t, Acceleration::Default>;
    using fast_byte2 = vec2<int8_t, Acceleration::Default>;
    using storage_sbyte2 = vec2<int8_t, Acceleration::Default>;
    using storage_byte2 = vec2<int8_t, Acceleration::Default>;

    using float2 = fast_float2;
    using double2 = fast_double2;

    using long2 = storage_long2;
    using ulong2 = storage_ulong2;

    using int2 = storage_int2;
    using uint2 = storage_uint2;

    using short2 = storage_short2;
    using ushort2 = storage_ushort2;

    using sbyte2 = storage_sbyte2;
    using byte2 = storage_byte2;

    using vec2f = storage_float2;
    using vec2d = storage_double2;
    using vec2i = storage_int2;
    using vec2u = storage_uint2;
    using vec2l = storage_long2;
    using vec2ul = storage_ulong2;
    using vec2s = storage_short2;
    using vec2us = storage_ushort2;

    using vec2b = storage_sbyte2;
    using vec2sb = storage_byte2;
}  // namespace move::math