#pragma once
#include <ostream>

#include "underlying_types.hpp"

#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/scalard.h>
#include <rtm/types.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include <fmt/format.h>

namespace move::vectormath
{
    // using value_type = float;
    // using vector_type = rtm::vector4f;

    template <typename value_type, typename vector_type>
    struct generic_vec3
    {
    public:
        using component_type = value_type;
        using underlying_type = vector_type;
        constexpr static uint32_t num_elements = 3;

        inline generic_vec3() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        inline generic_vec3(
            value_type x, value_type y = 0, value_type z = 0) noexcept
            : _value(rtm::vector_set(x, y, z, 0))
        {
        }

        inline generic_vec3(const vector_type& v) noexcept : _value(v)
        {
        }

    public:
        template <typename Archive>
        inline void serialize(Archive& ar)
        {
            for (uint32_t i = 0; i < num_elements; ++i)
            {
                /* If is reading */
                if constexpr (Archive::is_loading::value)
                {
                    value_type val;
                    ar(val);
                    set_component(i, val);
                }
                else
                {
                    ar(get_component(i));
                }
            }
        }

    public:
        inline operator vector_type&()
        {
            return _value;
        }

        inline bool operator==(const generic_vec3& v) const noexcept
        {
            return rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        inline bool operator!=(const generic_vec3& v) const noexcept
        {
            return !rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        inline generic_vec3& operator=(const generic_vec3& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline generic_vec3& operator+=(const generic_vec3& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        inline generic_vec3& operator-=(const generic_vec3& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        inline generic_vec3& operator*=(const generic_vec3& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        inline generic_vec3& operator/=(const generic_vec3& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        inline generic_vec3 operator+(const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_add(_value, v._value));
        }

        inline generic_vec3 operator-(const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_sub(_value, v._value));
        }

        inline generic_vec3 operator*(const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_mul(_value, v._value));
        }

        inline generic_vec3 operator/(const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_div(_value, v._value));
        }

        inline generic_vec3 operator-() const noexcept
        {
            return generic_vec3(rtm::vector_neg(_value));
        }

        inline value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return rtm::vector_get_x(_value);
                case 1:
                    return rtm::vector_get_y(_value);
                default:
                    return rtm::vector_get_z(_value);
            }
        }

        inline void set_component(int index, value_type value)
        {
            switch (index)
            {
                case 0:
                    set_x(value);
                    break;
                case 1:
                    set_y(value);
                    break;
                case 2:
                    set_z(value);
                    break;
            }
        }

        class component_accessor
        {
        public:
            inline component_accessor(generic_vec3& vec, int index) noexcept
                : _vec(vec), _index(index)
            {
            }

            inline operator value_type() const noexcept
            {
                return _vec.get_component(_index);
            }

            inline component_accessor& operator=(const value_type& v) noexcept
            {
                _vec.set_component(_index, v);
                return *this;
            }

        private:
            generic_vec3& _vec;
            int _index;
        };

        inline value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        inline component_accessor operator[](int i) noexcept
        {
            return component_accessor(*this, i);
        }

    public:
        inline component_accessor x() noexcept
        {
            return component_accessor(*this, 0);
        }

        inline component_accessor y() noexcept
        {
            return component_accessor(*this, 1);
        }

        inline component_accessor z() noexcept
        {
            return component_accessor(*this, 2);
        }

        inline value_type x() const noexcept
        {
            return rtm::vector_get_x(_value);
        }

        inline value_type y() const noexcept
        {
            return rtm::vector_get_y(_value);
        }

        inline value_type z() const noexcept
        {
            return rtm::vector_get_z(_value);
        }

        inline generic_vec3& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        inline generic_vec3& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        inline generic_vec3& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        inline value_type length() const noexcept
        {
            return rtm::vector_length3(_value);
        }

        inline value_type length_approximate() const noexcept
        {
            // TODO: Find a better approximation?
            return rtm::vector_length3(_value);
        }

        inline value_type squared_length() const noexcept
        {
            return rtm::vector_length_squared3(_value);
        }

        inline generic_vec3 normalized() const noexcept
        {
            return rtm::vector_mul(_value, value_type(1) / length());
        }

        inline generic_vec3 normalized_approximate() const noexcept
        {
            return rtm::vector_mul(
                _value, value_type(1) / length_approximate());
        }

        inline value_type dot(const generic_vec3& v) const noexcept
        {
            return rtm::vector_dot3(_value, v._value);
        }

        inline value_type distance_to_point(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).length();
        }

        inline value_type distance_to_point_approximate(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        inline value_type squared_distance_to_point(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        inline generic_vec3 cross(const generic_vec3& v2) const noexcept
        {
            return rtm::vector_cross3(_value, v2._value);
        }

        inline value_type angle_between_normalized_vectors(
            const generic_vec3& v) const noexcept
        {
            return std::acos(rtm::vector_dot3(_value, v._value));
        }

        inline value_type angle_between_vectors(
            const generic_vec3& v) const noexcept
        {
            // Compute with dot product
            auto norm = normalized();
            auto vnorm = v.normalized();
            return norm.angle_between_normalized_vectors(vnorm);
        }

        inline generic_vec3 reflect(const generic_vec3& normal) const noexcept
        {
            return *this - (normal * (2 * dot(normal)));
        }

        inline generic_vec3 refract(
            const generic_vec3& normal, value_type ior) const noexcept
        {
            using namespace rtm;
            const vector_type& incident = _value;
            const vector_type& nrm = normal._value;
            const vector_type& index = vector_set(ior);

            value_type dotinorm = vector_dot(incident, nrm);
            vector_type roiPlusDotinorm = vector_mul(index, dotinorm);
            vector_type innerSqrt = vector_set(
                scalar_sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
            ;

            return vector_sub(vector_mul(index, incident),
                vector_mul(nrm, vector_add(roiPlusDotinorm, innerSqrt)));
        }

        inline generic_vec3 min(const generic_vec3& v) const noexcept
        {
            using namespace rtm;
            return generic_vec3(vector_min(_value, v._value));
        }

        inline generic_vec3 max(const generic_vec3& v) const noexcept
        {
            using namespace rtm;
            return generic_vec3(vector_max(_value, v._value));
        }

    public:
        inline void normalize() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized();
        }

        inline void normalize_approximate() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized_approximate();
        }

    public:
        inline static generic_vec3 zero() noexcept
        {
            return generic_vec3(0, 0, 0);
        }

        inline static generic_vec3 one() noexcept
        {
            return generic_vec3(1, 1, 1);
        }

        inline static generic_vec3 x_axis() noexcept
        {
            return generic_vec3(1, 0, 0);
        }

        inline static generic_vec3 y_axis() noexcept
        {
            return generic_vec3(0, 1, 0);
        }

        inline static generic_vec3 z_axis() noexcept
        {
            return generic_vec3(0, 0, 1);
        }

        inline static generic_vec3 right() noexcept
        {
            return x_axis();
        }

        inline static generic_vec3 up() noexcept
        {
            return y_axis();
        }

        inline static generic_vec3 forward() noexcept
        {
            return z_axis();
        }

    private:
        vector_type _value;
    };

    using vec3f = generic_vec3<float, rtm::vector4f>;
    using vec3d = generic_vec3<double, rtm::vector4d>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec3 = vec3d;
#else
    using vec3 = vec3f;
#endif

    template <typename value_type, typename vector_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec3<value_type, vector_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathcereal.hpp"
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec3f);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec3d);
// MOVE_VECTORMATH_CEREAL_SERIALIZER(move::vectormath::vec3f);
// MOVE_VECTORMATH_CEREAL_SERIALIZER(move::vectormath::vec3d);
#endif

template <typename value_type, typename vector_type>
struct fmt::formatter<move::vectormath::generic_vec3<value_type, vector_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec3<value_type, vector_type> const& number,
        FormatContext& ctx)
    {
        return format_to(
            ctx.out(), "({}, {}, {})", number.x(), number.y(), number.z());
    }
};