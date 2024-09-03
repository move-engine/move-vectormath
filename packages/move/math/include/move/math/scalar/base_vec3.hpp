#pragma once
#include <cstddef>
#include <iosfwd>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/meta/type_utils.hpp>

namespace move::math::scalar
{
    template <typename T>
    struct base_vec3
    {
    public:
        constexpr static auto acceleration = Acceleration::Scalar;
        constexpr static bool has_fields = true;
        constexpr static bool has_pointer_semantics = has_fields;
        // Member variables
    public:
        union
        {
            struct
            {
                T x, y, z;
            };
            T data[3];
        };

        // Constructors
    public:
        MVM_INLINE base_vec3() : x(0), y(0), z(0)
        {
        }

        MVM_INLINE base_vec3(const T& x, const T& y, const T& z) :
            x(x), y(y), z(z)
        {
        }

        MVM_INLINE base_vec3(const base_vec3& other) :
            x(other.x), y(other.y), z(other.z)
        {
        }

        MVM_INLINE base_vec3& operator=(const base_vec3& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            return *this;
        }

        // Arithmetic operators
    public:
        MVM_INLINE_NODISCARD base_vec3 operator+(const base_vec3& other) const
        {
            return base_vec3(x + other.x, y + other.y, z + other.z);
        }

        MVM_INLINE_NODISCARD base_vec3 operator-(const base_vec3& other) const
        {
            return base_vec3(x - other.x, y - other.y, z - other.z);
        }

        MVM_INLINE_NODISCARD base_vec3 operator*(const base_vec3& other) const
        {
            return base_vec3(x * other.x, y * other.y, z * other.z);
        }

        MVM_INLINE_NODISCARD base_vec3 operator/(const base_vec3& other) const
        {
            return base_vec3(x / other.x, y / other.y, z / other.z);
        }

        MVM_INLINE_NODISCARD base_vec3 operator+(const T& scalar) const
        {
            return base_vec3(x + scalar, y + scalar, z + scalar);
        }

        MVM_INLINE_NODISCARD base_vec3 operator-(const T& scalar) const
        {
            return base_vec3(x - scalar, y - scalar, z - scalar);
        }

        MVM_INLINE_NODISCARD base_vec3 operator*(const T& scalar) const
        {
            return base_vec3(x * scalar, y * scalar, z * scalar);
        }

        MVM_INLINE_NODISCARD base_vec3 operator/(const T& scalar) const
        {
            return base_vec3(x / scalar, y / scalar, z / scalar);
        }

        MVM_INLINE_NODISCARD base_vec3 operator-() const
        {
            return base_vec3(-x, -y, -z);
        }

        MVM_INLINE base_vec3& operator+=(const base_vec3& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            return *this;
        }

        MVM_INLINE base_vec3& operator-=(const base_vec3& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
        }

        MVM_INLINE base_vec3& operator*=(const base_vec3& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            return *this;
        }

        MVM_INLINE base_vec3& operator/=(const base_vec3& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            return *this;
        }

        MVM_INLINE base_vec3& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            return *this;
        }

        MVM_INLINE base_vec3& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            return *this;
        }

        MVM_INLINE base_vec3& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            return *this;
        }

        MVM_INLINE base_vec3& operator/=(const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            return *this;
        }

        // Stream overload operators for printing
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const base_vec3& vec)
        {
            os << move::meta::type_name<base_vec3>() << "(" << vec.x << ", "
               << vec.y << ", " << vec.z << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator<(const base_vec3& other) const
        {
            return x < other.x && y < other.y && z < other.z;
        }

        MVM_INLINE_NODISCARD bool operator>(const base_vec3& other) const
        {
            return x > other.x && y > other.y && z > other.z;
        }

        MVM_INLINE_NODISCARD bool operator<=(const base_vec3& other) const
        {
            return x <= other.x && y <= other.y && z <= other.z;
        }

        MVM_INLINE_NODISCARD bool operator>=(const base_vec3& other) const
        {
            return x >= other.x && y >= other.y && z >= other.z;
        }

        MVM_INLINE_NODISCARD bool operator==(const base_vec3& other) const
        {
            return x == other.x && y == other.y && z == other.z;
        }

        MVM_INLINE_NODISCARD bool operator!=(const base_vec3& other) const
        {
            return x != other.x || y != other.y || z != other.z;
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD T& operator[](const std::size_t index)
        {
            return data[math::min<std::size_t>(index, 2)];
        }

        MVM_INLINE_NODISCARD const T& operator[](const std::size_t index) const
        {
            return data[math::min<std::size_t>(index, 2)];
        }

        MVM_INLINE_NODISCARD T get_x() const
        {
            return x;
        }

        MVM_INLINE_NODISCARD T get_y() const
        {
            return y;
        }

        MVM_INLINE_NODISCARD T get_z() const
        {
            return z;
        }

        MVM_INLINE void set_x(const T& value)
        {
            x = value;
        }

        MVM_INLINE void set_y(const T& value)
        {
            y = value;
        }

        MVM_INLINE void set_z(const T& value)
        {
            z = value;
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            archive(x, y, z);
            return;
        }

        // Mathemtical operations
    public:
        MVM_INLINE_NODISCARD T length() const
        {
            return math::sqrt(length_squared());
        }

        MVM_INLINE_NODISCARD T length_squared() const
        {
            return x * x + y * y + z * z;
        }

        MVM_INLINE_NODISCARD T reciprocal_length() const
        {
            return math::sqrt_reciprocal(length_squared());
        }

        MVM_INLINE_NODISCARD base_vec3 normalized() const
        {
            return *this / length();
        }

        MVM_INLINE_NODISCARD T distance(const base_vec3& other) const
        {
            return (*this - other).length();
        }

        MVM_INLINE_NODISCARD T distance_squared(const base_vec3& other) const
        {
            return (*this - other).length_squared();
        }

        // Mutators
    public:
        MVM_INLINE base_vec3& normalize()
        {
            *this /= length();
            return *this;
        }

        MVM_INLINE base_vec3& fill(const T& val)
        {
            x = y = z = val;
            return *this;
        }

        MVM_INLINE base_vec3& set(const T& x, const T& y, const T& z)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            return *this;
        }

        MVM_INLINE base_vec3& set_zero()
        {
            return fill(0);
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
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z;
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
            return {T(v1.y * v2.z - v1.z * v2.y), T(v1.z * v2.x - v1.x * v2.z),
                    T(v1.x * v2.y - v1.y * v2.x)};
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
            return (p1 - p2).length();
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
            const base_vec3& v1, const base_vec3& v2) noexcept
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
            const base_vec3& v1, const base_vec3& v2) noexcept
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
            // Compute the clamp once
            return base_vec3(math::lerp_unclamped(v1.x, v2.x, t),
                             math::lerp_unclamped(v1.y, v2.y, t),
                             math::lerp_unclamped(v1.z, v2.z, t));
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
            return base_vec3(math::lerp_unclamped(v1.x, v2.x, t.x),
                             math::lerp_unclamped(v1.y, v2.y, t.y),
                             math::lerp_unclamped(v1.z, v2.z, t.z));
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
            return base_vec3(math::lerp(v1.x, v2.x, clamped),
                             math::lerp(v1.y, v2.y, clamped),
                             math::lerp(v1.z, v2.z, clamped));
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
            return base_vec3(math::lerp(v1.x, v2.x, t.x),
                             math::lerp(v1.y, v2.y, t.y),
                             math::lerp(v1.z, v2.z, t.z));
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
            return base_vec3(math::min(v1.x, v2.x), math::min(v1.y, v2.y),
                             math::min(v1.z, v2.z));
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
            return base_vec3(math::max(v1.x, v2.x), math::max(v1.y, v2.y),
                             math::max(v1.z, v2.z));
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
            return base_vec3(math::clamp(v.x, min.x, max.x),
                             math::clamp(v.y, min.y, max.y),
                             math::clamp(v.z, min.z, max.z));
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
            return base_vec3(math::clamp(v.x, min, max),
                             math::clamp(v.y, min, max),
                             math::clamp(v.z, min, max));
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
            return base_vec3(value, value, value);
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
}  // namespace move::math::scalar