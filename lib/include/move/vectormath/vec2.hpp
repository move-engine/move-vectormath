#pragma once
#include <cmath>
#include <ostream>
#include <type_traits>

#include <rtm/scalard.h>
#include "macros.hpp"

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
        MVM_INLINE_NODISCARD generic_vec2_scalar() noexcept : _x(0), _y(0)
        {
        }

        /**
         * @brief Construct a new generic_vec2_scalar object
         *
         * @return generic_vec2_scalar A new vector initialized to (x, y)
         */
        MVM_INLINE_NODISCARD generic_vec2_scalar(
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
        MVM_INLINE void serialize(Archive& ar)
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
        MVM_INLINE_NODISCARD bool operator==(
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
        MVM_INLINE_NODISCARD bool operator!=(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x != v._x || _y != v._y;
        }

        /**
         * @brief Determines if all components of the vector are less than the
         * corresponding components of another vector
         *
         * @param v The other vector
         * @return true All components of the vector are less than the
         * corresponding components of the other vector
         * @return false Any component of the vector is greater than or equal
         * to the corresponding component of the other vector
         */
        MVM_INLINE_NODISCARD bool operator<(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x < v._x && _y < v._y;
        }

        /**
         * @brief Determines if all components of the vector are less than or
         * equal to the corresponding components of another vector
         *
         * @param v The other vector
         * @return true All components of the vector are less than or equal to
         * the corresponding components of the other vector
         * @return false Any component of the vector is greater than the
         * corresponding component of the other vector
         */
        MVM_INLINE_NODISCARD bool operator<=(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x <= v._x && _y <= v._y;
        }

        /**
         * @brief Determines if all components of the vector are greater than
         * the corresponding components of another vector
         *
         * @param v The other vector
         * @return true All components of the vector are greater than the
         * corresponding components of the other vector
         * @return false Any component of the vector is less than or equal to
         * the corresponding component of the other vector
         */
        MVM_INLINE_NODISCARD bool operator>(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x > v._x && _y > v._y;
        }

        /**
         * @brief Determines if all components of the vector are greater than or
         * equal to the corresponding components of another vector
         *
         * @param v The other vector
         * @return true All components of the vector are greater than or equal
         * to the corresponding components of the other vector
         * @return false Any component of the vector is less than the
         * corresponding component of the other vector
         */
        MVM_INLINE_NODISCARD bool operator>=(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x >= v._x && _y >= v._y;
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec2_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec2_scalar& operator=(
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
        MVM_INLINE generic_vec2_scalar& operator+=(
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
        MVM_INLINE generic_vec2_scalar& operator-=(
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
        MVM_INLINE generic_vec2_scalar& operator*=(const value_type& v) noexcept
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
        MVM_INLINE generic_vec2_scalar& operator*=(
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
        MVM_INLINE generic_vec2_scalar& operator/=(const value_type& v) noexcept
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
        MVM_INLINE generic_vec2_scalar& operator/=(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator+(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator-(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator*(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator*(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator/(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator/(
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
        MVM_INLINE_NODISCARD generic_vec2_scalar operator-() const noexcept
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
        MVM_INLINE_NODISCARD value_type get_component(int index) const noexcept
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
        MVM_INLINE void set_component(size_t index, value_type value)
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
        MVM_INLINE_NODISCARD value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        /**
         * @brief Stores the components of the vector in an array.  The pointer
         * must point to least 2 elements of type value_type.
         *
         * @param data The array to store the components in (must have at least
         * 4 elements)
         */
        MVM_INLINE void to_data_array(value_type* data) const noexcept
        {
            for (uint32_t i = 0; i < num_components; ++i)
            {
                data[i] = get_component(i);
            }
        }

    public:
        /**
         * @brief Returns the x component of the vector
         *
         * @return value_type The x component
         */
        MVM_INLINE_NODISCARD value_type x() const noexcept
        {
            return _x;
        }

        /**
         * @brief Returns the y component of the vector
         *
         * @return value_type The y component
         */
        MVM_INLINE_NODISCARD value_type y() const noexcept
        {
            return _y;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec2_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec2_scalar& x(value_type x) noexcept
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
        MVM_INLINE generic_vec2_scalar& y(value_type y) noexcept
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
        MVM_INLINE generic_vec2_scalar& set_x(value_type x) noexcept
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
        MVM_INLINE generic_vec2_scalar& set_y(value_type y) noexcept
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

        MVM_INLINE generic_vec2_scalar& fill(value_type v) noexcept
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
        MVM_INLINE generic_vec2_scalar& set(value_type x, value_type y) noexcept
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
        MVM_INLINE generic_vec2_scalar& set(
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
        MVM_INLINE_NODISCARD value_type length() const noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return rtm::scalar_sqrt(squared_length());
            }
            else
            {
                return std::sqrt(squared_length());
            }
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        MVM_INLINE_NODISCARD value_type squared_length() const noexcept
        {
            return _x * _x + _y * _y;
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        MVM_INLINE_NODISCARD value_type reciprocal_length() const noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return rtm::scalar_sqrt_reciprocal(squared_length());
            }
            else
            {
                return value_type(1.0) / std::sqrt(squared_length());
            }
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec2_scalar The normalized vector
         */
        MVM_INLINE_NODISCARD generic_vec2_scalar normalized() const noexcept
        {
            return *this / length();
        }

        /**
         * @brief Returns the absolute value of the vector
         *
         * @return generic_vec2_scalar The absolute vector
         */
        MVM_INLINE_NODISCARD generic_vec2_scalar abs() const noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return generic_vec2_scalar(
                    rtm::scalar_abs(_x), rtm::scalar_abs(_y));
            }
            else
            {
                return generic_vec2_scalar(std::abs(_x), std::abs(_y));
            }
        }

    public:
        /**
         * @brief Returns the dot product of two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type dot(
            const generic_vec2_scalar& v1,
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
        MVM_INLINE_NODISCARD static value_type distance_between_points(
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
        MVM_INLINE_NODISCARD static value_type squared_distance_between_points(
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
        MVM_INLINE_NODISCARD static value_type distance_to_line(
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
        MVM_INLINE_NODISCARD static value_type cross(
            const generic_vec2_scalar& lhs,
            const generic_vec2_scalar& rhs) noexcept
        {
            static_assert(!std::is_unsigned_v<value_type>,
                "Cross product is not defined for unsigned types");

            return lhs._x * rhs._y - lhs._y * rhs._x;
        }

        /**
         * @brief Returns the angle between two normalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_normalized_vectors(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            // Compute with dot product
            auto res = dot(v1, v2);
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return rtm::scalar_acos(res);
            }
            else
            {
                return std::acos(res);
            }
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_vectors(
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
        MVM_INLINE_NODISCARD static generic_vec2_scalar reflect(
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
        MVM_INLINE_NODISCARD static generic_vec2_scalar refract(
            const generic_vec2_scalar& incident,
            const generic_vec2_scalar& normal, value_type ior) noexcept
        {
            // Based on XMVector2RefractV
            auto idotn = dot(incident, normal);

            auto ry = value_type(1) - (idotn * idotn);
            auto rx = value_type(1) - (ry * ior * ior);
            ry = value_type(1) - (ry * ior * ior);

            if (rx >= value_type(0))
            {
                if constexpr (std::is_floating_point_v<value_type>)
                {
                    rx = (ior * incident.x()) -
                         (normal.x() * ((ior * idotn) + rtm::scalar_sqrt(rx)));
                }
                else
                {
                    rx = (ior * incident.x()) -
                         (normal.x() * ((ior * idotn) + std::sqrt(rx)));
                }
            }
            else
            {
                rx = value_type(0);
            }

            if (ry >= value_type(0))
            {
                if constexpr (std::is_floating_point_v<value_type>)
                {
                    ry = (ior * incident.y()) -
                         (normal.y() * ((ior * idotn) + rtm::scalar_sqrt(ry)));
                }
                else
                {
                    ry = (ior * incident.y()) -
                         (normal.y() * ((ior * idotn) + std::sqrt(ry)));
                }
            }
            else
            {
                ry = value_type(0);
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
        MVM_INLINE_NODISCARD static generic_vec2_scalar min(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return generic_vec2_scalar(rtm::scalar_min(v1._x, v2._x),
                    rtm::scalar_min(v1._y, v2._y));
            }
            else
            {
                return generic_vec2_scalar(
                    std::min(v1._x, v2._x), std::min(v1._y, v2._y));
            }
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec2_scalar The maximum vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar max(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return generic_vec2_scalar(rtm::scalar_max(v1._x, v2._x),
                    rtm::scalar_max(v1._y, v2._y));
            }
            else
            {
                return generic_vec2_scalar(
                    std::max(v1._x, v2._x), std::max(v1._y, v2._y));
            }
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return generic_vec2_scalar The clamped vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar clamp(
            const generic_vec2_scalar& v, const generic_vec2_scalar& min,
            const generic_vec2_scalar& max) noexcept
        {
            if constexpr (std::is_floating_point_v<value_type>)
            {
                return generic_vec2_scalar(
                    rtm::scalar_clamp(v._x, min._x, max._x),
                    rtm::scalar_clamp(v._y, min._y, max._y));
            }
            else
            {
                return generic_vec2_scalar(std::clamp(v._x, min._x, max._x),
                    std::clamp(v._y, min._y, max._y));
            }
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE void normalize() noexcept
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
        MVM_INLINE_NODISCARD static generic_vec2_scalar filled(
            value_type value) noexcept
        {
            return generic_vec2_scalar(value, value);
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec2_scalar The infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative infinity.
         *
         * @return generic_vec2_scalar The infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar
        negative_infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec2_scalar The NaN vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec2_scalar The zero vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to negative one.
         *
         * @return generic_vec2_scalar The negative one vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar negative_one() noexcept
        {
            return filled(-1);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec2_scalar The one vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The x axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar x_axis() noexcept
        {
            return generic_vec2_scalar(1, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The y axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar y_axis() noexcept
        {
            return generic_vec2_scalar(0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec2_scalar The left vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The right vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec2_scalar The down vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec2_scalar The up vector
         */
        MVM_INLINE_NODISCARD static generic_vec2_scalar up() noexcept
        {
            return y_axis();
        }

    private:
        value_type _x;
        value_type _y;
    };

    using vec2f32 = generic_vec2_scalar<float>;
    using vec2f64 = generic_vec2_scalar<double>;

    using vec2i8 = generic_vec2_scalar<int8_t>;
    using vec2i16 = generic_vec2_scalar<int16_t>;
    using vec2i32 = generic_vec2_scalar<int32_t>;
    using vec2i64 = generic_vec2_scalar<int64_t>;

    using vec2u8 = generic_vec2_scalar<uint8_t>;
    using vec2u16 = generic_vec2_scalar<uint16_t>;
    using vec2u32 = generic_vec2_scalar<uint32_t>;
    using vec2u64 = generic_vec2_scalar<uint64_t>;

    using vec2f = vec2f32;
    using vec2d = vec2f64;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using vec2 = vec2d;
    using vec2i = vec2i64;
    using vec2u = vec2u64;
#else
    using vec2 = vec2f;
    using ivec2 = vec2i32;
    using vec2i = vec2i32;
    using vec2u = vec2u32;
#endif

    using fvec2 = vec2f;
    using ivec2 = vec2i;
    using uvec2 = vec2u;

    template <typename value_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec2_scalar<value_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2f32)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2f64)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2i8)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2i16)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2i32)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2i64)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2u8)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2u16)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2u32)
// MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2u64)

namespace nlohmann
{
    template <typename value_type>
    struct adl_serializer<move::vectormath::generic_vec2_scalar<value_type>>
        : public move::vectormath::json_serializer_template<
              move::vectormath::generic_vec2_scalar<value_type>,
              move::vectormath::generic_vec2_scalar<value_type>::num_components>
    {
    };
}  // namespace nlohmann

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