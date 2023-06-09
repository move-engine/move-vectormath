#pragma once

#include "fastvec2.hpp"

namespace move::vectormath
{
    struct vec2
    {
        using XMFLOAT2 = internal::DirectX::XMFLOAT2;

    public:
        constexpr static uint32_t num_elements = 2;

        inline vec2() noexcept : _value(0, 0)
        {
        }

        inline vec2(float x, float y = 0) noexcept : _value(x, y)
        {
        }

        inline vec2(const XMFLOAT2& v) noexcept : _value(v)
        {
        }

        inline vec2(const fastvec2& v) noexcept : _value(v)
        {
        }

    public:
        inline fastvec2 fast() const
        {
            return _value;
        }

    public:
        inline bool operator==(const vec2& v) const noexcept
        {
            return _value.x == v._value.x && _value.y == v._value.y;
        }

        inline bool operator!=(const vec2& v) const noexcept
        {
            return _value.x != v._value.x || _value.y != v._value.y;
        }

        inline vec2& operator=(const vec2& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline vec2& operator+=(const vec2& v) noexcept
        {
            _value.x += v._value.x;
            _value.y += v._value.y;
            return *this;
        }

        inline vec2& operator-=(const vec2& v) noexcept
        {
            _value.x -= v._value.x;
            _value.y -= v._value.y;
            return *this;
        }

        inline vec2& operator*=(const vec2& v) noexcept
        {
            _value.x *= v._value.x;
            _value.y *= v._value.y;
            return *this;
        }

        inline vec2& operator/=(const vec2& v) noexcept
        {
            _value.x /= v._value.x;
            _value.y /= v._value.y;
            return *this;
        }

        inline vec2 operator+(const vec2& v) const noexcept
        {
            return vec2(_value.x + v._value.x, _value.y + v._value.y);
        }

        inline vec2 operator-(const vec2& v) const noexcept
        {
            return vec2(_value.x - v._value.x, _value.y - v._value.y);
        }

        inline vec2 operator*(const vec2& v) const noexcept
        {
            return vec2(_value.x * v._value.x, _value.y * v._value.y);
        }

        inline vec2 operator/(const vec2& v) const noexcept
        {
            return vec2(_value.x / v._value.x, _value.y / v._value.y);
        }

        inline vec2 operator-() const noexcept
        {
            return vec2(-_value.x, -_value.y);
        }

        inline float operator[](int i) const noexcept
        {
            return (i & (0x1)) == 0 ? _value.x : _value.y;
        }

        inline float& operator[](int i) noexcept
        {
            return (i & (0x1)) == 0 ? _value.x : _value.y;
        }

    public:
        inline float& x() noexcept
        {
            return _value.x;
        }

        inline float& y() noexcept
        {
            return _value.y;
        }

        inline float x() const noexcept
        {
            return _value.x;
        }

        inline float y() const noexcept
        {
            return _value.y;
        }

        inline vec2& set_x(float x) noexcept
        {
            _value.x = x;
            return *this;
        }

        inline vec2& set_y(float y) noexcept
        {
            _value.y = y;
            return *this;
        }

        inline float length() const noexcept
        {
            return fast().length();
        }

        inline float length_approximate() const noexcept
        {
            return fast().length_approximate();
        }

        inline float squared_length() const noexcept
        {
            return fast().squared_length();
        }

        inline vec2 normalized() const noexcept
        {
            return fast().normalized();
        }

        inline fastvec2 normalized_approximate() const noexcept
        {
            return fast().normalized_approximate();
        }

        inline float dot(const vec2& v) const noexcept
        {
            return fast().dot(v._value);
        }

        inline float distance_to_point(const vec2& v) const noexcept
        {
            return fast().distance_to_point(v.fast());
        }

        inline float distance_to_point_approximate(const vec2& v) const noexcept
        {
            return fast().distance_to_point_approximate(v.fast());
        }

        inline float distance_to_line(
            const vec2& v0, const vec2& v1) const noexcept
        {
            return fast().distance_to_line(v0.fast(), v1.fast());
        }

        inline float squared_distance_to_point(const vec2& v) const noexcept
        {
            return fast().squared_distance_to_point(v.fast());
        }

        inline vec2 cross(const vec2& v) const noexcept
        {
            return fast().cross(v._value);
        }

        inline float angle_between_normals(const vec2& v) const noexcept
        {
            return fast().angle_between_normals(v.fast());
        }

        inline float angle_between_vectors(const vec2& v) const noexcept
        {
            return fast().angle_between_vectors(v.fast());
        }

        inline vec2 reflect(const vec2& v) const noexcept
        {
            return fast().reflect(v.fast());
        }

        inline vec2 refract(const vec2& v, float eta) const noexcept
        {
            return fast().refract(v.fast(), eta);
        }

        inline vec2 min(const vec2& v) const noexcept
        {
            return fast().min(v.fast());
        }

        inline vec2 max(const vec2& v) const noexcept
        {
            return fast().max(v.fast());
        }

    public:
        inline void normalize() noexcept
        {
            fast().normalized().store(_value);
        }

        inline void normalize_approximate() noexcept
        {
            fast().normalized_approximate().store(_value);
        }

    public:
        inline static vec2 intersect_line(const vec2& line1point1,
            const vec2& line1point2, const vec2& line2point1,
            const vec2& line2point2) noexcept
        {
            return fastvec2::intersect_line(line1point1.fast(),
                line1point2.fast(), line2point1.fast(), line2point2.fast());
        }

        inline static vec2 zero() noexcept
        {
            return vec2(0.0f, 0.0f);
        }

        inline static vec2 one() noexcept
        {
            return vec2(1.0f, 1.0f);
        }

        inline static vec2 x_axis() noexcept
        {
            return vec2(1.0f, 0.0f);
        }

        inline static vec2 y_axis() noexcept
        {
            return vec2(0.0f, 1.0f);
        }

        inline static vec2 right() noexcept
        {
            return vec2(1.0f, 0.0f);
        }

        inline static vec2 left() noexcept
        {
            return vec2(-1.0f, 0.0f);
        }

        inline static vec2 up() noexcept
        {
            return vec2(0.0f, 1.0f);
        }

        inline static vec2 down() noexcept
        {
            return vec2(0.0f, -1.0f);
        }

    private:
        XMFLOAT2 _value;
    };

    struct norm2 : public vec2
    {
        inline norm2(const vec2& v) noexcept : vec2(v.normalized())
        {
        }

        inline norm2(float x, float y) noexcept
            : vec2(internal::DirectX::XMVector2Normalize(
                  internal::DirectX::XMVectorSet(x, y, 0.0f, 0.0f)))
        {
        }
    };

    struct ivec2
    {
    public:
        constexpr static uint32_t num_elements = 2;

        inline ivec2() noexcept : _x(0), _y(0)
        {
        }

        inline ivec2(int x, int y = 0) noexcept : _x(x), _y(y)
        {
        }

        inline ivec2(const ivec2& v) noexcept : _x(v._x), _y(v._y)
        {
        }

        inline ivec2(const vec2& v) noexcept
            : _x(static_cast<int>(v.x())), _y(static_cast<int>(v.y()))
        {
        }

    public:
        inline ivec2& operator=(const ivec2& v) noexcept
        {
            _x = v._x;
            _y = v._y;
            return *this;
        }

        inline ivec2& operator+=(const ivec2& v) noexcept
        {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        inline ivec2& operator-=(const ivec2& v) noexcept
        {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        inline ivec2& operator*=(const ivec2& v) noexcept
        {
            _x *= v._x;
            _y *= v._y;
            return *this;
        }

        inline ivec2& operator/=(const ivec2& v) noexcept
        {
            _x /= v._x;
            _y /= v._y;
            return *this;
        }

        inline ivec2& operator%=(const ivec2& v) noexcept
        {
            _x %= v._x;
            _y %= v._y;
            return *this;
        }

        inline ivec2& operator*=(int v) noexcept
        {
            _x *= v;
            _y *= v;
            return *this;
        }

        inline ivec2& operator/=(int v) noexcept
        {
            _x /= v;
            _y /= v;
            return *this;
        }

        inline ivec2& operator%=(int v) noexcept
        {
            _x %= v;
            _y %= v;
            return *this;
        }

        inline ivec2 operator-() const noexcept
        {
            return ivec2(-_x, -_y);
        }

        inline ivec2 operator+(const ivec2& v) const noexcept
        {
            return ivec2(_x + v._x, _y + v._y);
        }

        inline ivec2 operator-(const ivec2& v) const noexcept
        {
            return ivec2(_x - v._x, _y - v._y);
        }

        inline ivec2 operator*(const ivec2& v) const noexcept
        {
            return ivec2(_x * v._x, _y * v._y);
        }

        inline ivec2 operator/(const ivec2& v) const noexcept
        {
            return ivec2(_x / v._x, _y / v._y);
        }

        inline ivec2 operator%(const ivec2& v) const noexcept
        {
            return ivec2(_x % v._x, _y % v._y);
        }

        inline ivec2 operator*(int v) const noexcept
        {
            return ivec2(_x * v, _y * v);
        }

        inline ivec2 operator/(int v) const noexcept
        {
            return ivec2(_x / v, _y / v);
        }

        inline ivec2 operator%(int v) const noexcept
        {
            return ivec2(_x % v, _y % v);
        }

        inline bool operator==(const ivec2& v) const noexcept
        {
            return _x == v._x && _y == v._y;
        }

        inline bool operator!=(const ivec2& v) const noexcept
        {
            return _x != v._x || _y != v._y;
        }

        inline int operator[](int i) const noexcept
        {
            return (i & (0x1)) == 0 ? _x : _y;
        }

        inline int& operator[](int i) noexcept
        {
            return (i & (0x1)) == 0 ? _x : _y;
        }

    public:
        inline int& x()
        {
            return _x;
        }

        inline int& y()
        {
            return _y;
        }

        inline int x() const
        {
            return _x;
        }

        inline int y() const
        {
            return _y;
        }

        inline ivec2& set_x(int x)
        {
            _x = x;
            return *this;
        }

        inline ivec2& set_y(int y)
        {
            _y = y;
            return *this;
        }

    public:
        static inline ivec2 zero() noexcept
        {
            return ivec2(0, 0);
        }

        static inline ivec2 one() noexcept
        {
            return ivec2(1, 1);
        }

        static inline ivec2 x_axis() noexcept
        {
            return ivec2(1, 0);
        }

        static inline ivec2 y_axis() noexcept
        {
            return ivec2(0, 1);
        }

        static inline ivec2 right() noexcept
        {
            return ivec2(1, 0);
        }

        static inline ivec2 left() noexcept
        {
            return ivec2(-1, 0);
        }

        static inline ivec2 up() noexcept
        {
            return ivec2(0, 1);
        }

        static inline ivec2 down() noexcept
        {
            return ivec2(0, -1);
        }

    public:
        int _x, _y;
    };
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathcereal.hpp"
#include "vmathjson.hpp"

MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2);
MOVE_VECTORMATH_CEREAL_SERIALIZER(move::vectormath::vec2);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::norm2);
MOVE_VECTORMATH_CEREAL_SERIALIZER(move::vectormath::norm2);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::ivec2);
MOVE_VECTORMATH_CEREAL_SERIALIZER(move::vectormath::ivec2);
#endif