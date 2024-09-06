#pragma once
#include <cstddef>
#include <iosfwd>

#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_common.hpp>
#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif

namespace move::math::simd_rtm
{
    namespace detail
    {
        MVM_TYPE_WRAPPER(v4f, rtm::vector4f)
        MVM_TYPE_WRAPPER(v4d, rtm::vector4d)

        template <typename T>
        using v4 = std::conditional_t<std::is_same_v<T, float>, v4f, v4d>;
    }  // namespace detail

    template <typename T, typename wrapper_type = detail::v4<T>>
    // requires std::is_floating_point_v<T>
    struct alignas(16) base_vec4
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;
        // Member variables
    private:
        using rtm_vec4_t = typename wrapper_type::type;
        rtm_vec4_t _value;

        // Constructors
    public:
        MVM_INLINE base_vec4() : _value(rtm::vector_zero())
        {
        }

        MVM_INLINE base_vec4(const T& x, const T& y, const T& z, const T& w) :
            _value(rtm::vector_set(x, y, z, w))
        {
        }

        MVM_INLINE base_vec4(const base_vec4& other) : _value(other._value)
        {
        }

        MVM_INLINE base_vec4(const rtm_vec4_t& other) : _value(other)
        {
        }

        MVM_INLINE base_vec4& operator=(const base_vec4& other)
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

        MVM_INLINE base_vec4& load_array(const T* src)
        {
            _value = rtm::vector_load(src);
            return *this;
        }

        MVM_INLINE_NODISCARD static base_vec4 from_array(const T* src)
        {
            base_vec4 result;
            result._value = rtm::vector_load(src);
            return result;
        }

        MVM_INLINE_NODISCARD rtm_vec4_t to_rtm() const
        {
            return _value;
        }

        MVM_INLINE_NODISCARD static base_vec4 from_rtm(const rtm_vec4_t& data)
        {
            base_vec4 result;
            result._value = data;
            return result;
        }

        // Arithmetic operators
    public:
        MVM_INLINE_NODISCARD base_vec4 operator+(const base_vec4& other) const
        {
            return base_vec4(rtm::vector_add(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec4 operator-(const base_vec4& other) const
        {
            return base_vec4(rtm::vector_sub(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec4 operator*(const base_vec4& other) const
        {
            return base_vec4(rtm::vector_mul(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec4 operator/(const base_vec4& other) const
        {
            return base_vec4(rtm::vector_div(_value, other._value));
        }

        MVM_INLINE_NODISCARD base_vec4 operator+(const T& scalar) const
        {
            return base_vec4(rtm::vector_add(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec4 operator-(const T& scalar) const
        {
            return base_vec4(rtm::vector_sub(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec4 operator*(const T& scalar) const
        {
            return base_vec4(rtm::vector_mul(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec4 operator/(const T& scalar) const
        {
            return base_vec4(rtm::vector_div(_value, scalar));
        }

        MVM_INLINE_NODISCARD base_vec4 operator-() const
        {
            return base_vec4(rtm::vector_neg(_value));
        }

        MVM_INLINE base_vec4& operator+=(const base_vec4& other)
        {
            _value = rtm::vector_add(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec4& operator-=(const base_vec4& other)
        {
            _value = rtm::vector_sub(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec4& operator*=(const base_vec4& other)
        {
            _value = rtm::vector_mul(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec4& operator/=(const base_vec4& other)
        {
            _value = rtm::vector_div(_value, other._value);
            return *this;
        }

        MVM_INLINE base_vec4& operator+=(const T& scalar)
        {
            _value = rtm::vector_add(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec4& operator-=(const T& scalar)
        {
            _value = rtm::vector_sub(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec4& operator*=(const T& scalar)
        {
            _value = rtm::vector_mul(_value, scalar);
            return *this;
        }

        MVM_INLINE base_vec4& operator/=(const T& scalar)
        {
            _value = rtm::vector_div(_value, scalar);
            return *this;
        }

        // Stream overload operators for printing
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const base_vec4& vec)
        {
#if defined(MVM_HAS_MOVE_CORE)
            os << move::meta::type_name<base_vec4>() << "("
#else
            os << "base_vec4("
#endif
               << vec.get_x() << ", " << vec.get_y() << ", " << vec.get_z()
               << ", " << vec.get_w() << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator<(const base_vec4& other) const
        {
            return rtm::mask_all_true(
                rtm::vector_less_than(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator>(const base_vec4& other) const
        {
            return rtm::mask_all_true(
                rtm::vector_greater_than(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator<=(const base_vec4& other) const
        {
            return rtm::mask_all_true(
                rtm::vector_less_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator>=(const base_vec4& other) const
        {
            return rtm::mask_all_true(
                rtm::vector_greater_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator==(const base_vec4& other) const
        {
            return rtm::mask_all_true(rtm::vector_equal(_value, other._value));
        }

        MVM_INLINE_NODISCARD bool operator!=(const base_vec4& other) const
        {
            return !rtm::mask_all_true(rtm::vector_equal(_value, other._value));
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD T operator[](const std::size_t index) const
        {
            return rtm::vector_get_component(_value, rtm::mix4(index));
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

        MVM_INLINE_NODISCARD T get_w() const
        {
            return rtm::vector_get_w(_value);
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

        MVM_INLINE void set_w(const T& value)
        {
            _value = rtm::vector_set_w(_value, value);
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[4];
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
            return rtm::vector_length(_value);
        }

        MVM_INLINE_NODISCARD T length_squared() const
        {
            return rtm::vector_length_squared(_value);
        }

        MVM_INLINE_NODISCARD T reciprocal_length() const
        {
            return rtm::vector_length_reciprocal(_value);
        }

        MVM_INLINE_NODISCARD base_vec4 normalized() const
        {
            return base_vec4(rtm::vector_normalize(_value));
        }

        MVM_INLINE_NODISCARD T distance(const base_vec4& other) const
        {
            return rtm::vector_length(rtm::vector_sub(_value, other._value));
        }

        MVM_INLINE_NODISCARD T distance_squared(const base_vec4& other) const
        {
            return rtm::vector_length_squared(
                rtm::vector_sub(_value, other._value));
        }

        // Mutators
    public:
        MVM_INLINE base_vec4& normalize()
        {
            _value = rtm::vector_normalize(_value);
            return *this;
        }

        MVM_INLINE base_vec4& fill(const T& val)
        {
            _value = rtm::vector_set(val);
            return *this;
        }

        MVM_INLINE base_vec4& set(const T& x,
                                  const T& y,
                                  const T& z,
                                  const T& w)
        {
            _value = rtm::vector_set(x, y, z, w);
            return *this;
        }

        MVM_INLINE base_vec4& set_zero()
        {
            return fill(0);
        }

        // Statics
    public:
        /**
         * @brief Calculates the dot product between two four-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The dot product
         */
        MVM_INLINE_NODISCARD static T dot(const base_vec4& v1,
                                          const base_vec4& v2) noexcept
        {
            return rtm::vector_dot(v1._value, v2._value);
        }

        /**
         * @brief Calculates the cross product between three four-dimensional
         * vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param v3 The third vector
         * @return base_vec4 The cross product
         */
        MVM_INLINE_NODISCARD static base_vec4 cross(
            const base_vec4& v1,
            const base_vec4& v2,
            const base_vec4& v3) noexcept
        {
            return {
                (((v2.get_z() * v3.get_w()) - (v2.get_w() * v3.get_z())) *
                 v1.get_y()) -
                    (((v2.get_y() * v3.get_w()) - (v2.get_w() * v3.get_y())) *
                     v1.get_z()) +
                    (((v2.get_y() * v3.get_z()) - (v2.get_z() * v3.get_y())) *
                     v1.get_w()),
                (((v2.get_w() * v3.get_z()) - (v2.get_z() * v3.get_w())) *
                 v1.get_x()) -
                    (((v2.get_w() * v3.get_x()) - (v2.get_x() * v3.get_w())) *
                     v1.get_z()) +
                    (((v2.get_z() * v3.get_x()) - (v2.get_x() * v3.get_z())) *
                     v1.get_w()),
                (((v2.get_y() * v3.get_w()) - (v2.get_w() * v3.get_y())) *
                 v1.get_x()) -
                    (((v2.get_x() * v3.get_w()) - (v2.get_w() * v3.get_x())) *
                     v1.get_y()) +
                    (((v2.get_x() * v3.get_y()) - (v2.get_y() * v3.get_x())) *
                     v1.get_w()),
                (((v2.get_z() * v3.get_y()) - (v2.get_y() * v3.get_z())) *
                 v1.get_x()) -
                    (((v2.get_z() * v3.get_x()) - (v2.get_x() * v3.get_z())) *
                     v1.get_y()) +
                    (((v2.get_y() * v3.get_x()) - (v2.get_x() * v3.get_y())) *
                     v1.get_z()),
            };
        }

        /**
         * @brief Returns the distance between two points
         *
         * @param p1 The first point
         * @param p2 The second point
         * @return T The distance between the two points
         */
        MVM_INLINE_NODISCARD static T distance_between_points(
            const base_vec4& p1, const base_vec4& p2) noexcept
        {
            return (p1 - p2).length();
        }

        /*
         * @brief Returns the distance between two points squared
         *
         * @param p1 The first point
         * @param p2 The second point
         */
        MVM_INLINE_NODISCARD static T distance_between_points_squared(
            const base_vec4& p1, const base_vec4& p2) noexcept
        {
            return (p1 - p2).length_squared();
        }

        /**
         * @brief Returns the angle between two normalizede vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static T angle_between_normalized_vectors(
            const base_vec4& v1, const base_vec4& v2) noexcept
        {
            return math::acos(dot(v1, v2));
        }

        /**
         * @brief Returns the angle between two unnormalized vectors
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return T The angle between the two vectors
         */
        MVM_INLINE_NODISCARD static T angle_between_vectors(
            const base_vec4& v1, const base_vec4& v2) noexcept
        {
            auto v1norm = rtm::vector_normalize(v1._value);
            auto v2norm = rtm::vector_normalize(v2._value);
            return math::acos(dot(v1norm, v2norm));
        }

        /**
         * @brief Reflects incident across normal and returns the result
         *
         * @param incident The incident vector
         * @param normal The normal vector
         */
        MVM_INLINE_NODISCARD static base_vec4 reflect(
            const base_vec4& incident, const base_vec4& normal) noexcept
        {
            using namespace rtm;
            const rtm_vec4_t& inc = incident._value;
            const rtm_vec4_t& nrm = normal._value;

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
        MVM_INLINE_NODISCARD static base_vec4 refract(const base_vec4& incident,
                                                      const base_vec4& normal,
                                                      T ior) noexcept
        {
            using namespace rtm;
            const rtm_vec4_t& inc = incident._value;
            const rtm_vec4_t& nrm = normal._value;
            const rtm_vec4_t& index = vector_set(ior);

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

            T dotinorm = vector_dot(inc, nrm);
            rtm_vec4_t roiPlusDotinorm = vector_mul(index, dotinorm);
            rtm_vec4_t innerSqrt = vector_set(
                scalar_sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
            ;

            return vector_sub(
                vector_mul(index, inc),
                vector_mul(nrm, vector_add(roiPlusDotinorm, innerSqrt)));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  Can be used for extrapolation as well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return base_vec4 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec4 lerp_unclamped(
            const base_vec4& v1, const base_vec4& v2, T t) noexcept
        {
            // Compute the clamp once
            return base_vec4(rtm::vector_lerp(v1._value, v2._value, t));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  Can be used for extrapolation as
         * well.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return base_vec4 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec4 lerp_unclamped(
            const base_vec4& v1,
            const base_vec4& v2,
            const base_vec4& t) noexcept
        {
            return base_vec4(rtm::vector_lerp(v1._value, v2._value, t._value));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * single interpolation value.  The interpolation value is clamped
         * between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation value
         * @return base_vec4 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec4 lerp(const base_vec4& v1,
                                                   const base_vec4& v2,
                                                   T t) noexcept
        {
            T clamped = math::saturate(t);
            return base_vec4(rtm::vector_lerp(v1._value, v2._value, clamped));
        }

        /**
         * @brief Returns the linear interpolation between two vectors using a
         * vector of interpolation values.  The interpolation values are
         * clamped between 0 and 1.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @param t The interpolation values
         * @return base_vec4 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec4 lerp(const base_vec4& v1,
                                                   const base_vec4& v2,
                                                   const base_vec4& t) noexcept
        {
            auto clamped = rtm::vector_clamp(t._value, rtm::vector_zero(),
                                             rtm::vector_set(T(1)));
            return base_vec4(rtm::vector_lerp(v1._value, v2._value, clamped));
        }

        /**
         * @brief Returns a vector containing the minimum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return base_vec4 The minimum vector
         */
        MVM_INLINE_NODISCARD static base_vec4 min(const base_vec4& v1,
                                                  const base_vec4& v2) noexcept
        {
            return base_vec4(rtm::vector_min(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector containing the maximum x, y, z, and w
         * components of the two vectors.
         *
         * @param v1 The first vector
         * @param v2 The second vector
         * @return base_vec4 The maximum vector
         */
        MVM_INLINE_NODISCARD static base_vec4 max(const base_vec4& v1,
                                                  const base_vec4& v2) noexcept
        {
            return base_vec4(rtm::vector_max(v1._value, v2._value));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum vectors.
         *
         * @param v The vector to clamp
         * @param min The minimum vector
         * @param max The maximum vector
         * @return base_vec4 The clamped vector
         */
        MVM_INLINE_NODISCARD static base_vec4 clamp(
            const base_vec4& v,
            const base_vec4& min,
            const base_vec4& max) noexcept
        {
            return base_vec4(
                rtm::vector_clamp(v._value, min._value, max._value));
        }

        /**
         * @brief Returns a vector the provided value clamped between the
         * provided minimum and maximum scalars.
         *
         * @param v The vector to clamp
         * @param min The minimum scalar
         * @param max The maximum scalar
         * @return base_vec4 The clamped vector
         */
        MVM_INLINE_NODISCARD static base_vec4 clamp(const base_vec4& v,
                                                    const T& min,
                                                    const T& max) noexcept
        {
            return base_vec4(rtm::vector_clamp(v._value, rtm::vector_set(min),
                                               rtm::vector_set(max)));
        }

        // Shorthand
    public:
        /**
         * @brief Returns a vector with all components set to the provided
         * value.
         *
         * @param value The value to fill the vector with
         * @return base_vec4 The filled vector
         */
        MVM_INLINE_NODISCARD static base_vec4 filled(T value) noexcept
        {
            return base_vec4(value, value, value, value);
        }

        /**
         * @brief Returns a vector with all components set to infinity.
         *
         * @return base_vec4 The infinity vector
         */
        MVM_INLINE_NODISCARD static base_vec4 infinity() noexcept
        {
            return filled(std::numeric_limits<T>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to negative
         * infinity.
         *
         * @return base_vec4 The negative infinity vector
         */
        MVM_INLINE_NODISCARD static base_vec4 negative_infinity() noexcept
        {
            return filled(-std::numeric_limits<T>::infinity());
        }

        /**
         * @brief Returns a vector with all components set to NaN.
         *
         * @return base_vec4 The NaN vector
         */
        MVM_INLINE_NODISCARD static base_vec4 nan() noexcept
        {
            return filled(std::numeric_limits<T>::quiet_NaN());
        }

        /**
         * @brief Returns a vector with all components set to zero.
         *
         * @return base_vec4 The zero vector
         */
        MVM_INLINE_NODISCARD static base_vec4 zero() noexcept
        {
            return filled(0);
        }

        /**
         * @brief Returns a vector with all components set to one.
         *
         * @return base_vec4 The one vector
         */
        MVM_INLINE_NODISCARD static base_vec4 one() noexcept
        {
            return filled(1);
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The x axis vector
         */
        MVM_INLINE_NODISCARD static base_vec4 x_axis() noexcept
        {
            return base_vec4(1, 0, 0);
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The y axis vector
         */
        MVM_INLINE_NODISCARD static base_vec4 y_axis() noexcept
        {
            return base_vec4(0, 1, 0);
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The z axis vector
         */
        MVM_INLINE_NODISCARD static base_vec4 z_axis() noexcept
        {
            return base_vec4(0, 0, 1);
        }

        /**
         * @brief Returns a vector with the w component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The w axis vector
         */
        MVM_INLINE_NODISCARD static base_vec4 w_axis() noexcept
        {
            return base_vec4(0, 0, 0, 1);
        }

        /**
         * @brief Returns a vector with the x component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec4 The left vector
         */
        MVM_INLINE_NODISCARD static base_vec4 left() noexcept
        {
            return -x_axis();
        }

        /**
         * @brief Returns a vector with the x component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The right vector
         */
        MVM_INLINE_NODISCARD static base_vec4 right() noexcept
        {
            return x_axis();
        }

        /**
         * @brief Returns a vector with the y component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec4 The down vector
         */
        MVM_INLINE_NODISCARD static base_vec4 down() noexcept
        {
            return -y_axis();
        }

        /**
         * @brief Returns a vector with the y component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The up vector
         */
        MVM_INLINE_NODISCARD static base_vec4 up() noexcept
        {
            return y_axis();
        }

        /**
         * @brief Returns a vector with the z component set to negative one and
         * all other components set to zero.
         *
         * @return base_vec4 The back vector
         */
        MVM_INLINE_NODISCARD static base_vec4 backward() noexcept
        {
            return -z_axis();
        }

        /**
         * @brief Returns a vector with the z component set to one and all
         * other components set to zero.
         *
         * @return base_vec4 The forward vector
         */
        MVM_INLINE_NODISCARD static base_vec4 forward() noexcept
        {
            return z_axis();
        }
    };
}  // namespace move::math::simd_rtm