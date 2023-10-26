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
    template <typename value_type, typename vector_type_raw>
    struct generic_vec4
    {
    public:
        using component_type = value_type;
        using vector_type = typename vector_type_raw::type;
        using underlying_type = vector_type;
        constexpr static uint32_t num_elements = 4;

        RTM_FORCE_INLINE generic_vec4() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        RTM_FORCE_INLINE generic_vec4(value_type x, value_type y = 0,
            value_type z = 0, value_type w = 0) noexcept
            : _value(rtm::vector_set(x, y, z, w))
        {
        }

        RTM_FORCE_INLINE generic_vec4(const vector_type& v) noexcept : _value(v)
        {
        }

    public:
        RTM_FORCE_INLINE vector_type get_internal() const noexcept
        {
            return _value;
        }

        RTM_FORCE_INLINE vector_type& get_internal() noexcept
        {
            return _value;
        }

    public:
        template <typename Archive>
        RTM_FORCE_INLINE void serialize(Archive& ar)
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
        RTM_FORCE_INLINE operator vector_type&()
        {
            return _value;
        }

        RTM_FORCE_INLINE operator const vector_type&() const
        {
            return _value;
        }

        RTM_FORCE_INLINE bool operator==(const generic_vec4& v) const noexcept
        {
            return rtm::mask_all_true(rtm::vector_equal(_value, v._value));
        }

        RTM_FORCE_INLINE bool operator!=(const generic_vec4& v) const noexcept
        {
            return !rtm::mask_all_true(rtm::vector_equal(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4& operator=(const generic_vec4& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator+=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator-=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator*=(value_type v) noexcept
        {
            _value = rtm::vector_mul(_value, v);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator*=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator/=(value_type v) noexcept
        {
            _value = rtm::vector_div(_value, rtm::vector_set(v));
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& operator/=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4 operator+(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_add(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4 operator-(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_sub(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4 operator*(value_type v) const noexcept
        {
            return generic_vec4(rtm::vector_mul(_value, v));
        }

        RTM_FORCE_INLINE generic_vec4 operator*(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_mul(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4 operator/(value_type v) const noexcept
        {
            return generic_vec4(rtm::vector_div(_value, rtm::vector_set(v)));
        }

        RTM_FORCE_INLINE generic_vec4 operator/(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_div(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4 operator-() const noexcept
        {
            return generic_vec4(rtm::vector_neg(_value));
        }

    public:
        RTM_FORCE_INLINE value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return rtm::vector_get_x(_value);
                case 1:
                    return rtm::vector_get_y(_value);
                case 2:
                    return rtm::vector_get_z(_value);
                default:
                    return rtm::vector_get_w(_value);
            }
        }

        RTM_FORCE_INLINE void set_component(int index, value_type value)
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
                default:
                    set_w(value);
                    break;
            }
        }

        class component_accessor
        {
        public:
            RTM_FORCE_INLINE component_accessor(
                generic_vec4& vec, int index) noexcept
                : _vec(vec), _index(index)
            {
            }

            RTM_FORCE_INLINE operator value_type() const noexcept
            {
                return _vec.get_component(_index);
            }

            RTM_FORCE_INLINE component_accessor& operator=(
                const value_type& v) noexcept
            {
                _vec.set_component(_index, v);
                return *this;
            }

        private:
            generic_vec4& _vec;
            int _index;
        };

        RTM_FORCE_INLINE value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        RTM_FORCE_INLINE component_accessor operator[](int i) noexcept
        {
            return component_accessor(*this, i);
        }

        RTM_FORCE_INLINE component_accessor x() noexcept
        {
            return component_accessor(*this, 0);
        }

        RTM_FORCE_INLINE component_accessor y() noexcept
        {
            return component_accessor(*this, 1);
        }

        RTM_FORCE_INLINE component_accessor z() noexcept
        {
            return component_accessor(*this, 2);
        }

        RTM_FORCE_INLINE component_accessor w() noexcept
        {
            return component_accessor(*this, 3);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& x(value_type value) noexcept
        {
            set_x(value);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& y(value_type value) noexcept
        {
            set_y(value);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& z(value_type value) noexcept
        {
            set_z(value);
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& w(value_type value) noexcept
        {
            set_w(value);
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& fill(component_type value) noexcept
        {
            _value = rtm::vector_set(value);
            return *this;
        }

        /**
         * @brief Sets the components of the vector
         *
         * @param x The new x component
         * @param y The new y component
         * @param z The new z component
         * @param w The new w component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set(component_type x, component_type y,
            component_type z, component_type w) noexcept
        {
            _value = rtm::vector_set(x, y, z, w);
            return *this;
        }

        /**
         * @brief Sets the components of the vector
         *
         * @param v The new vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set(const generic_vec4& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        RTM_FORCE_INLINE value_type x() const noexcept
        {
            return rtm::vector_get_x(_value);
        }

        RTM_FORCE_INLINE value_type y() const noexcept
        {
            return rtm::vector_get_y(_value);
        }

        RTM_FORCE_INLINE value_type z() const noexcept
        {
            return rtm::vector_get_z(_value);
        }

        RTM_FORCE_INLINE value_type w() const noexcept
        {
            return rtm::vector_get_w(_value);
        }

        RTM_FORCE_INLINE generic_vec4& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec4& set_w(value_type w) noexcept
        {
            _value = rtm::vector_set_w(_value, w);
            return *this;
        }

    public:
        RTM_FORCE_INLINE value_type length() const noexcept
        {
            return rtm::vector_length(_value);
        }

        RTM_FORCE_INLINE value_type length_approximate() const noexcept
        {
            // TODO: Find a better approximation?
            return rtm::vector_length(_value);
        }

        RTM_FORCE_INLINE value_type squared_length() const noexcept
        {
            return rtm::vector_length_squared(_value);
        }

        RTM_FORCE_INLINE generic_vec4 normalized() const noexcept
        {
            return rtm::vector_mul(_value, value_type(1) / length());
        }

        RTM_FORCE_INLINE generic_vec4 normalized_approximate() const noexcept
        {
            return rtm::vector_mul(
                _value, value_type(1) / length_approximate());
        }

        RTM_FORCE_INLINE value_type dot(const generic_vec4& v) const noexcept
        {
            return rtm::vector_dot(_value, v._value);
        }

        RTM_FORCE_INLINE value_type distance_to_point(
            const generic_vec4& v) const noexcept
        {
            return (v - *this).length();
        }

        RTM_FORCE_INLINE value_type distance_to_point_approximate(
            const generic_vec4& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        RTM_FORCE_INLINE value_type squared_distance_to_point(
            const generic_vec4& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        RTM_FORCE_INLINE generic_vec4 cross(
            const generic_vec4& v2, const generic_vec4& v3) const noexcept
        {
            // TODO: Write a faster implementation, because this is terrible
            const auto& v1 = *this;
            return {
                (((v2.z() * v3.w()) - (v2.w() * v3.z())) * v1.y()) -
                    (((v2.y() * v3.w()) - (v2.w() * v3.y())) * v1.z()) +
                    (((v2.y() * v3.z()) - (v2.z() * v3.y())) * v1.w()),
                (((v2.w() * v3.z()) - (v2.z() * v3.w())) * v1.x()) -
                    (((v2.w() * v3.x()) - (v2.x() * v3.w())) * v1.z()) +
                    (((v2.z() * v3.x()) - (v2.x() * v3.z())) * v1.w()),
                (((v2.y() * v3.w()) - (v2.w() * v3.y())) * v1.x()) -
                    (((v2.x() * v3.w()) - (v2.w() * v3.x())) * v1.y()) +
                    (((v2.x() * v3.y()) - (v2.y() * v3.x())) * v1.w()),
                (((v2.z() * v3.y()) - (v2.y() * v3.z())) * v1.x()) -
                    (((v2.z() * v3.x()) - (v2.x() * v3.z())) * v1.y()) +
                    (((v2.y() * v3.x()) - (v2.x() * v3.y())) * v1.z()),
            };
        }

        RTM_FORCE_INLINE value_type angle_between_normalized_vectors(
            const generic_vec4& v) const noexcept
        {
            return std::acos(rtm::vector_dot(_value, v._value));
        }

        RTM_FORCE_INLINE value_type angle_between_vectors(
            const generic_vec4& v) const noexcept
        {
            // Compute with dot product
            auto norm = normalized();
            auto vnorm = v.normalized();
            return norm.angle_between_normalized_vectors(vnorm);
        }

        RTM_FORCE_INLINE generic_vec4 reflect(
            const generic_vec4& normal) const noexcept
        {
            using namespace rtm;
            const vector_type& incident = _value;
            const vector_type& nrm = normal._value;

            auto dot = vector_dot(incident, nrm);
            auto dot2 = vector_add(dot, dot);
            auto mul = vector_mul(nrm, dot2);
            auto res = vector_sub(incident, mul);
            return res;
        }

        RTM_FORCE_INLINE generic_vec4 refract(
            const generic_vec4& normal, value_type ior) const noexcept
        {
            using namespace rtm;
            const vector_type& incident = _value;
            const vector_type& nrm = normal._value;
            const vector_type& index = vector_set(ior);

            // clang-format off
            // Algorithm:
            // Result = RefractionIndex * Incident - Normal * (RefractionIndex * dot(Incident, Normal) + sqrt(1 - RefractionIndex * RefractionIndex * (1 - dot(Incident, Normal) * dot(Incident, Normal))))
            // 
            // Expanded:
            // dotinorm = dot(Incident, Normal);
            // roiPlusDotinorm = RefractionIndex * dotinorm
            // innerSqrt = sqrt(1 - RefractionIndex * RefractionIndex * (1 -
            //             dotinorm * dotinorm))
            //
            // Result = RefractionIndex * Incident - Normal * (
            //     roiPlusDotinorm +
            //     innerSqrt
            // )
            //
            // clang-format on

            value_type dotinorm = vector_dot(incident, nrm);
            vector_type roiPlusDotinorm = vector_mul(index, dotinorm);
            vector_type innerSqrt = vector_set(
                scalar_sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
            ;

            return vector_sub(vector_mul(index, incident),
                vector_mul(nrm, vector_add(roiPlusDotinorm, innerSqrt)));
        }

        RTM_FORCE_INLINE generic_vec4 min(const generic_vec4& v) const noexcept
        {
            using namespace rtm;
            return generic_vec4(vector_min(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec4 max(const generic_vec4& v) const noexcept
        {
            using namespace rtm;
            return generic_vec4(vector_max(_value, v._value));
        }

    public:
        RTM_FORCE_INLINE void normalize() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized();
        }

        RTM_FORCE_INLINE void normalize_approximate() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized_approximate();
        }

    public:
        RTM_FORCE_INLINE static generic_vec4 zero() noexcept
        {
            return generic_vec4(0, 0, 0, 0);
        }

        RTM_FORCE_INLINE static generic_vec4 one() noexcept
        {
            return generic_vec4(1, 1, 1, 1);
        }

        RTM_FORCE_INLINE static generic_vec4 x_axis() noexcept
        {
            return generic_vec4(1, 0, 0);
        }

        RTM_FORCE_INLINE static generic_vec4 y_axis() noexcept
        {
            return generic_vec4(0, 1, 0);
        }

        RTM_FORCE_INLINE static generic_vec4 z_axis() noexcept
        {
            return generic_vec4(0, 0, 1);
        }

        RTM_FORCE_INLINE static generic_vec4 w_axis() noexcept
        {
            return generic_vec4(0, 0, 0, 1);
        }

        RTM_FORCE_INLINE static generic_vec4 right() noexcept
        {
            return x_axis();
        }

        RTM_FORCE_INLINE static generic_vec4 up() noexcept
        {
            return y_axis();
        }

        RTM_FORCE_INLINE static generic_vec4 forward() noexcept
        {
            return z_axis();
        }

    private:
        vector_type _value;
    };

    using vec4f = generic_vec4<float, wrappers::v4f>;
    using vec4d = generic_vec4<double, wrappers::v4d>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec4 = vec4d;
#else
    using vec4 = vec4f;
#endif

    template <typename value_type, typename vector_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec4<value_type, vector_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", "
                  << v.w() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec4f);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec4d);
#endif

template <typename value_type, typename vector_type>
struct fmt::formatter<move::vectormath::generic_vec4<value_type, vector_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec4<value_type, vector_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {}, {}, {})", number.x(), number.y(),
            number.z(), number.w());
    }
};