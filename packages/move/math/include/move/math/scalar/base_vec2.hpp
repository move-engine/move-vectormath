#pragma once
#include <cstddef>
#include <iosfwd>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif

namespace move::math::scalar
{
    template <typename T>
    struct base_vec2
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
                T x, y;
            };
            T data[2];
        };

        // Constructors
    public:
        MVM_INLINE base_vec2() : x(0), y(0)
        {
        }

        MVM_INLINE base_vec2(const T& x, const T& y) : x(x), y(y)
        {
        }

        MVM_INLINE base_vec2(const base_vec2& other) : x(other.x), y(other.y)
        {
        }

        MVM_INLINE base_vec2& operator=(const base_vec2& other)
        {
            x = other.x;
            y = other.y;
            return *this;
        }

        // Pointers
    public:
        MVM_INLINE_NODISCARD T* to_array()
        {
            return data;
        }

        MVM_INLINE_NODISCARD const T* to_array() const
        {
            return data;
        }

        MVM_INLINE void store_array(T* dest) const
        {
            memcpy(dest, data, sizeof(data));
        }

        // Arithmetic operators
    public:
        MVM_INLINE_NODISCARD base_vec2 operator+(const base_vec2& other) const
        {
            return base_vec2(x + other.x, y + other.y);
        }

        MVM_INLINE_NODISCARD base_vec2 operator-(const base_vec2& other) const
        {
            return base_vec2(x - other.x, y - other.y);
        }

        MVM_INLINE_NODISCARD base_vec2 operator*(const base_vec2& other) const
        {
            return base_vec2(x * other.x, y * other.y);
        }

        MVM_INLINE_NODISCARD base_vec2 operator/(const base_vec2& other) const
        {
            return base_vec2(x / other.x, y / other.y);
        }

        MVM_INLINE_NODISCARD base_vec2 operator+(const T& scalar) const
        {
            return base_vec2(x + scalar, y + scalar);
        }

        MVM_INLINE_NODISCARD base_vec2 operator-(const T& scalar) const
        {
            return base_vec2(x - scalar, y - scalar);
        }

        MVM_INLINE_NODISCARD base_vec2 operator*(const T& scalar) const
        {
            return base_vec2(x * scalar, y * scalar);
        }

        MVM_INLINE_NODISCARD base_vec2 operator/(const T& scalar) const
        {
            return base_vec2(x / scalar, y / scalar);
        }

        MVM_INLINE_NODISCARD base_vec2 operator-() const
        {
            return base_vec2(-x, -y);
        }

        // Stream overload operators for printing
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const base_vec2& vec)
        {
#if defined(MVM_HAS_MOVE_CORE)
            os << move::meta::type_name<base_vec2>() << "("
#else
            os << "base_vec2("
#endif
               << vec.x << ", " << vec.y << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator<(const base_vec2& other) const
        {
            return x < other.x && y < other.y;
        }

        MVM_INLINE_NODISCARD bool operator>(const base_vec2& other) const
        {
            return x > other.x && y > other.y;
        }

        MVM_INLINE_NODISCARD bool operator<=(const base_vec2& other) const
        {
            return x <= other.x && y <= other.y;
        }

        MVM_INLINE_NODISCARD bool operator>=(const base_vec2& other) const
        {
            return x >= other.x && y >= other.y;
        }

        MVM_INLINE_NODISCARD bool operator==(const base_vec2& other) const
        {
            return x == other.x && y == other.y;
        }

        MVM_INLINE_NODISCARD bool operator!=(const base_vec2& other) const
        {
            return x != other.x || y != other.y;
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD T& operator[](const size_t& index)
        {
            return data[math::min<std::size_t>(index, 1)];
        }

        MVM_INLINE_NODISCARD const T& operator[](const size_t& index) const
        {
            return data[math::min<std::size_t>(index, 1)];
        }

        MVM_INLINE_NODISCARD T get_x() const
        {
            return x;
        }

        MVM_INLINE_NODISCARD T get_y() const
        {
            return y;
        }

        MVM_INLINE base_vec2& set_x(const T& x)
        {
            this->x = x;
            return *this;
        }

        MVM_INLINE base_vec2& set_y(const T& y)
        {
            this->y = y;
            return *this;
        }

        // Conversion operators
    public:
        MVM_INLINE_NODISCARD operator T*()
        {
            return data;
        }

        MVM_INLINE_NODISCARD operator const T*() const
        {
            return data;
        }

        // Mathematical operations
    public:
        MVM_INLINE_NODISCARD T length() const
        {
            return sqrt<T>(length_squared());
        }

        MVM_INLINE_NODISCARD T length_squared() const
        {
            return x * x + y * y;
        }

        MVM_INLINE_NODISCARD T reciprocal_length() const
        {
            return sqrt_reciprocal<T>(length_squared());
        }

        MVM_INLINE_NODISCARD base_vec2 normalized() const
        {
            return *this / length();
        }

        MVM_INLINE_NODISCARD T aspect_ratio() const
        {
            return x / y;
        }

        MVM_INLINE_NODISCARD T dot(const base_vec2& other) const
        {
            return x * other.x + y * other.y;
        }

        // Mutators
    public:
        MVM_INLINE base_vec2& normalize()
        {
            *this /= length();
            return *this;
        }

        MVM_INLINE base_vec2& fill(const T& value)
        {
            set_x(value);
            set_y(value);
            return *this;
        }

        MVM_INLINE base_vec2& set(const T& x, const T& y)
        {
            set_x(x);
            set_y(y);
            return *this;
        }

        MVM_INLINE base_vec2& set_zero()
        {
            return fill(0);
        }
    };
}  // namespace move::math::scalar