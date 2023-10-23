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

    template <typename value_type, typename vector_type_raw>
    struct generic_vec3
    {
    public:
        using component_type = value_type;
        using vector_type = typename vector_type_raw::type;
        using underlying_type = vector_type;
        constexpr static uint32_t num_elements = 3;

        RTM_FORCE_INLINE generic_vec3() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        RTM_FORCE_INLINE generic_vec3(
            value_type x, value_type y = 0, value_type z = 0) noexcept
            : _value(rtm::vector_set(x, y, z, 0))
        {
        }

        RTM_FORCE_INLINE generic_vec3(const vector_type& v) noexcept : _value(v)
        {
        }

    public:
        RTM_FORCE_INLINE vector_type get_internal() const noexcept
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

        RTM_FORCE_INLINE bool operator==(const generic_vec3& v) const noexcept
        {
            return rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        RTM_FORCE_INLINE bool operator!=(const generic_vec3& v) const noexcept
        {
            return !rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3& operator=(const generic_vec3& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator+=(
            const generic_vec3& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator-=(
            const generic_vec3& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator*=(value_type v) noexcept
        {
            _value = rtm::vector_mul(_value, v);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator*=(
            const generic_vec3& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator/=(value_type v) noexcept
        {
            _value = rtm::vector_div(_value, v);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& operator/=(
            const generic_vec3& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3 operator+(
            const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_add(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3 operator-(
            const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_sub(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3 operator*(value_type v) const noexcept
        {
            return generic_vec3(rtm::vector_mul(_value, v));
        }

        RTM_FORCE_INLINE generic_vec3 operator*(
            const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_mul(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3 operator/(value_type v) const noexcept
        {
            return generic_vec3(rtm::vector_div(_value, v));
        }

        RTM_FORCE_INLINE generic_vec3 operator/(
            const generic_vec3& v) const noexcept
        {
            return generic_vec3(rtm::vector_div(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3 operator-() const noexcept
        {
            return generic_vec3(rtm::vector_neg(_value));
        }

        RTM_FORCE_INLINE value_type get_component(int index) const noexcept
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
            }
        }

        RTM_FORCE_INLINE value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

    public:
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

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& x(value_type x) noexcept
        {
            return set_x(x);
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& y(value_type y) noexcept
        {
            return set_y(y);
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& z(value_type z) noexcept
        {
            return set_z(z);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        RTM_FORCE_INLINE generic_vec3& fill(component_type value) noexcept
        {
            // TODO: Test to see if it's faster to do a single set(value, value,
            // value, 0) vs the set(value) + set_w(0)
            _value = rtm::vector_set(value);
            _value = rtm::vector_set_w(_value, 0);
            return *this;
        }

        /**
         * @brief Sets the x, y and z components of the vector
         *
         * @param x The new x component
         * @param y The new y component
         * @param z The new z component
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& set(
            value_type x, value_type y, value_type z) noexcept
        {
            _value = rtm::vector_set(x, y, z, 0);
            return *this;
        }

        /**
         * @brief Sets the x, y and z components of the vector
         *
         * @param v The new x, y and z components
         * @return generic_vec3& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3& set(const vector_type& v) noexcept
        {
            _value = v;
            return *this;
        }

        /**
         * @brief Returns the length of the vector
         *
         * @return value_type The length of the vector
         */
        RTM_FORCE_INLINE value_type length() const noexcept
        {
            return rtm::vector_length3(_value);
        }

        /**
         * @brief Returns the approximate length of the vector
         * @note This currently uses the same implementation as length(), but
         * the intention is to eventually have a faster approximation
         *
         * @return value_type The approximate length of the vector
         */
        RTM_FORCE_INLINE value_type length_approximate() const noexcept
        {
            // TODO: Find a better approximation?
            return rtm::vector_length3(_value);
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        RTM_FORCE_INLINE value_type squared_length() const noexcept
        {
            return rtm::vector_length_squared3(_value);
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        RTM_FORCE_INLINE value_type reciprocal_length() const noexcept
        {
            return rtm::vector_length_reciprocal3(_value);
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec3 The normalized vector
         */
        RTM_FORCE_INLINE generic_vec3 normalized() const noexcept
        {
            return rtm::vector_mul(_value, value_type(1) / length());
        }

        /**
         * @brief Returns an approximate normalized copy of the vector
         * @note This currently uses the same implementation as normalized(),
         * but the intention is to eventually have a faster approximation
         *
         * @return generic_vec3 The approximate normalized vector
         */
        RTM_FORCE_INLINE generic_vec3 normalized_approximate() const noexcept
        {
            return rtm::vector_mul(
                _value, value_type(1) / length_approximate());
        }

        /**
         * @brief Returns the dot product of the vector and another vector
         *
         * @param v The other vector
         * @return value_type The dot product of the two vectors
         */
        RTM_FORCE_INLINE value_type dot(const generic_vec3& v) const noexcept
        {
            return rtm::vector_dot3(_value, v._value);
        }

        /**
         * @brief Returns the euclidian distance between the vector and another
         * vector, treating them as points in 3d space.
         *
         * @param v The other vector
         * @return value_type The distance between the two vectors
         */
        RTM_FORCE_INLINE value_type distance_to_point(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).length();
        }

        /**
         * @brief Returns the approximate euclidian distance between the vector
         * and another vector, treating them as points in 3d space.
         * @note This currently uses the same implementation as
         * distance_to_point(), but the intention is to eventually have a faster
         * approximation
         *
         * @param v The other vector
         * @return value_type The approximate distance between the two vectors
         */
        RTM_FORCE_INLINE value_type distance_to_point_approximate(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        /**
         * @brief Returns the squared euclidian distance between the vector and
         * another vector, treating them as points in 3d space.
         *
         * @param v The other vector
         * @return value_type The squared distance between the two vectors
         */
        RTM_FORCE_INLINE value_type squared_distance_to_point(
            const generic_vec3& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        /**
         * @brief Calculates the distance between a point and a line defined by
         * two points.
         *
         * @param v0 The first point on the line
         * @param v1 The second point on the line
         * @return value_type The distance between the point and the line
         */
        RTM_FORCE_INLINE float distance_to_line(
            const generic_vec3& v0, const generic_vec3& v1) const noexcept
        {
            // Implementation based on XMVector3LinePointDistance
            using namespace rtm;
            const vector_type& point = _value;
            const vector_type& line_point1 = v0._value;
            const vector_type& line_point2 = v1._value;

            const auto point_vector = vector_sub(point, line_point1);
            const auto line_vector = vector_sub(line_point2, line_point1);
            const auto length_sq = vector_length_squared3(line_vector);
            const auto point_projection_scale =
                vector_div(vector_dot3(point_vector, line_vector), length_sq);
            const auto distance_vector = vector_sub(
                point_vector, vector_mul(line_vector, point_projection_scale));
            return vector_length3(distance_vector);
        }

        /**
         * @brief Calculates the cross product of the vector and another vector
         *
         * @param v2 The other vector
         * @return generic_vec3 The cross product of the two vectors
         */
        RTM_FORCE_INLINE generic_vec3 cross(
            const generic_vec3& v2) const noexcept
        {
            return rtm::vector_cross3(_value, v2._value);
        }

        RTM_FORCE_INLINE value_type angle_between_normalized_vectors(
            const generic_vec3& v) const noexcept
        {
            return std::acos(rtm::vector_dot3(_value, v._value));
        }

        RTM_FORCE_INLINE value_type angle_between_vectors(
            const generic_vec3& v) const noexcept
        {
            // Compute with dot product
            auto norm = normalized();
            auto vnorm = v.normalized();
            return norm.angle_between_normalized_vectors(vnorm);
        }

        RTM_FORCE_INLINE generic_vec3 reflect(
            const generic_vec3& normal) const noexcept
        {
            // Based on XMVector3Reflect
            using namespace rtm;
            const vector_type& incident = _value;
            const vector_type& nrm = normal._value;

            auto dot = vector_dot(incident, nrm);
            auto dot2 = vector_add(dot, dot);
            auto mul = vector_mul(nrm, dot2);
            auto res = vector_sub(incident, mul);
            return res;
        }

        RTM_FORCE_INLINE generic_vec3 refract(
            const generic_vec3& normal, value_type ior) const noexcept
        {
            // Based on XMVector3Refract
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

        RTM_FORCE_INLINE generic_vec3 min(const generic_vec3& v) const noexcept
        {
            using namespace rtm;
            return generic_vec3(vector_min(_value, v._value));
        }

        RTM_FORCE_INLINE generic_vec3 max(const generic_vec3& v) const noexcept
        {
            using namespace rtm;
            return generic_vec3(vector_max(_value, v._value));
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
        RTM_FORCE_INLINE static generic_vec3 zero() noexcept
        {
            return generic_vec3(0, 0, 0);
        }

        RTM_FORCE_INLINE static generic_vec3 one() noexcept
        {
            return generic_vec3(1, 1, 1);
        }

        RTM_FORCE_INLINE static generic_vec3 x_axis() noexcept
        {
            return generic_vec3(1, 0, 0);
        }

        RTM_FORCE_INLINE static generic_vec3 y_axis() noexcept
        {
            return generic_vec3(0, 1, 0);
        }

        RTM_FORCE_INLINE static generic_vec3 z_axis() noexcept
        {
            return generic_vec3(0, 0, 1);
        }

        RTM_FORCE_INLINE static generic_vec3 right() noexcept
        {
            return x_axis();
        }

        RTM_FORCE_INLINE static generic_vec3 up() noexcept
        {
            return y_axis();
        }

        RTM_FORCE_INLINE static generic_vec3 forward() noexcept
        {
            return z_axis();
        }

    private:
        vector_type _value;
    };

    using vec3f = generic_vec3<float, wrappers::v4f>;
    using vec3d = generic_vec3<double, wrappers::v4d>;

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
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec3f);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec3d);
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