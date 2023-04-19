#pragma once

#include "fastvec3.hpp"

namespace move::vectormath
{
    struct vec3
    {
        using XMFLOAT3 = internal::DirectX::XMFLOAT3;

    public:
        inline vec3() noexcept : _value(0, 0, 0)
        {
        }

        inline vec3(float x, float y = 0, float z = 0) noexcept
            : _value(x, y, z)
        {
        }

        inline vec3(const XMFLOAT3& v) noexcept : _value(v)
        {
        }

        inline vec3(const fastvec3& v) noexcept : _value(v)
        {
        }

    public:
        inline fastvec3 fast() const
        {
            return _value;
        }

    public:
        inline operator XMFLOAT3&()
        {
            return _value;
        }

        inline bool operator==(const vec3& v) const noexcept
        {
            return fast() == v.fast();
        }

        inline bool operator!=(const vec3& v) const noexcept
        {
            return fast() != v.fast();
        }

        inline vec3& operator=(const vec3& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline vec3& operator+=(const vec3& v) noexcept
        {
            (fast() + v.fast()).store(_value);
            return *this;
        }

        inline vec3& operator-=(const vec3& v) noexcept
        {
            (fast() - v.fast()).store(_value);
            return *this;
        }

        inline vec3& operator*=(const vec3& v) noexcept
        {
            (fast() * v.fast()).store(_value);
            return *this;
        }

        inline vec3& operator/=(const vec3& v) noexcept
        {
            (fast() / v.fast()).store(_value);
            return *this;
        }

        inline vec3 operator+(const vec3& v) const noexcept
        {
            return vec3(fast() + v.fast());
        }

        inline vec3 operator-(const vec3& v) const noexcept
        {
            return vec3(fast() - v.fast());
        }

        inline vec3 operator*(const vec3& v) const noexcept
        {
            return vec3(fast() * v.fast());
        }

        inline vec3 operator/(const vec3& v) const noexcept
        {
            return vec3(fast() / v.fast());
        }

        inline vec3 operator-() const noexcept
        {
            return vec3(_value.x, _value.y, _value.z);
        }

        inline float operator[](int i) const noexcept
        {
            int index = i % 3;
            switch (index)
            {
                case 0:
                    return _value.x;
                case 1:
                    return _value.y;
                default:
                    return _value.z;
            }
            return 0;
        }

        inline float& operator[](int i) noexcept
        {
            int index = i % 3;
            switch (index)
            {
                case 0:
                    return _value.x;
                case 1:
                    return _value.y;
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

        inline vec3& set_x(float x) noexcept
        {
            _value.x = x;
            return *this;
        }

        inline vec3& set_y(float y) noexcept
        {
            _value.y = y;
            return *this;
        }

        inline vec3& set_z(float z) noexcept
        {
            _value.z = z;
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

        inline vec3 normalized() const noexcept
        {
            return fast().normalized();
        }

        inline fastvec3 normalized_approximate() const noexcept
        {
            return fast().normalized_approximate();
        }

        inline float dot(const vec3& v) const noexcept
        {
            return fast().dot(v._value);
        }

        inline float distance_to_point(const vec3& v) const noexcept
        {
            return fast().distance_to_point(v.fast());
        }

        inline float distance_to_point_approximate(const vec3& v) const noexcept
        {
            return fast().distance_to_point_approximate(v.fast());
        }

        inline float squared_distance_to_point(const vec3& v) const noexcept
        {
            return fast().squared_distance_to_point(v.fast());
        }

        inline float distance_to_line(
            const vec3& v0, const vec3& v1) const noexcept
        {
            return fast().distance_to_line(v0.fast(), v1.fast());
        }

        inline vec3 cross(const vec3& v) const noexcept
        {
            return fast().cross(v._value);
        }

        inline float angle_between_normals(const vec3& v) const noexcept
        {
            return fast().angle_between_normals(v.fast());
        }

        inline float angle_between_vectors(const vec3& v) const noexcept
        {
            return fast().angle_between_vectors(v.fast());
        }

        inline vec3 reflect(const vec3& v) const noexcept
        {
            return fast().reflect(v.fast());
        }

        inline vec3 refract(const vec3& v, float eta) const noexcept
        {
            return fast().refract(v.fast(), eta);
        }

        inline vec3 min(const vec3& v) const noexcept
        {
            return fast().min(v.fast());
        }

        inline vec3 max(const vec3& v) const noexcept
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
        inline static vec3 zero() noexcept
        {
            return vec3(0, 0, 0);
        }

        inline static vec3 one() noexcept
        {
            return vec3(1, 1, 1);
        }

        inline static vec3 x_axis() noexcept
        {
            return vec3(1, 0, 0);
        }

        inline static vec3 y_axis() noexcept
        {
            return vec3(0, 1, 0);
        }

        inline static vec3 z_axis() noexcept
        {
            return vec3(0, 0, 1);
        }

        inline static vec3 right() noexcept
        {
            return x_axis();
        }

        inline static vec3 up() noexcept
        {
            return y_axis();
        }

        inline static vec3 forward() noexcept
        {
            return z_axis();
        }

    private:
        XMFLOAT3 _value;
    };

    struct norm3 : public vec3
    {
        inline norm3(const vec3& v) noexcept : vec3(v.normalized())
        {
        }

        inline norm3(float x, float y, float z) noexcept
            : vec3(internal::DirectX::XMVector2Normalize(
                  internal::DirectX::XMVectorSet(x, y, z, 0.0f)))
        {
        }
    };
}  // namespace move::vectormath