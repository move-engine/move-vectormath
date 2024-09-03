#pragma once
#include <cstddef>
#include <iosfwd>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/meta/type_utils.hpp>

namespace move::math::scalar
{
    template <typename T>
    struct base_vec4
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
                T x, y, z, w;
            };
            T data[4];
        };

        // Constructors
    public:
        MVM_INLINE base_vec4() : x(0), y(0), z(0), w(0)
        {
        }

        MVM_INLINE base_vec4(const T& x, const T& y, const T& z, const T& w) :
            x(x), y(y), z(z), w(w)
        {
        }

        MVM_INLINE base_vec4(const base_vec4& other) :
            x(other.x), y(other.y), z(other.z), w(other.w)
        {
        }

        MVM_INLINE base_vec4& operator=(const base_vec4& other)
        {
            x = other.x;
            y = other.y;
            z = other.z;
            w = other.w;
            return *this;
        }

        // Arithmetic operators
    public:
        MVM_INLINE_NODISCARD base_vec4 operator+(const base_vec4& other) const
        {
            return base_vec4(x + other.x, y + other.y, z + other.z,
                             w + other.w);
        }

        MVM_INLINE_NODISCARD base_vec4 operator-(const base_vec4& other) const
        {
            return base_vec4(x - other.x, y - other.y, z - other.z,
                             w - other.w);
        }

        MVM_INLINE_NODISCARD base_vec4 operator*(const base_vec4& other) const
        {
            return base_vec4(x * other.x, y * other.y, z * other.z,
                             w * other.w);
        }

        MVM_INLINE_NODISCARD base_vec4 operator/(const base_vec4& other) const
        {
            return base_vec4(x / other.x, y / other.y, z / other.z,
                             w / other.w);
        }

        MVM_INLINE_NODISCARD base_vec4 operator+(const T& scalar) const
        {
            return base_vec4(x + scalar, y + scalar, z + scalar, w + scalar);
        }

        MVM_INLINE_NODISCARD base_vec4 operator-(const T& scalar) const
        {
            return base_vec4(x - scalar, y - scalar, z - scalar, w - scalar);
        }

        MVM_INLINE_NODISCARD base_vec4 operator*(const T& scalar) const
        {
            return base_vec4(x * scalar, y * scalar, z * scalar, w * scalar);
        }

        MVM_INLINE_NODISCARD base_vec4 operator/(const T& scalar) const
        {
            return base_vec4(x / scalar, y / scalar, z / scalar, w / scalar);
        }

        MVM_INLINE_NODISCARD base_vec4 operator-() const
        {
            return base_vec4(-x, -y, -z, -w);
        }

        MVM_INLINE base_vec4& operator+=(const base_vec4& other)
        {
            x += other.x;
            y += other.y;
            z += other.z;
            w += other.w;
            return *this;
        }

        MVM_INLINE base_vec4& operator-=(const base_vec4& other)
        {
            x -= other.x;
            y -= other.y;
            z -= other.z;
            w -= other.w;
        }

        MVM_INLINE base_vec4& operator*=(const base_vec4& other)
        {
            x *= other.x;
            y *= other.y;
            z *= other.z;
            w *= other.w;
            return *this;
        }

        MVM_INLINE base_vec4& operator/=(const base_vec4& other)
        {
            x /= other.x;
            y /= other.y;
            z /= other.z;
            w /= other.w;
            return *this;
        }

        MVM_INLINE base_vec4& operator+=(const T& scalar)
        {
            x += scalar;
            y += scalar;
            z += scalar;
            w += scalar;
            return *this;
        }

        MVM_INLINE base_vec4& operator-=(const T& scalar)
        {
            x -= scalar;
            y -= scalar;
            z -= scalar;
            w -= scalar;
            return *this;
        }

        MVM_INLINE base_vec4& operator*=(const T& scalar)
        {
            x *= scalar;
            y *= scalar;
            z *= scalar;
            w *= scalar;
            return *this;
        }

        MVM_INLINE base_vec4& operator/=(const T& scalar)
        {
            x /= scalar;
            y /= scalar;
            z /= scalar;
            w /= scalar;
            return *this;
        }

        // Stream overload operators for printing
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const base_vec4& vec)
        {
            os << move::meta::type_name<base_vec4>() << "(" << vec.x << ", "
               << vec.y << ", " << vec.z << ", " << vec.w << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator<(const base_vec4& other) const
        {
            return x < other.x && y < other.y && z < other.z && w < other.w;
        }

        MVM_INLINE_NODISCARD bool operator>(const base_vec4& other) const
        {
            return x > other.x && y > other.y && z > other.z && w > other.w;
        }

        MVM_INLINE_NODISCARD bool operator<=(const base_vec4& other) const
        {
            return x <= other.x && y <= other.y && z <= other.z && w <= other.w;
        }

        MVM_INLINE_NODISCARD bool operator>=(const base_vec4& other) const
        {
            return x >= other.x && y >= other.y && z >= other.z && w >= other.w;
        }

        MVM_INLINE_NODISCARD bool operator==(const base_vec4& other) const
        {
            return x == other.x && y == other.y && z == other.z && w == other.w;
        }

        MVM_INLINE_NODISCARD bool operator!=(const base_vec4& other) const
        {
            return x != other.x || y != other.y || z != other.z || w != other.w;
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD T& operator[](const std::size_t index)
        {
            return data[math::min<std::size_t>(index, 3)];
        }

        MVM_INLINE_NODISCARD const T& operator[](const std::size_t index) const
        {
            return data[math::min<std::size_t>(index, 3)];
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

        MVM_INLINE_NODISCARD T get_w() const
        {
            return w;
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

        MVM_INLINE void set_w(const T& value)
        {
            w = value;
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            archive(x, y, z, w);
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
            return x * x + y * y + z * z + w * w;
        }

        MVM_INLINE_NODISCARD T reciprocal_length() const
        {
            return math::sqrt_reciprocal(length_squared());
        }

        MVM_INLINE_NODISCARD base_vec4 normalized() const
        {
            return *this / length();
        }

        MVM_INLINE_NODISCARD T distance(const base_vec4& other) const
        {
            return (*this - other).length();
        }

        MVM_INLINE_NODISCARD T distance_squared(const base_vec4& other) const
        {
            return (*this - other).length_squared();
        }

        // Mutators
    public:
        MVM_INLINE base_vec4& normalize()
        {
            *this /= length();
            return *this;
        }

        MVM_INLINE base_vec4& fill(const T& val)
        {
            x = y = z = w = val;
            return *this;
        }

        MVM_INLINE base_vec4& set(const T& x,
                                  const T& y,
                                  const T& z,
                                  const T& w)
        {
            this->x = x;
            this->y = y;
            this->z = z;
            this->w = w;
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
            return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
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
        MVM_INLINE_NODISCARD static base_vec4 reflect(
            const base_vec4& incident, const base_vec4& normal) noexcept
        {
            auto dot = base_vec4::dot(incident, normal);
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
        MVM_INLINE_NODISCARD static base_vec4 refract(const base_vec4& inc,
                                                      const base_vec4& nrm,
                                                      T ior) noexcept
        {
            base_vec4 index = base_vec4(ior, ior, ior, ior);

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

            T dotinorm = base_vec4::dot(inc, nrm);
            base_vec4 roiPlusDotinorm = index * dotinorm;
            base_vec4 innerSqrt = base_vec4::filled(
                math::sqrt(1 - ior * ior * (1 - dotinorm * dotinorm)));
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
         * @return base_vec4 The result of the interpolation
         */
        MVM_INLINE_NODISCARD static base_vec4 lerp_unclamped(
            const base_vec4& v1, const base_vec4& v2, T t) noexcept
        {
            // Compute the clamp once
            return base_vec4(math::lerp_unclamped(v1.x, v2.x, t),
                             math::lerp_unclamped(v1.y, v2.y, t),
                             math::lerp_unclamped(v1.z, v2.z, t),
                             math::lerp_unclamped(v1.w, v2.w, t));
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
            return base_vec4(math::lerp_unclamped(v1.x, v2.x, t.x),
                             math::lerp_unclamped(v1.y, v2.y, t.y),
                             math::lerp_unclamped(v1.z, v2.z, t.z),
                             math::lerp_unclamped(v1.w, v2.w, t.w));
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
            return base_vec4(math::lerp(v1.x, v2.x, clamped),
                             math::lerp(v1.y, v2.y, clamped),
                             math::lerp(v1.z, v2.z, clamped),
                             math::lerp(v1.w, v2.w, clamped));
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
            return base_vec4(
                math::lerp(v1.x, v2.x, t.x), math::lerp(v1.y, v2.y, t.y),
                math::lerp(v1.z, v2.z, t.z), math::lerp(v1.w, v2.w, t.w));
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
            return base_vec4(math::min(v1.x, v2.x), math::min(v1.y, v2.y),
                             math::min(v1.z, v2.z), math::min(v1.w, v2.w));
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
            return base_vec4(math::max(v1.x, v2.x), math::max(v1.y, v2.y),
                             math::max(v1.z, v2.z), math::max(v1.w, v2.w));
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
                math::clamp(v.x, min.x, max.x), math::clamp(v.y, min.y, max.y),
                math::clamp(v.z, min.z, max.z), math::clamp(v.w, min.w, max.w));
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
            return base_vec4(
                math::clamp(v.x, min, max), math::clamp(v.y, min, max),
                math::clamp(v.z, min, max), math::clamp(v.w, min, max));
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
}  // namespace move::math::scalar