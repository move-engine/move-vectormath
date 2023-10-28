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
    /**
     * @brief A three component vector that can store either floats or doubles.
     * The underlying implementation is RTM's vector4f or vector4d.
     * @note Instances of this class must be aligned to 16 bytes.
     *
     * @tparam value_type The type of the components of the vector
     * @tparam vector_type_raw The underlying RTM vector type
     */
    template <typename value_type, typename vector_type_raw>
    struct generic_vec3_rtm
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
        constexpr static uint32_t num_components = 3;

    public:
        /**
         * @brief Construct a new generic_vec3_rtm object
         *
         * @return generic_vec3_rtm A new vector initialized to (0, 0, 0)
         */
        RTM_FORCE_INLINE generic_vec3_rtm() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        /**
         * @brief Construct a new generic_vec3_rtm object
         *
         * @param x The x component of the vector
         * @param y The y component of the vector
         * @param z The z component of the vector
         * @return generic_vec3_rtm A new vector initialized to (x, y, z, 0)
         */
        RTM_FORCE_INLINE generic_vec3_rtm(
            value_type x, value_type y = 0, value_type z = 0) noexcept
            : _value(rtm::vector_set(x, y, z, 0))
        {
        }

        /**
         * @brief Constructs a new generic_vec3_rtm object from an RTM vector
         * without clearing the w component.  The bool is used to differentiate
         * this constructor from the one that takes a vector_type and clears
         * the w component.
         *
         * @param v The RTM vector to copy
         * @param _ A dummy bool that is not used
         * @return generic_vec3_rtm A new vector initialized to v
         */
        RTM_FORCE_INLINE generic_vec3_rtm(const vector_type& v, bool _) noexcept
            : _value(v)
        {
        }

        /**
         * @brief Construct a new generic_vec3_rtm object.  Sets the w component
         * of the vector to 0.  To create a generic_vec3_rtm without clearing
         * the w component, use the constructor that takes a vector_type and a
         * bool.
         *
         * @param v The RTM vector to copy.
         * @return generic_vec3_rtm A new vector initialized to v
         */
        RTM_FORCE_INLINE generic_vec3_rtm(const vector_type& v) noexcept
            : _value(rtm::vector_set_w(v, 0))
        {
        }

    public:
        /**
         * @brief Returns the internal RTM vector.  Useful for tight loops,
         * where RTM may outperform the higher level API.
         *
         * @return const vector_type& A reference to the internal RTM vector.
         */
        RTM_FORCE_INLINE vector_type get_internal() const noexcept
        {
            return _value;
        }

        /**
         * @brief Returns the internal RTM vector.  Useful for tight loops,
         * where RTM may outperform the higher level API.
         *
         * @return const vector_type& A reference to the internal RTM vector.
         */
        RTM_FORCE_INLINE vector_type& get_internal() noexcept
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
         * @brief Returns a reference to the vector as a RTM vector.  This both
         * enables the compiler to do additional optimizations, as well as
         * allowing you to use the vector in functions that take RTM vectors
         * directly.
         *
         * @return vector_type& A reference to the RTM vector
         */
        RTM_FORCE_INLINE operator vector_type&()
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
        RTM_FORCE_INLINE operator const vector_type&() const
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
        RTM_FORCE_INLINE bool operator==(
            const generic_vec3_rtm& v) const noexcept
        {
            return rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        /**
         * @brief Determines if the vector is not equal to another vector
         *
         * @param v The other vector
         * @return true The vectors are not equal
         * @return false The vectors are equal
         */
        RTM_FORCE_INLINE bool operator!=(
            const generic_vec3_rtm& v) const noexcept
        {
            return !rtm::mask_all_true3(rtm::vector_equal(_value, v._value));
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator=(
            const generic_vec3_rtm& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator+=(
            const generic_vec3_rtm& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator-=(
            const generic_vec3_rtm& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator*=(value_type v) noexcept
        {
            _value = rtm::vector_mul(_value, v);
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator*=(
            const generic_vec3_rtm& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator/=(value_type v) noexcept
        {
            _value = rtm::vector_div(_value, rtm::vector_set(v));
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& operator/=(
            const generic_vec3_rtm& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_rtm The result of the addition
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator+(
            const generic_vec3_rtm& v) const noexcept
        {
            return generic_vec3_rtm(rtm::vector_add(_value, v._value));
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_rtm The result of the subtraction
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator-(
            const generic_vec3_rtm& v) const noexcept
        {
            return generic_vec3_rtm(rtm::vector_sub(_value, v._value));
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec3_rtm The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator*(value_type v) const noexcept
        {
            return generic_vec3_rtm(rtm::vector_mul(_value, v));
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_rtm The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator*(
            const generic_vec3_rtm& v) const noexcept
        {
            return generic_vec3_rtm(rtm::vector_mul(_value, v._value));
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec3_rtm The result of the division
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator/(value_type v) const noexcept
        {
            return generic_vec3_rtm(
                rtm::vector_div(_value, rtm::vector_set(v)));
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec3_rtm The result of the division
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator/(
            const generic_vec3_rtm& v) const noexcept
        {
            return generic_vec3_rtm(rtm::vector_div(_value, v._value));
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec3_rtm The negated vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm operator-() const noexcept
        {
            return generic_vec3_rtm(rtm::vector_neg(_value));
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

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.
         * @note Currently the w component is set if the index is out of range.
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
                case 1:
                    set_y(value);
                    break;
                case 2:
                    set_z(value);
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
            return rtm::vector_get_x(_value);
        }

        /**
         * @brief Returns the y component of the vector
         *
         * @return value_type The y component
         */
        RTM_FORCE_INLINE value_type y() const noexcept
        {
            return rtm::vector_get_y(_value);
        }

        /**
         * @brief Returns the z component of the vector
         *
         * @return value_type The z component
         */
        RTM_FORCE_INLINE value_type z() const noexcept
        {
            return rtm::vector_get_z(_value);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& x(value_type x) noexcept
        {
            return set_x(x);
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& y(value_type y) noexcept
        {
            return set_y(y);
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& z(value_type z) noexcept
        {
            return set_z(z);
        }

        /**
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        /**
         * @brief Sets all components of the vector to the same value
         *
         * @param value The new value for all components
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& fill(component_type value) noexcept
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
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& set(
            value_type x, value_type y, value_type z) noexcept
        {
            _value = rtm::vector_set(x, y, z, 0);
            return *this;
        }

        /**
         * @brief Sets the x, y and z components of the vector
         *
         * @param v A vector containing the new x, y and z components
         * @return generic_vec3_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm& set(const vector_type& v) noexcept
        {
            _value = v;
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
         * @return generic_vec3_rtm The normalized vector
         */
        RTM_FORCE_INLINE generic_vec3_rtm normalized() const noexcept
        {
            return rtm::vector_normalize3(_value);
        }

    public:
        /**
         * @brief Returns the dot product of two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        RTM_FORCE_INLINE static value_type dot(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
        {
            return rtm::vector_dot3(v1._value, v2._value);
        }

        /**
         * @brief Returns the euclidean distance between two vectors as if they
         * were points in four dimensional space.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         */
        RTM_FORCE_INLINE static value_type distance_between_points(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
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
        RTM_FORCE_INLINE static value_type squared_distance_between_points(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
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
        RTM_FORCE_INLINE static value_type distance_to_line(
            const generic_vec3_rtm& point, const generic_vec3_rtm& v0,
            const generic_vec3_rtm& v1) noexcept
        {
            // Implementation based on XMVector3LinePointDistance
            using namespace rtm;
            const vector_type& pt = point._value;
            const vector_type& line_pt1 = v0._value;
            const vector_type& line_pt2 = v1._value;

            const auto point_vector = vector_sub(pt, line_pt1);
            const auto line_vector = vector_sub(line_pt2, line_pt1);
            const auto length_sq = vector_length_squared3(line_vector);
            const auto point_projection_scale =
                vector_div(vector_dot3(point_vector, line_vector), length_sq);
            const auto distance_vector = vector_sub(
                point_vector, vector_mul(line_vector, point_projection_scale));
            return vector_length3(distance_vector);
        }

        /**
         * @brief Calculates the cross product of two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_rtm The cross product of the two vectors
         */
        RTM_FORCE_INLINE static generic_vec3_rtm cross(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
        {
            return rtm::vector_cross3(v1._value, v2._value);
        }

        /**
         * @brief Returns the angle between two normalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_normalized_vectors(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
        {
            return std::acos(rtm::vector_dot3(v1._value, v2._value));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_vectors(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
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
        RTM_FORCE_INLINE static generic_vec3_rtm reflect(
            const generic_vec3_rtm& incident,
            const generic_vec3_rtm& normal) noexcept
        {
            // Based on XMVector3Reflect
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
        RTM_FORCE_INLINE static generic_vec3_rtm refract(
            const generic_vec3_rtm& incident, const generic_vec3_rtm& normal,
            value_type ior) noexcept
        {
            // Based on XMVector3Refract
            using namespace rtm;
            const vector_type& inc = incident._value;
            const vector_type& nrm = normal._value;
            const vector_type& index = vector_set(ior);

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
         * @return generic_vec3_rtm The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec3_rtm lerp_unclamped(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2,
            value_type t) noexcept
        {
            return generic_vec3_rtm(rtm::vector_lerp(v1._value, v2._value, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec3_rtm The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec3_rtm lerp_unclamped(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2,
            const generic_vec3_rtm& t) noexcept
        {
            return generic_vec3_rtm(
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
         * @return generic_vec3_rtm The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec3_rtm lerp(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2,
            value_type t) noexcept
        {
            return generic_vec3_rtm(rtm::vector_lerp(v1._value, v2._value,
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
         * @return generic_vec3_rtm The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec3_rtm lerp(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2,
            const generic_vec3_rtm& t) noexcept
        {
            return generic_vec3_rtm(rtm::vector_lerp(v1._value, v2._value,
                rtm::vector_clamp(t._value, rtm::vector_set(value_type(0)),
                    vector_set(value_type(1)))));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_rtm The minimum vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm min(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
        {
            return generic_vec3_rtm(rtm::vector_min(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec3_rtm The maximum vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm max(
            const generic_vec3_rtm& v1, const generic_vec3_rtm& v2) noexcept
        {
            return generic_vec3_rtm(rtm::vector_max(v1._value, v2._value));
        }

    public:
        /**
         * @brief Normalizes the vector
         * @note The behavior is undefined if the length of the vector is zero
         * @return generic_vec4_rtm& A reference to the vector
         */
        RTM_FORCE_INLINE void normalize() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized();
        }

    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to set all components to
         * @return generic_vec3_rtm The filled vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm filled(
            value_type value) noexcept
        {
            return generic_vec3_rtm(rtm::vector_set(value));
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return generic_vec3_rtm The infinity vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return generic_vec3_rtm The negative infinity vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<value_type>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return generic_vec3_rtm The NaN vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return generic_vec3_rtm The zero vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to negative one.
         *
         * @return generic_vec3_rtm The negative one vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm negative_one() noexcept
        {
            return filled(-1);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return generic_vec3_rtm The one vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The x axis vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm x_axis() noexcept
        {
            return generic_vec3_rtm(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The y axis vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm y_axis() noexcept
        {
            return generic_vec3_rtm(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The z axis vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm z_axis() noexcept
        {
            return generic_vec3_rtm(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_rtm The left vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The right vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_rtm The down vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The up vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return generic_vec3_rtm The backward vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return generic_vec3_rtm The forward vector
         */
        RTM_FORCE_INLINE static generic_vec3_rtm forward() noexcept
        {
            return z_axis();
        }

    private:
        vector_type _value;
    };

    using vec3f = generic_vec3_rtm<float, wrappers::v4f>;
    using vec3d = generic_vec3_rtm<double, wrappers::v4d>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec3 = vec3d;
#else
    using vec3 = vec3f;
#endif

    template <typename value_type, typename vector_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec3_rtm<value_type, vector_type>& v)
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
struct fmt::formatter<
    move::vectormath::generic_vec3_rtm<value_type, vector_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec3_rtm<value_type, vector_type> const&
            number,
        FormatContext& ctx)
    {
        return format_to(
            ctx.out(), "({}, {}, {})", number.x(), number.y(), number.z());
    }
};