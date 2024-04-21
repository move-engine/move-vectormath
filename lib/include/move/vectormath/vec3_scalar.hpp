#pragma once
#include <ostream>

#include "macros.hpp"
#include "move/vectormath/scalar.hpp"
#include "move/vectormath/vec2.hpp"
#include "underlying_types.hpp"

#include <fmt/format.h>

namespace move::vectormath
{
    /**
     * @brief A three component vector that can store floats, doubles, or
     * integer types. The underlying implementation is basic scalar math.
     * @note This type has no alignment requirements.
     *
     * @tparam value_type The type of the components of the vector
     */
    template <typename value_type>
    struct generic_vec3_scalar
    {
    public:
        /**
         * @brief The type of the components of the vector
         */
        using component_type = value_type;

        /**
         * @brief The number of components in the vector
         */
        constexpr static uint32_t num_components = 3;

    public:
        /**
         * @brief Construct a new generic_vec3_scalar object
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar() noexcept : _value(0, 0, 0)
        {
        }

        /**
         * @brief Construct a new generic_vec3_scalar object
         *
         * @param x The x component of the vector
         * @param y The y component of the vector
         * @param z The z component of the vector
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar(
            value_type x, value_type y = 0, value_type z = 0) noexcept
            : _value(x, y, z)
        {
        }

        generic_vec3_scalar(const generic_vec3_scalar& v) = default;

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

        /**
         * @brief Determines if the vector is equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are equal
         * @return false The vectors are not equal
         */
        MVM_INLINE_NODISCARD bool operator==(
            const generic_vec3_scalar& v) const noexcept
        {
            return x() == v.x() && y() == v.y() && z() == v.z();
        }

        /**
         * @brief Determines if the vector is not equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are not equal
         * @return false The vectors are equal
         */
        MVM_INLINE_NODISCARD bool operator!=(
            const generic_vec3_scalar& v) const noexcept
        {
            return x() != v.x() || y() != v.y() || z() != v.z();
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
            const generic_vec3_scalar& v) const noexcept
        {
            return x() < v.x() && y() < v.y() && z() < v.z();
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
            const generic_vec3_scalar& v) const noexcept
        {
            return x() <= v.x() && y() <= v.y() && z() <= v.z();
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
            const generic_vec3_scalar& v) const noexcept
        {
            return x() > v.x() && y() > v.y() && z() > v.z();
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
            const generic_vec3_scalar& v) const noexcept
        {
            return x() >= v.x() && y() >= v.y() && z() >= v.z();
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator=(
            const generic_vec3_scalar& v) noexcept
        {
            x(v.x());
            y(v.y());
            z(v.z());
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator+=(
            const generic_vec3_scalar& v) noexcept
        {
            x(x() + v.x());
            y(y() + v.y());
            z(z() + v.z());
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator-=(
            const generic_vec3_scalar& v) noexcept
        {
            x(x() - v.x());
            y(y() - v.y());
            z(z() - v.z());
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator*=(value_type v) noexcept
        {
            x(x() * v);
            y(y() * v);
            z(z() * v);
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator*=(
            const generic_vec3_scalar& v) noexcept
        {
            x(x() * v.x());
            y(y() * v.y());
            z(z() * v.z());
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator/=(value_type v) noexcept
        {
            x(x() / v);
            y(y() / v);
            z(z() / v);
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& operator/=(
            const generic_vec3_scalar& v) noexcept
        {
            x(x() / v.x());
            y(y() / v.y());
            z(z() / v.z());
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_scalar The result of the addition
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator+(
            const generic_vec3_scalar& v) const noexcept
        {
            return {x() + v.x(), y() + v.y(), z() + v.z()};
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_scalar The result of the subtraction
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator-(
            const generic_vec3_scalar& v) const noexcept
        {
            return {value_type(x() - v.x()), value_type(y() - v.y()),
                value_type(z() - v.z())};
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec3_scalar The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator*(
            value_type v) const noexcept
        {
            return {
                value_type(x() * v), value_type(y() * v), value_type(z() * v)};
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_scalar The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator*(
            const generic_vec3_scalar& v) const noexcept
        {
            return {x() * v.x(), y() * v.y(), z() * v.z()};
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec3_scalar The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator/(
            value_type v) const noexcept
        {
            return {x() / v, y() / v, z() / v};
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_scalar The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator/(
            const generic_vec3_scalar& v) const noexcept
        {
            return {x() / v.x(), y() / v.y(), z() / v.z()};
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec3_scalar The negated vector
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar operator-() const noexcept
        {
            return {-x(), -y(), -z()};
        }

    public:
        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the z component is returned if the index is out of
         * range.
         *
         * @param index The index of the component to return
         * @return value_type The component at the specified index
         */
        MVM_INLINE_NODISCARD value_type get_component(int index) const noexcept
        {
            return _data[scalar::min(index, 2)];
        }

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the w component is set if the index is out of range.
         *
         * @param index The index of the component to set
         * @param value The new value for the component
         */
        MVM_INLINE void set_component(size_t index, value_type value)
        {
            _data[scalar::min(index, size_t(2))] = value;
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

    public:
        /**
         * @brief Returns the x component of the vector
         *
         * @return value_type The x component
         */
        MVM_INLINE_NODISCARD value_type x() const noexcept
        {
            return _value._x;
        }

        /**
         * @brief Returns the y component of the vector
         *
         * @return value_type The y component
         */
        MVM_INLINE_NODISCARD value_type y() const noexcept
        {
            return _value._y;
        }

        /**
         * @brief Returns the z component of the vector
         *
         * @return value_type The z component
         */
        MVM_INLINE_NODISCARD value_type z() const noexcept
        {
            return _value._z;
        }

        /**
         * @brief Returns the x and y components of the vector
         *
         * @return generic_vec2_scalar<value_type> The x and y components
         */
        MVM_INLINE_NODISCARD generic_vec2_scalar<value_type> xy() const noexcept
        {
            return {x(), y()};
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& x(value_type x) noexcept
        {
            return set_x(x);
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& y(value_type y) noexcept
        {
            return set_y(y);
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& z(value_type z) noexcept
        {
            return set_z(z);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& set_x(value_type x) noexcept
        {
            _value._x = x;
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& set_y(value_type y) noexcept
        {
            _value._y = y;
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& set_z(value_type z) noexcept
        {
            _value._z = z;
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& fill(component_type value) noexcept
        {
            set(value, value, value);
            return *this;
        }

        /**
         * @brief Sets the x, y and z components of the vector
         *
         * @param x The new x component
         * @param y The new y component
         * @param z The new z component
         * @return generic_vec3_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec3_scalar& set(
            value_type x, value_type y, value_type z) noexcept
        {
            set_x(x);
            set_y(y);
            set_z(z);
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
            return value_type(std::sqrt(squared_length()));
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        MVM_INLINE_NODISCARD value_type squared_length() const noexcept
        {
            return _value._x * _value._x + _value._y * _value._y +
                   _value._z * _value._z;
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        MVM_INLINE_NODISCARD value_type reciprocal_length() const noexcept
        {
            return value_type(1) / length();
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec3_scalar The normalized vector
         */
        MVM_INLINE_NODISCARD generic_vec3_scalar normalized() const noexcept
        {
            return *this * reciprocal_length();
        }

    public:
        /**
         * @brief Returns the dot product of two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type dot(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return v1.x() * v2.x() + v1.y() * v2.y() + v1.z() * v2.z();
        }

        /**
         * @brief Returns the euclidean distance between two vectors as if they
         * were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type distance_between_points(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return (v2 - v1).length();
        }

        /**
         * @brief Returns the squared euclidean distance between two vectors as
         * if they were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type squared_distance_between_points(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return (v2 - v1).squared_length();
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
            const generic_vec3_scalar& point, const generic_vec3_scalar& v0,
            const generic_vec3_scalar& v1) noexcept
        {
            // Implementation based on XMVector3LinePointDistance
            const auto& pt = point;
            const auto& line_pt1 = v0;
            const auto& line_pt2 = v1;

            const auto point_vector = pt - line_pt1;
            const auto line_vector = line_pt2 - line_pt1;
            const auto length_sq = line_vector.squared_length();
            const auto point_projection_scale =
                point_vector.dot(point_vector, line_vector) / length_sq;
            const auto distance_vector =
                point_vector - line_vector * point_projection_scale;
            return distance_vector.length();
        }

        /**
         * @brief Calculates the cross product of two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_scalar The cross product of the two vectors
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar cross(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return {value_type(v1._value._y * v2._value._z -
                               v1._value._z * v2._value._y),
                value_type(
                    v1._value._z * v2._value._x - v1._value._x * v2._value._z),
                value_type(
                    v1._value._x * v2._value._y - v1._value._y * v2._value._x)};
        }

        /**
         * @brief Returns the angle between two normalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_normalized_vectors(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return std::acos(dot(v1, v2));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_vectors(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
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
        MVM_INLINE_NODISCARD static generic_vec3_scalar reflect(
            const generic_vec3_scalar& incident,
            const generic_vec3_scalar& normal) noexcept
        {
            // Based on XMVector3Reflect
            const auto& inc = incident;
            const auto& nrm = normal;

            auto dot_incnrm = dot(inc, nrm);
            auto dot2 = dot_incnrm + dot_incnrm;
            auto mul = nrm * dot2;
            auto res = inc - mul;
            return res;
        }

        /**
         * @brief Refracts incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         * @param ior The index of refraction
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar refract(
            const generic_vec3_scalar& incident,
            const generic_vec3_scalar& normal, value_type ior) noexcept
        {
            auto dotinorm = dot(incident, normal);
            auto roi_plus_dotinorm = ior * dotinorm;
            auto inner_sqrt =
                (std::sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));

            return ior * incident - (roi_plus_dotinorm + inner_sqrt) * normal;
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  Can be used for extrapolation as well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec3_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar lerp_unclamped(
            const generic_vec3_scalar& v1, const generic_vec3_scalar& v2,
            value_type t) noexcept
        {
            const auto scalar_lerp =
                [](value_type a, value_type b, value_type t) noexcept
            {
                return a + (b - a) * t;
            };

            return {scalar_lerp(v1._x, v2._x, t), scalar_lerp(v1._y, v2._y, t),
                scalar_lerp(v1._z, v2._z, t)};
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec3_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar lerp_unclamped(
            const generic_vec3_scalar& v1, const generic_vec3_scalar& v2,
            const generic_vec3_scalar& t) noexcept
        {
            const auto scalar_lerp =
                [](value_type a, value_type b, value_type t) noexcept
            {
                return a + (b - a) * t;
            };

            return {scalar_lerp(v1._x, v2._x, t._x),
                scalar_lerp(v1._y, v2._y, t._y),
                scalar_lerp(v1._z, v2._z, t._z)};
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  The interpolation value is clamped
         * between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec3_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar lerp(
            const generic_vec3_scalar& v1, const generic_vec3_scalar& v2,
            value_type t) noexcept
        {
            auto clamped = std::clamp(t, value_type(0), value_type(1));
            return lerp_unclamped(v1, v2, clamped);
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  The interpolation values are
         * clamped between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec3_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar lerp(
            const generic_vec3_scalar& v1, const generic_vec3_scalar& v2,
            const generic_vec3_scalar& t) noexcept
        {
            auto clamped = clamp(t, zero(), one());
            return lerp_unclamped(v1, v2, clamped);
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_scalar The minimum vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar min(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return {
                scalar::min(v1.x(), v2.x()),
                scalar::min(v1.y(), v2.y()),
                scalar::min(v1.z(), v2.z()),
            };
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_scalar The maximum vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar max(
            const generic_vec3_scalar& v1,
            const generic_vec3_scalar& v2) noexcept
        {
            return {
                scalar::max(v1.x(), v2.x()),
                scalar::max(v1.y(), v2.y()),
                scalar::max(v1.z(), v2.z()),
            };
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return generic_vec3_scalar The clamped vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar clamp(
            const generic_vec3_scalar& v, const generic_vec3_scalar& min,
            const generic_vec3_scalar& max) noexcept
        {
            return {
                scalar::clamp(v.x(), min.x(), max.x()),
                scalar::clamp(v.y(), min.y(), max.y()),
                scalar::clamp(v.z(), min.z(), max.z()),
            };
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE void normalize() noexcept
        {
            *this *= reciprocal_length();
        }

    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to set all components to
         * @return generic_vec3_scalar The filled vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar filled(
            value_type value) noexcept
        {
            return {value, value, value};
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec3_scalar The infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return generic_vec3_scalar The negative infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar
        negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec3_scalar The NaN vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar nan() noexcept
        {
            static_assert(std::numeric_limits<value_type>::has_quiet_NaN,
                "The value type does not support NaN");
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec3_scalar The zero vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to negative one.
         *
         * @return generic_vec3_scalar The negative one vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar negative_one() noexcept
        {
            return filled(-1);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec3_scalar The one vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The x axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar x_axis() noexcept
        {
            return generic_vec3_scalar(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The y axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar y_axis() noexcept
        {
            return generic_vec3_scalar(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The z axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar z_axis() noexcept
        {
            return generic_vec3_scalar(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_scalar The left vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The right vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_scalar The down vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The up vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_scalar The backward vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_scalar The forward vector
         */
        MVM_INLINE_NODISCARD static generic_vec3_scalar forward() noexcept
        {
            return z_axis();
        }

    private:
        union
        {
            struct data
            {
                inline data(value_type x, value_type y, value_type z) noexcept
                    : _x(x), _y(y), _z(z)
                {
                }

                value_type _x, _y, _z;
            } _value;
            value_type _data[3];
        };
    };

    template <typename value_type>
    MVM_INLINE_NODISCARD generic_vec3_scalar<value_type> operator*(
        value_type v, const generic_vec3_scalar<value_type>& vec) noexcept
    {
        return vec * v;
    }

    using vec3f32_scalar = generic_vec3_scalar<float>;
    using vec3f64_scalar = generic_vec3_scalar<double>;

    using vec3i8_scalar = generic_vec3_scalar<int8_t>;
    using vec3i16_scalar = generic_vec3_scalar<int16_t>;
    using vec3i32_scalar = generic_vec3_scalar<int32_t>;
    using vec3i64_scalar = generic_vec3_scalar<int64_t>;

    using vec3u8_scalar = generic_vec3_scalar<uint8_t>;
    using vec3u16_scalar = generic_vec3_scalar<uint16_t>;
    using vec3u32_scalar = generic_vec3_scalar<uint32_t>;
    using vec3u64_scalar = generic_vec3_scalar<uint64_t>;

    using vec3f_scalar = vec3f32_scalar;
    using vec3d_scalar = vec3f64_scalar;
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"

namespace nlohmann
{
    template <typename value_type>
    struct adl_serializer<move::vectormath::generic_vec3_scalar<value_type>>
        : public move::vectormath::json_serializer_template<
              move::vectormath::generic_vec3_scalar<value_type>,
              move::vectormath::generic_vec3_scalar<value_type>::num_components>
    {
    };
}  // namespace nlohmann

#endif