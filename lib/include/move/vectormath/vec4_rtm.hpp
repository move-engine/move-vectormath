#pragma once
#include <ostream>

#include "macros.hpp"
#include "move/vectormath/vec2.hpp"
#include "move/vectormath/vec3_rtm.hpp"
#include "move/vectormath/vec3_scalar.hpp"
#include "move/vectormath/vec4_scalar.hpp"
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
    /**
     * @brief A four component vector that can store either floats or doubles.
     * The underlying implementation is RTM's vector4f or vector4d.
     * @note Instances of this class must be aligned to 16 bytes.
     *
     * @tparam value_type The type of the components of the vector
     * @tparam vector_type_raw The underlying RTM vector type
     */
    template <typename value_type, typename vector_type_raw>
    struct generic_vec4_rtm
    {
    public:
        /**
         * @brief The type of the components of the vector
         */
        using component_type = value_type;

        /*
         * @brief The type of the underlying RTM vector.  Should be either
         * rtm::vector4f or rtm::vector4d.
         */
        using vector_type = typename vector_type_raw::type;

        /**
         * @brief The number of components in the vector
         */
        constexpr static uint32_t num_components = 4;

    public:
        /**
         * @brief Construct a new generic_vec4_rtm object
         *
         * @return generic_vec4_rtm A new vector initialized to (0, 0, 0, 0)
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        /**
         * @brief Construct a new generic_vec4_rtm object
         *
         * @param x The x component
         * @param y The y component.  Defaults to 0.
         * @param z The z component.  Defaults to 0.
         * @param w The w component.  Defaults to 0.
         * @return generic_vec4_rtm A new vector initialized to (x, y, z, w)
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(value_type x, value_type y = 0,
            value_type z = 0, value_type w = 0) noexcept
            : _value(rtm::vector_set(x, y, z, w))
        {
        }

        /**
         * @brief Construct a new generic_vec4_rtm object using an RTM vector
         *
         * @param v The RTM vector to copy
         * @return generic_vec4_rtm A new vector initialized to v
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(const vector_type& v) noexcept
            : _value(v)
        {
        }

        /**
         * @brief Fill the X and Y components of a new generic_vec4_rtm object
         * with those of a scalar vector2, filling the remaining components with
         * the provided values.
         *
         * @param v The scalar vector2 to copy
         * @return generic_vec4_rtm A new vector initialized to v
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(
            const generic_vec2_scalar<value_type>& v, value_type z = 0,
            value_type w = 0) noexcept
            : _value(rtm::vector_set(v.x(), v.y(), z, w))
        {
        }

        /**
         * @brief Fill the X, Y, and Z components of a new generic_vec4_rtm
         * object with those of a scalar vector3, filling the remaining
         * component with the provided values.
         *
         * @param v The scalar vector3 to copy
         * @return generic_vec4_rtm A new vector initialized to v
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(
            const generic_vec3_rtm<value_type, vector_type_raw>& v,
            value_type w = 0) noexcept
            : _value(rtm::vector_set(v.x(), v.y(), v.z(), w))
        {
        }

        /**
         * @brief Fill the X, Y, and Z components of a new generic_vec4_rtm
         * object with those of an RTM vector3, filling the remaining component
         * with the provided values.
         *
         * @param v The scalar vector3 to copy
         * @return generic_vec4_rtm A new vector initialized to v
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(
            const generic_vec3_scalar<value_type>& v, value_type w = 0) noexcept
            : _value(rtm::vector_set(v.x(), v.y(), v.z(), w))
        {
        }

        /**
         * @brief Fill the X, Y, Z, and W components of a new generic_vec4_rtm
         * object with those of a scalar vector4.
         *
         * @param v The scalar vector4 to copy
         * @return generic_vec4_rtm A new vector initialized to v
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm(
            const generic_vec4_scalar<value_type>& v) noexcept
            : _value(rtm::vector_set(v.x(), v.y(), v.z(), v.w()))
        {
        }

    public:
        /**
         * @brief Returns the internal RTM vector.  Useful for tight loops,
         * where RTM may outperform the higher level API.
         *
         * @return const vector_type& A reference to the internal RTM vector.
         */
        MVM_INLINE_NODISCARD const vector_type& get_internal() const noexcept
        {
            return _value;
        }

        /** @brief Returns the internal RTM vector.  Useful for tight loops,
         * where RTM may outperform the higher level API.
         *
         * @return vector_type& A reference to the internal RTM vector.
         */
        MVM_INLINE_NODISCARD vector_type& get_internal() noexcept
        {
            return _value;
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
        /***
         * @brief Converts an RTM vector to a scalar vector
         *
         * @return generic_vec4_scalar<value_type> The scalar vector
         */
        MVM_INLINE_NODISCARD operator generic_vec4_scalar<value_type>()
            const noexcept
        {
            return generic_vec4_scalar<value_type>(x(), y(), z(), w());
        };

        /***
         * @brief Converts an RTM vector to a scalar vector
         *
         * @return generic_vec4_scalar<value_type> The scalar vector
         */
        MVM_INLINE_NODISCARD generic_vec4_scalar<value_type> to_scalar()
            const noexcept
        {
            return generic_vec4_scalar<value_type>(x(), y(), z(), w());
        }

        /**
         * @brief Returns a reference to the vector as a RTM vector.  This both
         * enables the compiler to do additional optimizations, as well as
         * allowing you to use the vector in functions that take RTM vectors
         * directly.
         *
         * @return vector_type& A reference to the RTM vector
         */
        MVM_INLINE_NODISCARD operator vector_type&()
        {
            return _value;
        }

        /**
         * @brief Returns a reference to the vector as a RTM vector.  This both
         * enables the compiler to do additional optimizations, as well as
         * allowing you to use the vector in functions that take RTM vectors
         * directly.
         *
         * @return const vector_type& A reference to the RTM vector
         */
        MVM_INLINE_NODISCARD operator const vector_type&() const
        {
            return _value;
        }

        /**
         * @brief Determines if the vector is equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are equal
         * @return false The vectors are not equal
         */
        MVM_INLINE_NODISCARD bool operator==(
            const generic_vec4_rtm& v) const noexcept
        {
            return rtm::mask_all_true(rtm::vector_equal(_value, v._value));
        }

        /**
         * @brief Determines if the vector is not equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are not equal
         * @return false The vectors are equal
         */
        MVM_INLINE_NODISCARD bool operator!=(
            const generic_vec4_rtm& v) const noexcept
        {
            return !rtm::mask_all_true(rtm::vector_equal(_value, v._value));
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
            const generic_vec4_rtm& v) const noexcept
        {
            return rtm::mask_all_true(rtm::vector_less_than(_value, v._value));
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
            const generic_vec4_rtm& v) const noexcept
        {
            return rtm::mask_all_true(rtm::vector_less_equal(_value, v._value));
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
            const generic_vec4_rtm& v) const noexcept
        {
            return rtm::mask_all_true(
                rtm::vector_greater_than(_value, v._value));
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
            const generic_vec4_rtm& v) const noexcept
        {
            return rtm::mask_all_true(
                rtm::vector_greater_equal(_value, v._value));
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator=(
            const generic_vec4_rtm& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator+=(
            const generic_vec4_rtm& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator-=(
            const generic_vec4_rtm& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator*=(value_type v) noexcept
        {
            _value = rtm::vector_mul(_value, v);
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator*=(
            const generic_vec4_rtm& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator/=(value_type v) noexcept
        {
            _value = rtm::vector_div(_value, rtm::vector_set(v));
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& operator/=(
            const generic_vec4_rtm& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_rtm The result of the addition
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator+(
            const generic_vec4_rtm& v) const noexcept
        {
            return generic_vec4_rtm(rtm::vector_add(_value, v._value));
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_rtm The result of the subtraction
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator-(
            const generic_vec4_rtm& v) const noexcept
        {
            return generic_vec4_rtm(rtm::vector_sub(_value, v._value));
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4_rtm The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator*(
            value_type v) const noexcept
        {
            return generic_vec4_rtm(rtm::vector_mul(_value, v));
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_rtm The result of the multiplication
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator*(
            const generic_vec4_rtm& v) const noexcept
        {
            return generic_vec4_rtm(rtm::vector_mul(_value, v._value));
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4_rtm The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator/(
            value_type v) const noexcept
        {
            return generic_vec4_rtm(
                rtm::vector_div(_value, rtm::vector_set(v)));
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4_rtm The result of the division
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator/(
            const generic_vec4_rtm& v) const noexcept
        {
            return generic_vec4_rtm(rtm::vector_div(_value, v._value));
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec4_rtm The result of the negation
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm operator-() const noexcept
        {
            return generic_vec4_rtm(rtm::vector_neg(_value));
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
            return rtm::vector_get_component(_value, rtm::mix4(index));
        }

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * set.
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
            return rtm::vector_get_x(_value);
        }

        /**
         * @brief Returns the y component of the vector
         *
         * @return value_type The y component
         */
        MVM_INLINE_NODISCARD value_type y() const noexcept
        {
            return rtm::vector_get_y(_value);
        }

        /**
         * @brief Returns the z component of the vector
         *
         * @return value_type The z component
         */
        MVM_INLINE_NODISCARD value_type z() const noexcept
        {
            return rtm::vector_get_z(_value);
        }

        /**
         * @brief Returns the x and y components of the vector
         *
         * @return generic_vec2_scalar<value_type> The x, y components
         */
        MVM_INLINE_NODISCARD generic_vec2_scalar<value_type> xy() const noexcept
        {
            return generic_vec2_scalar<value_type>(x(), y());
        }

        /**
         * @brief Returns the x, y, and z components of the vector
         *
         * @return generic_vec3_rtm<value_type, vector_type_raw> The x, y, and z
         * components
         */
        MVM_INLINE_NODISCARD generic_vec3_rtm<value_type, vector_type_raw> xyz()
            const noexcept
        {
            return generic_vec3_rtm<value_type, vector_type_raw>(x(), y(), z());
        }

        /**
         * @brief Returns the w component of the vector
         *
         * @return value_type The w component
         */
        MVM_INLINE_NODISCARD value_type w() const noexcept
        {
            return rtm::vector_get_w(_value);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm& x(value_type value) noexcept
        {
            set_x(value);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm& y(value_type value) noexcept
        {
            set_y(value);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm& z(value_type value) noexcept
        {
            set_z(value);
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm& w(value_type value) noexcept
        {
            set_w(value);
            return *this;
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set_w(value_type w) noexcept
        {
            _value = rtm::vector_set_w(_value, w);
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& fill(component_type value) noexcept
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
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set(component_type x, component_type y,
            component_type z, component_type w) noexcept
        {
            _value = rtm::vector_set(x, y, z, w);
            return *this;
        }

        /**
         * @brief Sets the components of the vector
         *
         * @param v The new vector
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE generic_vec4_rtm& set(const generic_vec4_rtm& v) noexcept
        {
            _value = v._value;
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
            return rtm::vector_length(_value);
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        MVM_INLINE_NODISCARD value_type squared_length() const noexcept
        {
            return rtm::vector_length_squared(_value);
        }

        /**
         * @brief Returns the reciprocal length of the vector
         *
         * @return value_type The reciprocal length of the vector
         */
        MVM_INLINE_NODISCARD value_type reciprocal_length() const noexcept
        {
            return rtm::vector_length_reciprocal(_value);
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec4_rtm A normalized copy of the vector
         */
        MVM_INLINE_NODISCARD generic_vec4_rtm normalized() const noexcept
        {
            return rtm::vector_mul(_value, value_type(1) / length());
        }

    public:
        /**
         * @brief Returns the dot product of two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type dot(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
        {
            return rtm::vector_dot(v1._value, v2._value);
        }

        /**
         * @brief Returns the euclidean distance between two vectors as if they
         * were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        MVM_INLINE_NODISCARD static value_type distance_between_points(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
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
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
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
         * @return generic_vec4_rtm The cross product
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm cross(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2,
            const generic_vec4_rtm& v3) noexcept
        {
            // TODO: Write a faster implementation, because this is terrible
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
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
        {
            return std::acos(rtm::vector_dot(v1._value, v2._value));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static value_type angle_between_vectors(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
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
        MVM_INLINE_NODISCARD static generic_vec4_rtm reflect(
            const generic_vec4_rtm& incident,
            const generic_vec4_rtm& normal) noexcept
        {
            using namespace rtm;
            const vector_type& inc = incident._value;
            const vector_type& nrm = normal._value;

            auto dot = vector_dot(inc, nrm);
            auto dot2 = vector_add(dot, dot);
            auto mul = vector_mul(nrm, dot2);
            auto res = vector_sub(inc, mul);
            return res;
        }

        /**
         * @brief Refracts incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         * @param ior The index of refraction
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm refract(
            const generic_vec4_rtm& incident, const generic_vec4_rtm& normal,
            value_type ior) noexcept
        {
            using namespace rtm;
            const vector_type& inc = incident;
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

            value_type dotinorm = vector_dot(inc, nrm);
            vector_type roiPlusDotinorm = vector_mul(index, dotinorm);
            vector_type innerSqrt = vector_set(
                scalar_sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
            ;

            return vector_sub(vector_mul(index, inc),
                vector_mul(nrm, vector_add(roiPlusDotinorm, innerSqrt)));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  Can be used for extrapolation as well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec4_rtm The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm lerp_unclamped(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2,
            value_type t) noexcept
        {
            return generic_vec4_rtm(rtm::vector_lerp(v1._value, v2._value, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec4_rtm The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm lerp_unclamped(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2,
            const generic_vec4_rtm& t) noexcept
        {
            return generic_vec4_rtm(
                rtm::vector_lerp(v1._value, v2._value, t._value));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  The interpolation value is clamped
         * between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return generic_vec4_rtm The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm lerp(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2,
            value_type t) noexcept
        {
            return generic_vec4_rtm(rtm::vector_lerp(v1._value, v2._value,
                rtm::scalar_clamp(t, value_type(0), value_type(1))));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  The interpolation values are
         * clamped between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec4_rtm The result of the interpolation
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm lerp(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2,
            const generic_vec4_rtm& t) noexcept
        {
            return generic_vec4_rtm(rtm::vector_lerp(v1._value, v2._value,
                rtm::vector_clamp(t._value, rtm::vector_set(value_type(0)),
                    vector_set(value_type(1)))));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4_rtm The minimum vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm min(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
        {
            return generic_vec4_rtm(rtm::vector_min(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4_rtm The maximum vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm max(
            const generic_vec4_rtm& v1, const generic_vec4_rtm& v2) noexcept
        {
            return generic_vec4_rtm(rtm::vector_max(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return generic_vec4_rtm The clamped vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm clamp(
            const generic_vec4_rtm& v, const generic_vec4_rtm& min,
            const generic_vec4_rtm& max) noexcept
        {
            return generic_vec4_rtm(
                rtm::vector_clamp(v._value, min._value, max._value));
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_rtm& A reference to the vector
         */
        MVM_INLINE void normalize() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized();
        }

    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to fill the vector with
         * @return generic_vec4_rtm The filled vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm filled(
            value_type value) noexcept
        {
            return generic_vec4_rtm(rtm::vector_set(value));
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec4_rtm The infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return generic_vec4_rtm The negative infinity vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm
        negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec4_rtm The NaN vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec4_rtm The zero vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec4_rtm The one vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The x axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm x_axis() noexcept
        {
            return generic_vec4_rtm(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The y axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm y_axis() noexcept
        {
            return generic_vec4_rtm(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The z axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm z_axis() noexcept
        {
            return generic_vec4_rtm(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the w component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The w axis vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm w_axis() noexcept
        {
            return generic_vec4_rtm(0, 0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_rtm The left vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The right vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_rtm The down vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The up vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec4_rtm The back vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec4_rtm The forward vector
         */
        MVM_INLINE_NODISCARD static generic_vec4_rtm forward() noexcept
        {
            return z_axis();
        }

    private:
        vector_type _value;
    };

    template <typename value_type, typename vector_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec4_rtm<value_type, vector_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", "
                  << v.w() << ")";
    }

    using vec4f32_rtm = generic_vec4_rtm<float, wrappers::v4f>;
    using vec4f64_rtm = generic_vec4_rtm<double, wrappers::v4d>;

    using vec4f_rtm = vec4f32_rtm;
    using vec4d_rtm = vec4f64_rtm;
}  // namespace move::vectormath

template <typename value_type, typename vector_type>
struct fmt::formatter<
    move::vectormath::generic_vec4_rtm<value_type, vector_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec4_rtm<value_type, vector_type> const&
            number,
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
    template <typename value_type, typename vector_type>
    struct adl_serializer<
        move::vectormath::generic_vec4_rtm<value_type, vector_type>>
        : public move::vectormath::json_serializer_template<
              move::vectormath::generic_vec4_rtm<value_type, vector_type>,
              move::vectormath::generic_vec4_rtm<value_type,
                  vector_type>::num_components>
    {
    };
}  // namespace nlohmann
#endif