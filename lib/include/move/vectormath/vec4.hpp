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

    public:
        /**
         * @brief Construct a new generic_vec4 object
         *
         * @return generic_vec4 A new vector initialized to (0, 0, 0, 0)
         */
        RTM_FORCE_INLINE generic_vec4() noexcept
            : _value(rtm::vector_set(
                  value_type(0), value_type(0), value_type(0), value_type(0)))
        {
        }

        /**
         * @brief Construct a new generic_vec4 object
         *
         * @param x The x component
         * @param y The y component.  Defaults to 0.
         * @param z The z component.  Defaults to 0.
         * @param w The w component.  Defaults to 0.
         * @return generic_vec4 A new vector initialized to (x, y, z, w)
         */
        RTM_FORCE_INLINE generic_vec4(value_type x, value_type y = 0,
            value_type z = 0, value_type w = 0) noexcept
            : _value(rtm::vector_set(x, y, z, w))
        {
        }

        /**
         * @brief Construct a new generic_vec4 object using an RTM vector
         *
         * @param v The RTM vector
         * @return generic_vec4 A new vector initialized to v
         */
        RTM_FORCE_INLINE generic_vec4(const vector_type& v) noexcept : _value(v)
        {
        }

    public:
        /**
         * @brief Returns the internal RTM vector.  Useful for tight loops,
         * where RTM may outperform the higher level API.
         *
         * @return const vector_type& A reference to the internal RTM vector.
         */
        RTM_FORCE_INLINE const vector_type& get_internal() const noexcept
        {
            return _value;
        }

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
        RTM_FORCE_INLINE bool operator==(const generic_vec4& v) const noexcept
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
        RTM_FORCE_INLINE bool operator!=(const generic_vec4& v) const noexcept
        {
            return !rtm::mask_all_true(rtm::vector_equal(_value, v._value));
        }

        /**
         * @brief Assigns the contents of another vector to this vector
         *
         * @param v The other vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator=(const generic_vec4& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        /**
         * @brief Adds another vector to this vector and stores the result in
         * this vector
         *
         * @param v The other vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator+=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_add(_value, v._value);
            return *this;
        }

        /**
         * @brief Subtracts another vector from this vector and stores the
         * result in this vector
         *
         * @param v The other vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator-=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_sub(_value, v._value);
            return *this;
        }

        /**
         * @brief Multiplies this vector by a scalar and stores the result in
         * this vector
         *
         * @param v The scalar
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator*=(value_type v) noexcept
        {
            _value = rtm::vector_mul(_value, v);
            return *this;
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator*=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_mul(_value, v._value);
            return *this;
        }

        /**
         * @brief Divides this vector by a scalar and stores the result in this
         * vector
         *
         * @param v The scalar
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator/=(value_type v) noexcept
        {
            _value = rtm::vector_div(_value, rtm::vector_set(v));
            return *this;
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * stores the result in this vector
         *
         * @param v The other vector
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& operator/=(
            const generic_vec4& v) noexcept
        {
            _value = rtm::vector_div(_value, v._value);
            return *this;
        }

        /**
         * @brief Adds the other vector to this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4 The result of the addition
         */
        RTM_FORCE_INLINE generic_vec4 operator+(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_add(_value, v._value));
        }

        /**
         * @brief Subtracts a vector from this vector and returns the result.
         * Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4 The result of the subtraction
         */
        RTM_FORCE_INLINE generic_vec4 operator-(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_sub(_value, v._value));
        }

        /**
         * @brief Multiplies this vector by a scalar and returns the result.
         * Does not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4 The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec4 operator*(value_type v) const noexcept
        {
            return generic_vec4(rtm::vector_mul(_value, v));
        }

        /**
         * @brief Multiplies this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4 The result of the multiplication
         */
        RTM_FORCE_INLINE generic_vec4 operator*(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_mul(_value, v._value));
        }

        /**
         * @brief Divides this vector by a scalar and returns the result.  Does
         * not modify this vector.
         *
         * @param v The scalar
         * @return generic_vec4 The result of the division
         */
        RTM_FORCE_INLINE generic_vec4 operator/(value_type v) const noexcept
        {
            return generic_vec4(rtm::vector_div(_value, rtm::vector_set(v)));
        }

        /**
         * @brief Divides this vector by another vector component-wise and
         * returns the result.  Does not modify this vector.
         *
         * @param v The other vector
         * @return generic_vec4 The result of the division
         */
        RTM_FORCE_INLINE generic_vec4 operator/(
            const generic_vec4& v) const noexcept
        {
            return generic_vec4(rtm::vector_div(_value, v._value));
        }

        /**
         * @brief Negates the vector and returns the result.  Does not modify
         * this vector.
         *
         * @return generic_vec4 The result of the negation
         */
        RTM_FORCE_INLINE generic_vec4 operator-() const noexcept
        {
            return generic_vec4(rtm::vector_neg(_value));
        }

    public:
        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * returned.
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
                case 2:
                    return rtm::vector_get_z(_value);
                default:
                    return rtm::vector_get_w(_value);
            }
        }

        /**
         * @brief Sets the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * set.
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
        RTM_FORCE_INLINE value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        /**
         * @brief Returns the component at the specified index.  If the index is
         * out of range, the result is undefined.  Currently the w component is
         * returned.
         *
         * @param index The index of the component to return
         * @return value_type The component at the specified index
         */
        RTM_FORCE_INLINE value_type operator[](int i) noexcept
        {
            return get_component(i);
        }

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
         * @brief Returns the w component of the vector
         *
         * @return value_type The w component
         */
        RTM_FORCE_INLINE value_type w() const noexcept
        {
            return rtm::vector_get_w(_value);
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
         * @brief Sets the x component of the vector
         *
         * @param x The new x component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        /**
         * @brief Sets the y component of the vector
         *
         * @param y The new y component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        /**
         * @brief Sets the z component of the vector
         *
         * @param z The new z component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        /**
         * @brief Sets the w component of the vector
         *
         * @param w The new w component
         * @return generic_vec4& A reference to the vector
         */
        RTM_FORCE_INLINE generic_vec4& set_w(value_type w) noexcept
        {
            _value = rtm::vector_set_w(_value, w);
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

    public:
        /**
         * @brief Returns the length of the vector
         *
         * @return value_type The length of the vector
         */
        RTM_FORCE_INLINE value_type length() const noexcept
        {
            return rtm::vector_length(_value);
        }

        /**
         * @brief Returns the squared length of the vector
         *
         * @return value_type The squared length of the vector
         */
        RTM_FORCE_INLINE value_type squared_length() const noexcept
        {
            return rtm::vector_length_squared(_value);
        }

        /**
         * @brief Returns a normalized copy of the vector
         *
         * @return generic_vec4 A normalized copy of the vector
         */
        RTM_FORCE_INLINE generic_vec4 normalized() const noexcept
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
        RTM_FORCE_INLINE static value_type dot(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
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
        RTM_FORCE_INLINE static value_type distance_between_points(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
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
        RTM_FORCE_INLINE static value_type squared_distance_to_point(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
        {
            return (v2 - v1).squared_length();
        }

        /**
         * @brief Returns the cross product between three four-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param v3 The third vector
         * @return generic_vec4 The cross product
         */
        RTM_FORCE_INLINE static generic_vec4 cross(const generic_vec4& v1,
            const generic_vec4& v2, const generic_vec4& v3) noexcept
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
         * @brief Returns the angle between two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_normalized_vectors(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
        {
            return std::acos(rtm::vector_dot(v1._value, v2._value));
        }

        /**
         * @brief Returns the angle between two vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return value_type The angle between the two vectors
         */
        RTM_FORCE_INLINE static value_type angle_between_vectors(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
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
        RTM_FORCE_INLINE static generic_vec4 reflect(
            const generic_vec4& incident, const generic_vec4& normal) noexcept
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
        RTM_FORCE_INLINE static generic_vec4 refract(
            const generic_vec4& incident, const generic_vec4& normal,
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
         * @return generic_vec4 The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec4 lerp_unclamped(
            const generic_vec4& v1, const generic_vec4& v2,
            value_type t) noexcept
        {
            return generic_vec4(rtm::vector_lerp(v1._value, v2._value, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return generic_vec4 The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec4 lerp_unclamped(
            const generic_vec4& v1, const generic_vec4& v2,
            const generic_vec4& t) noexcept
        {
            return generic_vec4(
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
         * @return generic_vec4 The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec4 lerp(const generic_vec4& v1,
            const generic_vec4& v2, value_type t) noexcept
        {
            return generic_vec4(rtm::vector_lerp(v1._value, v2._value,
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
         * @return generic_vec4 The result of the interpolation
         */
        RTM_FORCE_INLINE static generic_vec4 lerp(const generic_vec4& v1,
            const generic_vec4& v2, const generic_vec4& t) noexcept
        {
            return generic_vec4(rtm::vector_lerp(v1._value, v2._value,
                rtm::vector_clamp(t._value, rtm::vector_set(value_type(0)),
                    vector_set(value_type(1)))));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4 The minimum vector
         */
        RTM_FORCE_INLINE static generic_vec4 min(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
        {
            return generic_vec4(rtm::vector_min(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return generic_vec4 The maximum vector
         */
        RTM_FORCE_INLINE static generic_vec4 max(
            const generic_vec4& v1, const generic_vec4& v2) noexcept
        {
            return generic_vec4(rtm::vector_max(v1._value, v2._value));
        }

    public:
        /**
         * @brief Returns a vector containing the absolute value of each
         */
        RTM_FORCE_INLINE void normalize() noexcept
        {
            // TODO: there's probably a faster way to do this?
            _value = normalized();
        }

    public:
        RTM_FORCE_INLINE static generic_vec4 filled(value_type value) noexcept
        {
            return generic_vec4(rtm::vector_set(value));
        }

        RTM_FORCE_INLINE static generic_vec4 infinity() noexcept
        {
            return filled(std::numeric_limits<value_type>::infinity());
        }

        RTM_FORCE_INLINE static generic_vec4 negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<value_type>::infinity());
        }

        RTM_FORCE_INLINE static generic_vec4 nan() noexcept
        {
            return filled(std::numeric_limits<value_type>::quiet_NaN());
        }

        RTM_FORCE_INLINE static generic_vec4 zero() noexcept
        {
            return filled(0);
        }

        RTM_FORCE_INLINE static generic_vec4 one() noexcept
        {
            return filled(1);
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

    public:
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