#pragma once

#include "fastvec4.hpp"

namespace move::vectormath
{
    struct vec4
    {
        using XMFLOAT4 = internal::DirectX::XMFLOAT4;

    public:
        inline vec4() noexcept : _value(0, 0, 0, 0)
        {
        }

        inline vec4(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _value(x, y, z, w)
        {
        }

        inline vec4(const XMFLOAT4& v) noexcept : _value(v)
        {
        }

        inline vec4(const fastvec4& v) noexcept : _value(v)
        {
        }

    public:
        inline fastvec4 fast() const
        {
            return _value;
        }

    public:
        inline operator XMFLOAT4&()
        {
            return _value;
        }

        inline bool operator==(const vec4& v) const noexcept
        {
            return fast() == v.fast();
        }

        inline bool operator!=(const vec4& v) const noexcept
        {
            return fast() != v.fast();
        }

        inline vec4& operator=(const vec4& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline vec4& operator+=(const vec4& v) noexcept
        {
            (fast() + v.fast()).store(_value);
            return *this;
        }

        inline vec4& operator-=(const vec4& v) noexcept
        {
            (fast() - v.fast()).store(_value);
            return *this;
        }

        inline vec4& operator*=(const vec4& v) noexcept
        {
            (fast() * v.fast()).store(_value);
            return *this;
        }

        inline vec4& operator/=(const vec4& v) noexcept
        {
            (fast() / v.fast()).store(_value);
            return *this;
        }

        inline vec4 operator+(const vec4& v) const noexcept
        {
            return vec4(fast() + v.fast());
        }

        inline vec4 operator-(const vec4& v) const noexcept
        {
            return vec4(fast() - v.fast());
        }

        inline vec4 operator*(const vec4& v) const noexcept
        {
            return vec4(fast() * v.fast());
        }

        inline vec4 operator/(const vec4& v) const noexcept
        {
            return vec4(fast() / v.fast());
        }

        inline vec4 operator-() const noexcept
        {
            return -fast();
        }

        inline float operator[](int i) const noexcept
        {
            int index = i % 4;
            switch (index)
            {
                case 0:
                    return _value.x;
                case 1:
                    return _value.y;
                case 2:
                    return _value.z;
                default:
                    return _value.w;
            }
            return 0;
        }

        inline float& operator[](int i) noexcept
        {
            int index = i % 4;
            switch (index)
            {
                case 0:
                    return _value.x;
                case 1:
                    return _value.y;
                case 2:
                    return _value.z;
                default:
                    return _value.z;
            }
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

        inline float& z() noexcept
        {
            return _value.z;
        }

        inline float& w() noexcept
        {
            return _value.w;
        }

        inline vec4& set_x(float x) noexcept
        {
            _value.x = x;
            return *this;
        }

        inline vec4& set_y(float y) noexcept
        {
            _value.y = y;
            return *this;
        }

        inline vec4& set_z(float z) noexcept
        {
            _value.z = z;
            return *this;
        }

        inline vec4& set_w(float w) noexcept
        {
            _value.w = w;
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

        inline vec4 normalized() const noexcept
        {
            return fast().normalized();
        }

        inline fastvec4 normalized_approximate() const noexcept
        {
            return fast().normalized_approximate();
        }

        inline float dot(const vec4& v) const noexcept
        {
            return fast().dot(v._value);
        }

        inline float distance_to_point(const vec4& v) const noexcept
        {
            return fast().distance_to_point(v.fast());
        }

        inline float distance_to_point_approximate(const vec4& v) const noexcept
        {
            return fast().distance_to_point_approximate(v.fast());
        }

        inline float squared_distance_to_point(const vec4& v) const noexcept
        {
            return fast().squared_distance_to_point(v.fast());
        }

        inline fastvec4 cross(const vec4& v0, const vec4& v1) const noexcept
        {
            return fast().cross(v0.fast(), v1.fast());
        }

        inline float angle_between_normals(const vec4& v) const noexcept
        {
            return fast().angle_between_normals(v.fast());
        }

        inline float angle_between_vectors(const vec4& v) const noexcept
        {
            return fast().angle_between_vectors(v.fast());
        }

        inline vec4 reflect(const vec4& v) const noexcept
        {
            return fast().reflect(v.fast());
        }

        inline vec4 refract(const vec4& v, float eta) const noexcept
        {
            return fast().refract(v.fast(), eta);
        }

        inline vec4 min(const vec4& v) const noexcept
        {
            return fast().min(v.fast());
        }

        inline vec4 max(const vec4& v) const noexcept
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
        inline static vec4 zero() noexcept
        {
            return vec4(0, 0, 0, 0);
        }

        inline static vec4 one() noexcept
        {
            return vec4(1, 1, 1, 1);
        }

        inline static vec4 x_axis() noexcept
        {
            return vec4(1, 0, 0);
        }

        inline static vec4 y_axis() noexcept
        {
            return vec4(0, 1, 0);
        }

        inline static vec4 z_axis() noexcept
        {
            return vec4(0, 0, 1);
        }

        inline static vec4 w_axis() noexcept
        {
            return vec4(0, 0, 0, 1);
        }

        inline static vec4 right() noexcept
        {
            return x_axis();
        }

        inline static vec4 up() noexcept
        {
            return y_axis();
        }

        inline static vec4 forward() noexcept
        {
            return z_axis();
        }

    private:
        XMFLOAT4 _value;
    };

    struct norm4 : public vec4
    {
        inline norm4(const vec4& v) noexcept : vec4(v.normalized())
        {
        }

        inline norm4(float x, float y, float z, float w) noexcept
            : vec4(internal::DirectX::XMVector2Normalize(
                  internal::DirectX::XMVectorSet(x, y, z, w)))
        {
        }
    };
}  // namespace move::vectormath