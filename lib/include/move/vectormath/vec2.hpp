#pragma once
#include <ostream>

#include <rtm/scalard.h>

namespace move::vectormath
{
    /**
     * @brief A two component vector that can store either floats or doubles.
     * The underlying implementation is scalar.
     * @note This type has no alignment requirements.
     *
     * @tparam value_type The type of the components of the vector
     */
    template <typename value_type>
    struct generic_vec2_scalar
    {
    public:
        /**
         * @brief The type of the components of the vector
         */
        using component_type = value_type;

        /**
         * @brief The number of components in the vector
         */
        constexpr static uint32_t num_components = 2;

    public:
        /**
         * @brief Construct a new generic_vec2_scalar object
         *
         * @return generic_vec2_scalar A new vector initialized to (0, 0)
         */
        RTM_FORCE_INLINE generic_vec2_scalar() noexcept : _x(0), _y(0)
        {
        }

        /**
         * @brief Construct a new generic_vec2_scalar object
         *
         * @return generic_vec2_scalar A new vector initialized to (x, y)
         */
        RTM_FORCE_INLINE generic_vec2_scalar(
            value_type x, value_type y = 0) noexcept
            : _x(x), _y(y)
        {
        }

    public:
        /**
         * @brief Serializes the vector to/from an archive.  Intended for use
         * with Cereal and Cereal-like APIs.
         *
         * @tparam Archive The archive type
         * @param ar The archive
         */
        template <typename Archive>
        RTM_FORCE_INLINE void serialize(Archive& ar)
        {
            for (uint32_t i = 0; i < num_components; ++i)
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
        /**
         * @brief Determines if the vector is equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are equal
         * @return false The vectors are not equal
         */
        RTM_FORCE_INLINE bool operator==(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x == v._x && _y == v._y;
        }

        /**
         * @brief Determines if the vector is not equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are not equal
         * @return false The vectors are equal
         */
        RTM_FORCE_INLINE bool operator!=(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x != v._x || _y != v._y;
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator=(
            const generic_vec2_scalar& v) noexcept
        {
            _x = v._x;
            _y = v._y;
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator+=(
            const generic_vec2_scalar& v) noexcept
        {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator-=(
            const generic_vec2_scalar& v) noexcept
        {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator*=(
            const value_type& v) noexcept
        {
            _x *= v;
            _y *= v;
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator*=(
            const generic_vec2_scalar& v) noexcept
        {
            _x *= v._x;
            _y *= v._y;
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator/=(
            const value_type& v) noexcept
        {
            _x /= v;
            _y /= v;
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& operator/=(
            const generic_vec2_scalar& v) noexcept
        {
            _x /= v._x;
            _y /= v._y;
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec2_scalar The result of the addition
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator+(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x + v._x, _y + v._y);
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec2_scalar The result of the subtraction
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator-(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x - v._x, _y - v._y);
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec2_scalar The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator*(
            const value_type& v) const noexcept
        {
            return generic_vec2_scalar(_x * v, _y * v);
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result. Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec2_scalar The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator*(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x * v._x, _y * v._y);
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec2_scalar The result of the division
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator/(
            const value_type& v) const noexcept
        {
            return generic_vec2_scalar(_x / v, _y / v);
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec2_scalar The result of the division
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator/(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x / v._x, _y / v._y);
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec2_scalar The negated vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar operator-() const noexcept
        {
            return generic_vec2_scalar(-_x, -_y);
        }

    public:
        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the y component is returned if the index is out of
         * range.
         *
         * @param index The index of the component to return
         * @return value_type The component at the specified index
         */
        RTM_FORCE_INLINE value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return _x;
                default:
                    return _y;
            }
        }

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the y component is set if the index is out of range.
         *
         * @param index The index of the component to set
         * @param value The new value for the component
         */
        RTM_FORCE_INLINE void set_component(int index, value_type value)
        {
            switch (index)
            {
                case 0:
                    set_x(value);
                    break;
                default:
                    set_y(value);
                    break;
            }
        }

        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * returned.
         *
         * @param index The index of the component to return
         * @return value_type The component at the specified index
         */
        RTM_FORCE_INLINE value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

    public:
        /**
         * @brief Returns the x component of the vector
         *
         * @return value_type The x component
         */
        RTM_FORCE_INLINE value_type x() const noexcept
        {
            return _x;
        }

        /**
         * @brief Returns the y component of the vector
         *
         * @return value_type The y component
         */
        RTM_FORCE_INLINE value_type y() const noexcept
        {
            return _y;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& set_x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& set_y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec2_scalar& A reference to the vector
         */

        RTM_FORCE_INLINE generic_vec2_scalar& fill(value_type v) noexcept
        {
            _x = v;
            _y = v;
            return *this;
        }

        /**
         * @brief Sets the x and y components of the vector
         *
         * @param x The new x component
         * @param y The new y component
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& set(
            value_type x, value_type y) noexcept
        {
            _x = x;
            _y = y;
            return *this;
        }

        /**
         * @brief Sets the x and y components of the vector
         *
         * @param v A vector containing the new x and y components
         * @return generic_vec2_scalar& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar& set(
            const generic_vec2_scalar& rhs) noexcept
        {
            _x = rhs._x;
            _y = rhs._y;
            return *this;
        }

    public:
        /**
         * @brief Returns the length of the vector
         *
         * @return value_type The length of the vector
         */
        RTM_FORCE_INLINE value_type length() const noexcept
        {
            return rtm::scalar_sqrt(squared_length());
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        RTM_FORCE_INLINE value_type squared_length() const noexcept
        {
            return _x * _x + _y * _y;
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        RTM_FORCE_INLINE value_type reciprocal_length() const noexcept
        {
            return rtm::scalar_sqrt_reciprocal(squared_length());
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec2_scalar The normalized vector
         */
        RTM_FORCE_INLINE generic_vec2_scalar normalized() const noexcept
        {
            return *this / length();
        }

    public:
        /**
         * @brief Returns the dot product of two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        RTM_FORCE_INLINE static value_type dot(const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return v1._x * v2._x + v1._y * v2._y;
        }

        /**
         * @brief Returns the euclidean distance between two vectors as if they
         * were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        RTM_FORCE_INLINE static value_type distance_between_points(
            const generic_vec2_scalar& point1,
            const generic_vec2_scalar& point2) noexcept
        {
            return (point2 - point1).length();
        }

        /**
         * @brief Returns the squared euclidean distance between two vectors as
         * if they were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        RTM_FORCE_INLINE static value_type squared_distance_between_points(
            const generic_vec2_scalar& point1,
            const generic_vec2_scalar& point2) noexcept
        {
            return (point2 - point1).squared_length();
        }

        /**
         * @brief Calculates the distance between a point and a line defined by
         * two points.
         *
         * @param point The point
         * @param v0 The first point on the line
         * @param v1 The second point on the line
         * @return value_type The distance between the point and the line
         */
        RTM_FORCE_INLINE static value_type distance_to_line(
            const generic_vec2_scalar& point, const generic_vec2_scalar& v0,
            const generic_vec2_scalar& v1) noexcept
        {
            // Adapted from DXM...
            // Given a vector PointVector from LinePoint1 to Point and a vector
            // LineVector from LinePoint1 to LinePoint2, the scaled distance
            // PointProjectionScale from LinePoint1 to the perpendicular
            // projection of PointVector onto the line is defined as:
            //
            //     PointProjectionScale = dot(PointVector, LineVector) /
            //     LengthSq(LineVector)

            auto point_vector = point - v0;
            auto line_vector = v1 - v0;

            auto length_sq = line_vector.squared_length();

            auto point_projection_scale = dot(point_vector, line_vector);
            point_projection_scale /= length_sq;

            auto distance_vector = line_vector * point_projection_scale;
            distance_vector = point_vector - distance_vector;

            return distance_vector.length();
        }

        /**
         * @brief Calculates the cross product of two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The cross product of the two vectors
         */
        RTM_FORCE_INLINE static value_type cross(const generic_vec2_scalar& lhs,
            const generic_vec2_scalar& rhs) noexcept
        {
            return lhs._x * rhs._y - lhs._y * rhs._x;
        }

        /**
         * @brief Returns the angle between two normalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_normalized_vectors(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            // Compute with dot product
            auto res = dot(v1, v2);
            return rtm::scalar_acos(res);
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_vectors(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            // Compute with dot product
            auto norm = v1.normalized();
            auto vnorm = v2.normalized();
            return angle_between_normalized_vectors(norm, vnorm);
        }

        /**
         * @brief Reflects incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar reflect(
            const generic_vec2_scalar& incident,
            const generic_vec2_scalar& normal) noexcept
        {
            return incident - (normal * (2 * dot(incident, normal)));
        }

        /**
         * @brief Refracts incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         * @param ior The index of refraction
         */
        RTM_FORCE_INLINE static generic_vec2_scalar refract(
            const generic_vec2_scalar& incident,
            const generic_vec2_scalar& normal, value_type ior) noexcept
        {
            // Based on XMVector2RefractV
            auto idotn = dot(incident, normal);

            auto ry = 1.0f - (idotn * idotn);
            auto rx = 1.0f - (ry * ior * ior);
            ry = 1.0f - (ry * ior * ior);

            if (rx >= 0.0f)
            {
                rx = (ior * incident.x()) -
                     (normal.x() * ((ior * idotn) + rtm::scalar_sqrt(rx)));
            }
            else
            {
                rx = 0.0f;
            }

            if (ry >= 0.0f)
            {
                ry = (ior * incident.y()) -
                     (normal.y() * ((ior * idotn) + rtm::scalar_sqrt(ry)));
            }
            else
            {
                ry = 0.0f;
            }

            return generic_vec2_scalar(rx, ry);
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec2_scalar The minimum vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar min(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return generic_vec2_scalar(
                rtm::scalar_min(v1._x, v2._x), rtm::scalar_min(v1._y, v2._y));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec2_scalar The maximum vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar max(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return generic_vec2_scalar(
                rtm::scalar_max(v1._x, v2._x), rtm::scalar_max(v1._y, v2._y));
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE void normalize() noexcept
        {
            *this /= length();
        }

    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to set all components to
         * @return generic_vec2_scalar The filled vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar filled(
            value_type value) noexcept
        {
            return generic_vec2_scalar(value, value);
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec2_scalar The infinity vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative infinity.
         *
         * @return generic_vec2_scalar The infinity vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar negative_infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec2_scalar The NaN vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec2_scalar The zero vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to negative one.
         *
         * @return generic_vec2_scalar The negative one vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar negative_one() noexcept
        {
            return filled(-1);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec2_scalar The one vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The x axis vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar x_axis() noexcept
        {
            return generic_vec2_scalar(1, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The y axis vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar y_axis() noexcept
        {
            return generic_vec2_scalar(0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec2_scalar The left vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The right vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec2_scalar The down vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The up vector
         */
        RTM_FORCE_INLINE static generic_vec2_scalar up() noexcept
        {
            return y_axis();
        }

    private:
        value_type _x;
        value_type _y;
    };

    using vec2f = generic_vec2_scalar<float>;
    using vec2d = generic_vec2_scalar<double>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec2 = vec2d;
#else
    using vec2 = vec2f;
#endif

    template <typename value_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec2_scalar<value_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2f);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2d);
#endif

template <typename value_type>
struct fmt::formatter<move::vectormath::generic_vec2_scalar<value_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec2_scalar<value_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {})", number.x(), number.y());
    }
};