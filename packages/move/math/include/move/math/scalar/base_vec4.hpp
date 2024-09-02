#pragma once
#include <cstddef>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>

namespace move::math::scalar
{
    template <typename T>
    struct base_vec4
    {
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

        MVM_INLINE ~base_vec4()
        {
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

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD auto operator<=>(const base_vec4& other) const =
            default;

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

        // Mutators
    public:
        MVM_INLINE base_vec4& zero()
        {
            x = y = z = w = T(0);
            return *this;
        }

        MVM_INLINE base_vec4& normalize()
        {
            *this /= length();
            return *this;
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
    };
}  // namespace move::math::scalar