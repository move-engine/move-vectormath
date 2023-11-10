#pragma once
#include <ostream>

#include "macros.hpp"
#include "scalar.hpp"
#include "underlying_types.hpp"

#include <fmt/format.h>

namespace move::vectormath
{
    /**
     * @brief A four component vector that can store either floats, doubles, or
     * integer types. The underlying implementation is basic scalar math.
     * @note This type has no alignment requirements
     *
     * @tparam value_type The type of the components of the vector
     */
    template <typename value_type>
    struct generic_vec4_scalar
    {
    public:
        /**
         * @brief The type of the components of the vector
         */
        using component_type = value_type;

        /**
         * @brief The number of components in the vector
         */
        constexpr static uint32_t num_components = 4;

    public:
        /**
         * @brief Construct a new generic_vec4_scalar
         *
         * @return generic_vec4_scalar A new vector initialized to (0, 0, 0, 0)
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar() noexcept : _value(0, 0, 0, 0)
        {
        }

        /**
         * @brief Construct a new generic_vec4_scalar object
         *
         * @param x The x component
         * @param y The y component.  Defaults to 0.
         * @param z The z component.  Defaults to 0.
         * @param w The w component.  Defaults to 0.
         * @return generic_vec4_scalar A new vector initialized to (x, y, z, w)
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar(value_type x, value_type y = 0,
            value_type z = 0, value_type w = 0) noexcept
            : _value(x, y, z, w)
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
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x == v._value._x && _value._y == v._value._y &&
                   _value._z == v._value._z && _value._w == v._value._w;
        }

        /**
         * @brief Determines if the vector is not equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are not equal
         * @return false The vectors are equal
         */
        MVM_INLINE_NODISCARD bool operator!=(
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x != v._value._x || _value._y != v._value._y ||
                   _value._z != v._value._z || _value._w != v._value._w;
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
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x < v._value._x && _value._y < v._value._y &&
                   _value._z < v._value._z && _value._w < v._value._w;
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
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x <= v._value._x && _value._y <= v._value._y &&
                   _value._z <= v._value._z && _value._w <= v._value._w;
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
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x > v._value._x && _value._y > v._value._y &&
                   _value._z > v._value._z && _value._w > v._value._w;
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
            const generic_vec4_scalar& v) const noexcept
        {
            return _value._x >= v._value._x && _value._y >= v._value._y &&
                   _value._z >= v._value._z && _value._w >= v._value._w;
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator=(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x = v._value._x;
            _value._y = v._value._y;
            _value._z = v._value._z;
            _value._w = v._value._w;
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator+=(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x += v._value._x;
            _value._y += v._value._y;
            _value._z += v._value._z;
            _value._w += v._value._w;
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator-=(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x -= v._value._x;
            _value._y -= v._value._y;
            _value._z -= v._value._z;
            _value._w -= v._value._w;
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator*=(value_type v) noexcept
        {
            _value._x *= v;
            _value._y *= v;
            _value._z *= v;
            _value._w *= v;
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator*=(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x *= v._value._x;
            _value._y *= v._value._y;
            _value._z *= v._value._z;
            _value._w *= v._value._w;
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator/=(value_type v) noexcept
        {
            _value._x /= v;
            _value._y /= v;
            _value._z /= v;
            _value._w /= v;
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& operator/=(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x /= v._value._x;
            _value._y /= v._value._y;
            _value._z /= v._value._z;
            _value._w /= v._value._w;
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_scalar The result of the addition
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator+(
            const generic_vec4_scalar& v) const noexcept
        {
            return generic_vec4_scalar(_value._x + v._value._x,
                _value._y + v._value._y, _value._z + v._value._z,
                _value._w + v._value._w);
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_scalar The result of the subtraction
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator-(
            const generic_vec4_scalar& v) const noexcept
        {
            return generic_vec4_scalar(_value._x - v._value._x,
                _value._y - v._value._y, _value._z - v._value._z,
                _value._w - v._value._w);
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4_scalar The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator*(
            value_type v) const noexcept
        {
            return generic_vec4_scalar(
                _value._x * v, _value._y * v, _value._z * v, _value._w * v);
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_scalar The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator*(
            const generic_vec4_scalar& v) const noexcept
        {
            return generic_vec4_scalar(_value._x * v._value._x,
                _value._y * v._value._y, _value._z * v._value._z,
                _value._w * v._value._w);
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4_scalar The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator/(
            value_type v) const noexcept
        {
            return generic_vec4_scalar(
                _value._x / v, _value._y / v, _value._z / v, _value._w / v);
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_scalar The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator/(
            const generic_vec4_scalar& v) const noexcept
        {
            return generic_vec4_scalar(_value._x / v._value._x,
                _value._y / v._value._y, _value._z / v._value._z,
                _value._w / v._value._w);
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec4_scalar The result of the negation
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar operator-() const noexcept
        {
            return generic_vec4_scalar(
                -_value._x, -_value._y, -_value._z, -_value._w);
        }

    public:
        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the w component is returned if the index is out of
         * range.
         *
         * @param index The index of the component to return
         * @return value_type The component at the specified index
         */
        MVM_INLINE_NODISCARD value_type get_component(int index) const noexcept
        {
            return _data[scalar::min(index, 3)];
        }

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * set.
         *
         * @param index The index of the component to set
         * @param value The new value for the component
         */
        MVM_INLINE void set_component(int index, value_type value)
        {
            _data[scalar::min(index, 3)] = value;
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
         * @brief Returns the w component of the vector
         *
         * @return value_type The w component
         */
        MVM_INLINE_NODISCARD value_type w() const noexcept
        {
            return _value._w;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& x(value_type value) noexcept
        {
            set_x(value);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& y(value_type value) noexcept
        {
            set_y(value);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& z(value_type value) noexcept
        {
            set_z(value);
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& w(value_type value) noexcept
        {
            set_w(value);
            return *this;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& set_x(value_type x) noexcept
        {
            _value._x = x;
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& set_y(value_type y) noexcept
        {
            _value._y = y;
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& set_z(value_type z) noexcept
        {
            _value._z = z;
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE generic_vec4_scalar& set_w(value_type w) noexcept
        {
            _value._w = w;
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& fill(
            component_type value) noexcept
        {
            _value._x = value;
            _value._y = value;
            _value._z = value;
            _value._w = value;
            return *this;
        }

        /**
         * @brief Sets the components of the vector
         *
         * @param x The new x component
         * @param y The new y component
         * @param z The new z component
         * @param w The new w component
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& set(component_type x,
            component_type y, component_type z, component_type w) noexcept
        {
            _value._x = x;
            _value._y = y;
            _value._z = z;
            _value._w = w;
            return *this;
        }

        /**
         * @brief Sets the components of the vector
         *
         * @param v The new vector
         * @return generic_vec4_scalar& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar& set(
            const generic_vec4_scalar& v) noexcept
        {
            _value._x = v._value._x;
            _value._y = v._value._y;
            _value._z = v._value._z;
            _value._w = v._value._w;
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
            return scalar::sqrt(squared_length());
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        MVM_INLINE_NODISCARD value_type squared_length() const noexcept
        {
            return (_value._x * _value._x) + (_value._y * _value._y) +
                   (_value._z * _value._z) + (_value._w * _value._w);
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        MVM_INLINE_NODISCARD value_type reciprocal_length() const noexcept
        {
            return scalar::reciprocal(length());
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec4_scalar A normalized copy of the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar normalized() const noexcept
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
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return (v1._value._x * v2._value._x) +
                   (v1._value._y * v2._value._y) +
                   (v1._value._z * v2._value._z) +
                   (v1._value._w * v2._value._w);
        }

        /**
         * @brief Returns the euclidean distance between two vectors as if they
         * were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type distance_between_points(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return (v2 - v1).length();
        }

        /**
         * @brief Returns the squared euclidean distance between two vectors as
         * if they were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The squared distance between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type squared_distance_between_points(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return (v2 - v1).squared_length();
        }

        /**
         * @brief Calculates the cross product between three four-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param v3 The third vector
         * @return generic_vec4_scalar The cross product
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar cross(
            const generic_vec4_scalar& v1, const generic_vec4_scalar& v2,
            const generic_vec4_scalar& v3) noexcept
        {
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

        /**
         * @brief Returns the angle between two normalizede vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_normalized_vectors(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return scalar::acos(dot(v1, v2));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_vectors(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            auto v1norm = v1.normalized();
            auto v2norm = v2.normalized();
            return angle_between_normalized_vectors(v1norm, v2norm);
        }

        /**
         * @brief Reflects incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar reflect(
            const generic_vec4_scalar& incident,
            const generic_vec4_scalar& normal) noexcept
        {
            auto dot = generic_vec4_scalar::dot(incident, normal);
            auto dot2 = dot + dot;
            auto mul = normal * dot2;
            return incident - mul;
        }

        /**
         * @brief Refracts incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         * @param ior The index of refraction
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar refract(
            const generic_vec4_scalar& inc, const generic_vec4_scalar& nrm,
            value_type ior) noexcept
        {
            generic_vec4_scalar index = generic_vec4_scalar(ior, ior, ior, ior);

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

            value_type dotinorm = generic_vec4_scalar::dot(inc, nrm);
            generic_vec4_scalar roiPlusDotinorm = index * dotinorm;
            generic_vec4_scalar innerSqrt = generic_vec4_scalar::filled(
                scalar::sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
            ;

            return ((index * inc) - (nrm * (roiPlusDotinorm + innerSqrt)));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  Can be used for extrapolation as well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec4_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar lerp_unclamped(
            const generic_vec4_scalar& v1, const generic_vec4_scalar& v2,
            value_type t) noexcept
        {
            return generic_vec4_scalar(
                scalar::lerp_unclamped(v1._value._x, v2._value._x, t),
                scalar::lerp_unclamped(v1._value._y, v2._value._y, t),
                scalar::lerp_unclamped(v1._value._z, v2._value._z, t),
                scalar::lerp_unclamped(v1._value._w, v2._value._w, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec4_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar lerp_unclamped(
            const generic_vec4_scalar& v1, const generic_vec4_scalar& v2,
            const generic_vec4_scalar& t) noexcept
        {
            return generic_vec4_scalar(
                scalar::lerp_unclamped(v1._value._x, v2._value._x, t._value._x),
                scalar::lerp_unclamped(v1._value._y, v2._value._y, t._value._y),
                scalar::lerp_unclamped(v1._value._z, v2._value._z, t._value._z),
                scalar::lerp_unclamped(
                    v1._value._w, v2._value._w, t._value._w));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  The interpolation value is clamped
         * between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec4_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar lerp(
            const generic_vec4_scalar& v1, const generic_vec4_scalar& v2,
            value_type t) noexcept
        {
            return generic_vec4_scalar(
                scalar::lerp(v1._value._x, v2._value._x, t),
                scalar::lerp(v1._value._y, v2._value._y, t),
                scalar::lerp(v1._value._z, v2._value._z, t),
                scalar::lerp(v1._value._w, v2._value._w, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  The interpolation values are
         * clamped between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec4_scalar The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar lerp(
            const generic_vec4_scalar& v1, const generic_vec4_scalar& v2,
            const generic_vec4_scalar& t) noexcept
        {
            return generic_vec4_scalar(
                scalar::lerp(v1._value._x, v2._value._x, t._value._x),
                scalar::lerp(v1._value._y, v2._value._y, t._value._y),
                scalar::lerp(v1._value._z, v2._value._z, t._value._z),
                scalar::lerp(v1._value._w, v2._value._w, t._value._w));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4_scalar The minimum vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar min(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return generic_vec4_scalar(scalar::min(v1._value._x, v2._value._x),
                scalar::min(v1._value._y, v2._value._y),
                scalar::min(v1._value._z, v2._value._z),
                scalar::min(v1._value._w, v2._value._w));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4_scalar The maximum vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar max(
            const generic_vec4_scalar& v1,
            const generic_vec4_scalar& v2) noexcept
        {
            return generic_vec4_scalar(scalar::max(v1._value._x, v2._value._x),
                scalar::max(v1._value._y, v2._value._y),
                scalar::max(v1._value._z, v2._value._z),
                scalar::max(v1._value._w, v2._value._w));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return generic_vec4_scalar The clamped vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar clamp(
            const generic_vec4_scalar& v, const generic_vec4_scalar& min,
            const generic_vec4_scalar& max) noexcept
        {
            return generic_vec4_scalar(
                scalar::clamp(v._value._x, min._value._x, max._value._x),
                scalar::clamp(v._value._y, min._value._y, max._value._y),
                scalar::clamp(v._value._z, min._value._z, max._value._z),
                scalar::clamp(v._value._w, min._value._w, max._value._w));
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_scalar& A reference to the vector
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
         * @param value The value to fill the vector with
         * @return generic_vec4_scalar The filled vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar filled(
            value_type value) noexcept
        {
            return generic_vec4_scalar(value, value, value, value);
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec4_scalar The infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return generic_vec4_scalar The negative infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar
        negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec4_scalar The NaN vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec4_scalar The zero vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec4_scalar The one vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The x axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar x_axis() noexcept
        {
            return generic_vec4_scalar(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The y axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar y_axis() noexcept
        {
            return generic_vec4_scalar(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The z axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar z_axis() noexcept
        {
            return generic_vec4_scalar(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the w component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The w axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar w_axis() noexcept
        {
            return generic_vec4_scalar(0, 0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_scalar The left vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The right vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_scalar The down vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The up vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_scalar The back vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_scalar The forward vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_scalar forward() noexcept
        {
            return z_axis();
        }

    private:
        union
        {
            struct data
            {
                inline data(value_type x, value_type y, value_type z,
                    value_type w) noexcept
                    : _x(x), _y(y), _z(z), _w(w)
                {
                }

                value_type _x, _y, _z, _w;
            } _value;
            value_type _data[4];
        };
    };

    template <typename value_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec4_scalar<value_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", "
                  << v.w() << ")";
    }

    using vec4f32_scalar = generic_vec4_scalar<float>;
    using vec4f64_scalar = generic_vec4_scalar<double>;

    using vec4i8_scalar = generic_vec4_scalar<std::int8_t>;
    using vec4i16_scalar = generic_vec4_scalar<std::int16_t>;
    using vec4i32_scalar = generic_vec4_scalar<std::int32_t>;
    using vec4i64_scalar = generic_vec4_scalar<std::int64_t>;

    using vec4u8_scalar = generic_vec4_scalar<std::uint8_t>;
    using vec4u16_scalar = generic_vec4_scalar<std::uint16_t>;
    using vec4u32_scalar = generic_vec4_scalar<std::uint32_t>;
    using vec4u64_scalar = generic_vec4_scalar<std::uint64_t>;

    using vec4f_scalar = vec4f32_scalar;
    using vec4d_scalar = vec4f64_scalar;
}  // namespace move::vectormath

template <typename value_type>
struct fmt::formatter<move::vectormath::generic_vec4_scalar<value_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec4_scalar<value_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {}, {}, {})", number.x(), number.y(),
            number.z(), number.w());
    }
};

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"

namespace nlohmann
{
    template <typename value_type>
    struct adl_serializer<move::vectormath::generic_vec4_scalar<value_type>>
        : public move::vectormath::json_serializer_template<
              move::vectormath::generic_vec4_scalar<value_type>,
              move::vectormath::generic_vec4_scalar<value_type>::num_components>
    {
    };
}  // namespace nlohmann

#endif