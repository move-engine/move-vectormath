#pragma once
#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_common.hpp>
#include <move/meta/type_utils.hpp>

namespace move::math::simd_rtm
{
    namespace detail
    {
        MVM_TYPE_WRAPPER(v3f, rtm::vector4f)
        MVM_TYPE_WRAPPER(v3d, rtm::vector4d)

        template <typename T>
        using v3 = std::conditional_t<std::is_same_v<T, float>, v3f, v3d>;
    }  // namespace detail

    template <typename T, typename wrapper_type = detail::v3<T>>
    // requires std::is_floating_point_v<T>
    struct alignas(16) base_vec3
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

        // Member variables
    private:
        using rtm_vec3_t = wrapper_type::type;
        rtm_vec3_t _value;

        // Constructors
    public:
        MVM_INLINE base_vec3() : _value(rtm::vector_zero())
        {
        }

        MVM_INLINE base_vec3(const T& x, const T& y = 0, const T& z = 0) :
            _value(rtm::vector_set(x, y, z))
        {
        }

        MVM_INLINE base_vec3(const base_vec3& other) : _value(other._value)
        {
        }

        MVM_INLINE base_vec3(const rtm_vec3_t& other) : _value(other)
        {
        }

        MVM_INLINE base_vec3& operator=(const base_vec3& other)
        {
            _value = other._value;
            return *this;
        }

        // Pointers
    public:
        MVM_INLINE void store_array(T* dest) const
        {
            rtm::vector_store(_value, dest);
        }

        MVM_INLINE base_vec3& load_array(const T* src)
        {
            _value = rtm::vector_load3(src);
            return *this;
        }

        MVM_INLINE_NODISCARD static base_vec3 from_array(const T* src)
        {
            base_vec3 result;
            result._value = rtm::vector_load3(src);
            return result;
        }

        MVM_INLINE_NODISCARD rtm_vec3_t to_rtm() const
        {
            return _value;
        }

        // Arithmetic operators
    public:
        MVM_INLINE_NODISCARD base_vec3 operator+(const base_vec3& other) const
        {
            return base_vec3(rtm::vector_add(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec3 operator-(const base_vec3& other) const
        {
            return base_vec3(rtm::vector_sub(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec3 operator*(const base_vec3& other) const
        {
            return base_vec3(rtm::vector_mul(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec3 operator/(const base_vec3& other) const
        {
            return base_vec3(rtm::vector_div(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec3 operator+(const T& scalar) const
        {
            return base_vec3(rtm::vector_add(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec3 operator-(const T& scalar) const
        {
            return base_vec3(rtm::vector_sub(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec3 operator*(const T& scalar) const
        {
            return base_vec3(rtm::vector_mul(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec3 operator/(const T& scalar) const
        {
            return base_vec3(rtm::vector_div(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec3 operator-() const
        {
            return base_vec3(rtm::vector_neg(_value));
        }

        MVM_INLINE base_vec3& operator+=(const base_vec3& other)
        {
            _value = rtm::vector_add(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec3& operator-=(const base_vec3& other)
        {
            _value = rtm::vector_sub(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec3& operator*=(const base_vec3& other)
        {
            _value = rtm::vector_mul(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec3& operator/=(const base_vec3& other)
        {
            _value = rtm::vector_div(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec3& operator+=(const T& scalar)
        {
            _value = rtm::vector_add(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec3& operator-=(const T& scalar)
        {
            _value = rtm::vector_sub(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec3& operator*=(const T& scalar)
        {
            _value = rtm::vector_mul(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec3& operator/=(const T& scalar)
        {
            _value = rtm::vector_div(_value, scalar);
            return *this;
        }

        // Stream overload operators for printing
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const base_vec3& vec)
        {
            os << move::meta::type_name<base_vec3>() << "(" << vec.get_x()
               << ", " << vec.get_y() << ", " << vec.get_z() << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator<(const base_vec3& other) const
        {
            return rtm::mask_all_true3(
                rtm::vector_less_than(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator>(const base_vec3& other) const
        {
            return rtm::mask_all_true3(
                rtm::vector_greater_than(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator<=(const base_vec3& other) const
        {
            return rtm::mask_all_true3(
                rtm::vector_less_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator>=(const base_vec3& other) const
        {
            return rtm::mask_all_true3(
                rtm::vector_greater_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator==(const base_vec3& other) const
        {
            return rtm::mask_all_true3(rtm::vector_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator!=(const base_vec3& other) const
        {
            return !rtm::mask_all_true3(
                rtm::vector_equal(_value, other._value));
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD const T operator[](const std::size_t index) const
        {
            return rtm::vector_get_component(
                _value, rtm::mix4(math::min<std::size_t>(index, 2)));
        }

        MVM_INLINE_NODISCARD T get_x() const
        {
            return rtm::vector_get_x(_value);
        }

        MVM_INLINE_NODISCARD T get_y() const
        {
            return rtm::vector_get_y(_value);
        }

        MVM_INLINE_NODISCARD T get_z() const
        {
            return rtm::vector_get_z(_value);
        }

        MVM_INLINE void set_x(const T& value)
        {
            _value = rtm::vector_set_x(_value, value);
        }

        MVM_INLINE void set_y(const T& value)
        {
            _value = rtm::vector_set_y(_value, value);
        }

        MVM_INLINE void set_z(const T& value)
        {
            _value = rtm::vector_set_z(_value, value);
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[3];
            if constexpr (Archive::is_loading::value)
            {
                archive(data);
                _value = rtm::vector_load(data);
            }
            else
            {
                store_array(data);
                archive(data);
            }
        }

        // Mathematical operations
    public:
        MVM_INLINE_NODISCARD T length() const
        {
            return rtm::vector_length3(_value);
        }

        MVM_INLINE_NODISCARD T length_squared() const
        {
            return rtm::vector_length_squared3(_value);
        }

        MVM_INLINE_NODISCARD T reciprocal_length() const
        {
            return rtm::vector_length_reciprocal3(_value);
        }

        MVM_INLINE_NODISCARD base_vec3 normalized() const
        {
            return base_vec3(rtm::vector_normalize3(_value));
        }

        MVM_INLINE_NODISCARD T distance(const base_vec3& other) const
        {
            return rtm::vector_distance3(_value, other._value);
        }

        MVM_INLINE_NODISCARD T distance_squared(const base_vec3& other) const
        {
            return rtm::vector_length_squared3(
                rtm::vector_sub(_value, other._value));
        }

        // Mutators
    public:
        MVM_INLINE base_vec3& normalize()
        {
            _value = rtm::vector_normalize3(_value);
            return *this;
        }

        MVM_INLINE base_vec3& fill(const T& val)
        {
            _value = rtm::vector_set(val, val, val, T(0));
            return *this;
        }

        MVM_INLINE base_vec3& set(const T& x, const T& y, const T& z)
        {
            _value = rtm::vector_set(x, y, z, T(0));
            return *this;
        }

        MVM_INLINE base_vec3& set_zero()
        {
            return fill(T(0));
        }

        // Statics
    public:
        /**
         * @brief Calculates the dot product between two three-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The dot product
         */
        MVM_INLINE_NODISCARD static T dot(const base_vec3& v1,
                                          const base_vec3& v2) noexcept
        {
            return T(rtm::vector_dot3(v1._value, v2._value));
        }

        /**
         * @brief Calculates the cross product between two three-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param v3 The third vector
         * @return base_vec3 The cross product
         */
        MVM_INLINE_NODISCARD static base_vec3 cross(
            const base_vec3& v1, const base_vec3& v2) noexcept
        {
            return base_vec3(rtm::vector_cross3(v1._value, v2._value));
        }

        /**
         * @brief Returns the distance between two points
         *
         * @param p1 The first point
         * @param p2 The second point
         * @return T The distance between the two points
         */
        MVM_INLINE_NODISCARD static T distance_between_points(
            const base_vec3& p1, const base_vec3& p2) noexcept
        {
            return rtm::vector_distance3(p1._value, p2._value);
        }

        /*
         * @brief Returns the distance between two points squared
         *
         * @param p1 The first point
         * @param p2 The second point
         */
        MVM_INLINE_NODISCARD static T distance_between_points_squared(
            const base_vec3& p1, const base_vec3& p2) noexcept
        {
            return rtm::vector_length_squared3(
                rtm::vector_sub(p1._value, p2._value));
        }

        /**
         * @brief Returns the angle between two normalizede vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static T angle_between_normalized_vectors(
            const base_vec3& v1, const base_vec3& v2) noexcept
        {
            return math::acos(T(rtm::vector_dot3(v1._value, v2._value)));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static T angle_between_vectors(
            const base_vec3& v1, const base_vec3& v2) noexcept
        {
            auto v1norm = rtm::vector_normalize3(v1._value);
            auto v2norm = rtm::vector_normalize3(v2._value);
            return math::acos(T(rtm::vector_dot3(v1norm, v2norm)));
        }

        /**
         * @brief Reflects incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         */
        MVM_INLINE_NODISCARD static base_vec3 reflect(
            const base_vec3& incident, const base_vec3& normal) noexcept
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
        MVM_INLINE_NODISCARD static base_vec3 refract(const base_vec3& incident,
                                                      const base_vec3& normal,
                                                      T ior) noexcept
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
         * @return base_vec3 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec3 lerp_unclamped(
            const base_vec3& v1, const base_vec3& v2, T t) noexcept
        {
            return base_vec3(rtm::vector_lerp(v1._value, v2._value, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return base_vec3 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec3 lerp_unclamped(
            const base_vec3& v1,
            const base_vec3& v2,
            const base_vec3& t) noexcept
        {
            return base_vec3(rtm::vector_lerp(v1._value, v2._value, t._value));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  The interpolation value is clamped
         * between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return base_vec3 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec3 lerp(const base_vec3& v1,
                                                   const base_vec3& v2,
                                                   T t) noexcept
        {
            T clamped = math::saturate(t);
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
         * @return base_vec3 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec3 lerp(const base_vec3& v1,
                                                   const base_vec3& v2,
                                                   const base_vec3& t) noexcept
        {
            return base_vec3(
                rtm::vector_lerp(v1.to_rtm(), v2.to_rtm(),
                                 rtm::vector_clamp(t._value, rtm::vector_zero(),
                                                   rtm::vector_set(T(1)))));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return base_vec3 The minimum vector
         */
        MVM_INLINE_NODISCARD static base_vec3 min(const base_vec3& v1,
                                                  const base_vec3& v2) noexcept
        {
            return base_vec3(rtm::vector_min(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return base_vec3 The maximum vector
         */
        MVM_INLINE_NODISCARD static base_vec3 max(const base_vec3& v1,
                                                  const base_vec3& v2) noexcept
        {
            return base_vec3(rtm::vector_max(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return base_vec3 The clamped vector
         */
        MVM_INLINE_NODISCARD static base_vec3 clamp(
            const base_vec3& v,
            const base_vec3& min,
            const base_vec3& max) noexcept
        {
            return base_vec3(
                rtm::vector_clamp(v._value, min._value, max._value));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum scalars.
         *
         * @param v The vector to clamp
         * @param min The minimum scalar
         * @param max The maximum scalar
         * @return base_vec3 The clamped vector
         */
        MVM_INLINE_NODISCARD static base_vec3 clamp(const base_vec3& v,
                                                    const T& min,
                                                    const T& max) noexcept
        {
            return base_vec3(rtm::vector_clamp(v._value, rtm::vector_set(min),
                                               rtm::vector_set(max)));
        }

        // Shorthand
    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to fill the vector with
         * @return base_vec3 The filled vector
         */
        MVM_INLINE_NODISCARD static base_vec3 filled(T value) noexcept
        {
            return base_vec3(rtm::vector_set(value, value, value, T(0)));
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return base_vec3 The infinity vector
         */
        MVM_INLINE_NODISCARD static base_vec3 infinity() noexcept
        {
            return filled(std::numeric_limits<T>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return base_vec3 The negative infinity vector
         */
        MVM_INLINE_NODISCARD static base_vec3 negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<T>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return base_vec3 The NaN vector
         */
        MVM_INLINE_NODISCARD static base_vec3 nan() noexcept
        {
            return filled(std::numeric_limits<T>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return base_vec3 The zero vector
         */
        MVM_INLINE_NODISCARD static base_vec3 zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return base_vec3 The one vector
         */
        MVM_INLINE_NODISCARD static base_vec3 one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The x axis vector
         */
        MVM_INLINE_NODISCARD static base_vec3 x_axis() noexcept
        {
            return base_vec3(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The y axis vector
         */
        MVM_INLINE_NODISCARD static base_vec3 y_axis() noexcept
        {
            return base_vec3(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The z axis vector
         */
        MVM_INLINE_NODISCARD static base_vec3 z_axis() noexcept
        {
            return base_vec3(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec3 The left vector
         */
        MVM_INLINE_NODISCARD static base_vec3 left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The right vector
         */
        MVM_INLINE_NODISCARD static base_vec3 right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec3 The down vector
         */
        MVM_INLINE_NODISCARD static base_vec3 down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The up vector
         */
        MVM_INLINE_NODISCARD static base_vec3 up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec3 The back vector
         */
        MVM_INLINE_NODISCARD static base_vec3 backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return base_vec3 The forward vector
         */
        MVM_INLINE_NODISCARD static base_vec3 forward() noexcept
        {
            return z_axis();
        }
    };
}  // namespace move::math::simd_rtm