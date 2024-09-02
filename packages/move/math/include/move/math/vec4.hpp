#pragma once
#include <move/math/macros.hpp>
#include <move/math/scalar/base_vec4.hpp>

namespace move::math
{
    // No SIMD support for vec4 yet
    template <typename T, move::math::Acceleration Accel>
    using base_vec4_t = move::math::scalar::base_vec4<T>;

    template <typename T, move::math::Acceleration Accel>
    struct vec4 : public base_vec4_t<T, Accel>
    {
    public:
        using base_t = base_vec4_t<T, Accel>;
        using base_vec4_t<T, Accel>::base_vec4_t;
        using component_type = T;
        constexpr static auto acceleration = Accel;
        constexpr static bool has_fields = true;
        constexpr static bool has_pointer_semantics = true;

        // Constructors
    public:
        MVM_INLINE vec4() : base_t()
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        MVM_INLINE vec4(vec4<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z(), other.get_w())
        {
        }

        // implicit conversion from base_vec4_t
        MVM_INLINE vec4(base_t other) :
            base_t(other.get_x(), other.get_y(), other.get_z(), other.get_w())
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        // implicit conversion from base_vec4_t
        MVM_INLINE vec4(base_vec4_t<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z(), other.get_w())
        {
        }

        MVM_INLINE vec4(const T& x,
                        const T& y = 0,
                        const T& z = 0,
                        const T& w = 0) :
            base_t(x, y, z, w)
        {
        }

        MVM_INLINE vec4(const vec4& other) : base_t(other)
        {
        }

        MVM_INLINE vec4& operator=(const vec4& other)
        {
            base_t::operator=(other);
            return *this;
        }

        MVM_INLINE ~vec4()
        {
        }

    public:
        MVM_INLINE_NODISCARD base_t storage() const
        {
            return *this;
        }

        MVM_INLINE_NODISCARD T* to_array()
        {
            return base_t::data;
        }

        MVM_INLINE_NODISCARD const T* to_array() const
        {
            return base_t::data;
        }

        // Assignment operators
    public:
        MVM_INLINE vec4& operator+=(const vec4& other)
        {
            *this = *this + other;
            return *this;
        }

        MVM_INLINE vec4& operator-=(const vec4& other)
        {
            *this = *this - other;
            return *this;
        }

        MVM_INLINE vec4& operator*=(const vec4& other)
        {
            *this = *this * other;
            return *this;
        }

        MVM_INLINE vec4& operator/=(const vec4& other)
        {
            *this = *this / other;
            return *this;
        }

        MVM_INLINE vec4& operator+=(const T& scalar)
        {
            *this = *this + scalar;
            return *this;
        }

        MVM_INLINE vec4& operator-=(const T& scalar)
        {
            *this = *this - scalar;
            return *this;
        }

        MVM_INLINE vec4& operator*=(const T& scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        MVM_INLINE vec4& operator/=(const T& scalar)
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
}  // namespace move::math