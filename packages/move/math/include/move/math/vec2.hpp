#pragma once
#include <cstdint>

#include <move/math/common.hpp>
#include <move/math/scalar/base_vec2.hpp>
#include <move/math/traits.hpp>

namespace move::math
{
    // Vec2 remains scalar-only until a real RTM backend exists.
    template <typename T, move::math::Acceleration Accel>
    using base_vec2_t = scalar::base_vec2<T>;

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
        constexpr static uint32_t element_count = base_t::element_count;

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

        // Public operations return the public wrapper, never the backend type.
    public:
        MVM_INLINE_NODISCARD static vec2 from_array(const T* src)
        {
            return vec2(base_t::from_array(src));
        }

        MVM_INLINE vec2& load_array(const T* src)
        {
            base_t::load_array(src);
            return *this;
        }

        MVM_INLINE_NODISCARD vec2 operator+(const vec2& other) const
        {
            return vec2(base_t::operator+(other));
        }

        MVM_INLINE_NODISCARD vec2 operator-(const vec2& other) const
        {
            return vec2(base_t::operator-(other));
        }

        MVM_INLINE_NODISCARD vec2 operator*(const vec2& other) const
        {
            return vec2(base_t::operator*(other));
        }

        MVM_INLINE_NODISCARD vec2 operator/(const vec2& other) const
        {
            return vec2(base_t::operator/(other));
        }

        MVM_INLINE_NODISCARD vec2 operator+(const T& scalar) const
        {
            return vec2(base_t::operator+(scalar));
        }

        MVM_INLINE_NODISCARD vec2 operator-(const T& scalar) const
        {
            return vec2(base_t::operator-(scalar));
        }

        MVM_INLINE_NODISCARD vec2 operator*(const T& scalar) const
        {
            return vec2(base_t::operator*(scalar));
        }

        MVM_INLINE_NODISCARD vec2 operator/(const T& scalar) const
        {
            return vec2(base_t::operator/(scalar));
        }

        MVM_INLINE_NODISCARD vec2 operator-() const
        {
            return vec2(base_t::operator-());
        }

        MVM_INLINE_NODISCARD vec2 normalized() const
            requires std::is_floating_point_v<T>
        {
            return vec2(base_t::normalized());
        }

        MVM_INLINE vec2& normalize()
            requires std::is_floating_point_v<T>
        {
            base_t::normalize();
            return *this;
        }

        MVM_INLINE vec2& fill(const T& value)
        {
            base_t::fill(value);
            return *this;
        }

        MVM_INLINE vec2& set(const T& x, const T& y)
        {
            base_t::set(x, y);
            return *this;
        }

        MVM_INLINE vec2& set_zero()
        {
            base_t::set_zero();
            return *this;
        }

        // Assignment operators
        MVM_INLINE vec2& operator+=(const vec2& other)
        {
            base_t::operator+=(other);
            return *this;
        }

        MVM_INLINE vec2& operator-=(const vec2& other)
        {
            base_t::operator-=(other);
            return *this;
        }

        MVM_INLINE vec2& operator*=(const vec2& other)
        {
            base_t::operator*=(other);
            return *this;
        }

        MVM_INLINE vec2& operator/=(const vec2& other)
        {
            base_t::operator/=(other);
            return *this;
        }

        MVM_INLINE vec2& operator+=(const T& scalar)
        {
            base_t::operator+=(scalar);
            return *this;
        }

        MVM_INLINE vec2& operator-=(const T& scalar)
        {
            base_t::operator-=(scalar);
            return *this;
        }

        MVM_INLINE vec2& operator*=(const T& scalar)
        {
            base_t::operator*=(scalar);
            return *this;
        }

        MVM_INLINE vec2& operator/=(const T& scalar)
        {
            base_t::operator/=(scalar);
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

    using fast_float2 = vec2<float, Acceleration::Scalar>;
    using fast_double2 = vec2<double, Acceleration::Scalar>;
    using storage_float2 = vec2<float, Acceleration::Scalar>;
    using storage_double2 = vec2<double, Acceleration::Scalar>;

    using fast_long2 = vec2<int64_t, Acceleration::Default>;
    using fast_ulong2 = vec2<uint64_t, Acceleration::Default>;
    using storage_long2 = vec2<int64_t, Acceleration::Scalar>;
    using storage_ulong2 = vec2<uint64_t, Acceleration::Scalar>;

    using fast_int2 = vec2<int32_t, Acceleration::Default>;
    using fast_uint2 = vec2<uint32_t, Acceleration::Default>;
    using storage_int2 = vec2<int32_t, Acceleration::Scalar>;
    using storage_uint2 = vec2<uint32_t, Acceleration::Scalar>;

    using fast_short2 = vec2<int16_t, Acceleration::Default>;
    using fast_ushort2 = vec2<uint16_t, Acceleration::Default>;
    using storage_short2 = vec2<int16_t, Acceleration::Scalar>;
    using storage_ushort2 = vec2<uint16_t, Acceleration::Scalar>;

    using fast_sbyte2 = vec2<int8_t, Acceleration::Default>;
    using fast_byte2 = vec2<uint8_t, Acceleration::Default>;
    using storage_sbyte2 = vec2<int8_t, Acceleration::Scalar>;
    using storage_byte2 = vec2<uint8_t, Acceleration::Scalar>;

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

    using vec2b = storage_byte2;
    using vec2sb = storage_sbyte2;

    template <typename T, move::math::Acceleration Accel>
    MVM_INLINE_NODISCARD bool approx_equal(
        const vec2<T, Accel>& a,
        const vec2<T, Accel>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        return approx_equal(a.get_x(), b.get_x(), epsilon) &&
               approx_equal(a.get_y(), b.get_y(), epsilon);
    }

    namespace traits
    {
        template <typename T, Acceleration Accel>
        constexpr bool is_vector_type_v<vec2<T, Accel>> = true;

        template <typename T, Acceleration Accel>
        constexpr uint32_t component_count_v<vec2<T, Accel>> = 2;
    }  // namespace traits
}  // namespace move::math
