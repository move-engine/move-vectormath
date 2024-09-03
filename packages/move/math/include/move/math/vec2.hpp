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
        std::conditional_t<vec2_acceleration<Accel> == Acceleration::SIMD,
                           scalar::base_vec2<T>,  // TODO: Replace with SIMD
                                                  // type
                           scalar::base_vec2<T>>;

    template <typename T, move::math::Acceleration Accel>
    struct vec2 : public base_vec2_t<T, Accel>
    {
    public:
        using base_t = base_vec2_t<T, Accel>;
        using base_vec2_t<T, Accel>::base_vec2_t;
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
        MVM_INLINE_NODISCARD base_t storage() const
        {
            return *this;
        }

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

    using float2 = vec2<float, Acceleration::Default>;
    using double2 = vec2<double, Acceleration::Default>;

    using long2 = vec2<int64_t, Acceleration::Default>;
    using ulong2 = vec2<uint64_t, Acceleration::Default>;

    using int2 = vec2<int32_t, Acceleration::Default>;
    using uint2 = vec2<uint32_t, Acceleration::Default>;

    using short2 = vec2<int16_t, Acceleration::Default>;
    using ushort2 = vec2<uint16_t, Acceleration::Default>;

    using sbyte2 = vec2<int8_t, Acceleration::Default>;
    using byte2 = vec2<int8_t, Acceleration::Default>;
}  // namespace move::math