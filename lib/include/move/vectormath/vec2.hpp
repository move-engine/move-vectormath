#pragma once

#include "fastvec2.hpp"

namespace move::vectormath
{
    struct vec2
    {
        using XMFLOAT2 = internal::DirectX::XMFLOAT2;

    public:
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
}  // namespace move::vectormath