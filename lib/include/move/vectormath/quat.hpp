#pragma once

#include "fastquat.hpp"

namespace move::vectormath
{
    struct quat
    {
        using XMFLOAT4 = internal::DirectX::XMFLOAT4;

    public:
        inline quat() noexcept : _value(0, 0, 0, 0)
        {
        }

        inline quat(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _value(x, y, z, w)
        {
        }

        inline quat(const XMFLOAT4& v) noexcept : _value(v)
        {
        }

        inline quat(const fastquat& v) noexcept : _value(v)
        {
        }

    public:
        inline fastquat fast() const
        {
            return _value;
        }

    public:
        inline operator XMFLOAT4&()
        {
            return _value;
        }

        inline bool operator==(const quat& v) const noexcept
        {
            return fast() == v.fast();
        }

        inline bool operator!=(const quat& v) const noexcept
        {
            return fast() != v.fast();
        }

        inline quat& operator=(const quat& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline quat& operator*=(const quat& v) noexcept
        {
            (fast() * v.fast()).store(_value);
            return *this;
        }

        inline quat operator*(const quat& v) const noexcept
        {
            return quat(fast() * v.fast());
        }

        inline quat operator-() const noexcept
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

        inline bool is_nan() const noexcept
        {
            return fast().is_nan();
        }

        inline bool is_infinite() const noexcept
        {
            return fast().is_infinite();
        }

        inline bool is_identity() const noexcept
        {
            return fast().is_identity();
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

        inline quat& set_x(float x) noexcept
        {
            _value.x = x;
            return *this;
        }

        inline quat& set_y(float y) noexcept
        {
            _value.y = y;
            return *this;
        }

        inline quat& set_z(float z) noexcept
        {
            _value.z = z;
            return *this;
        }

        inline quat& set_w(float w) noexcept
        {
            _value.w = w;
            return *this;
        }

        inline float length() const noexcept
        {
            return fast().length();
        }

        inline float squared_length() const noexcept
        {
            return fast().squared_length();
        }

        inline float reciprocal_length() const noexcept
        {
            return fast().reciprocal_length();
        }

        inline quat normalized() const noexcept
        {
            return fast().normalized();
        }

        inline quat normalized_approximate() const noexcept
        {
            return fast().normalized_approximate();
        }

        inline float dot(const quat& v) const noexcept
        {
            return fast().dot(v._value);
        }

        inline quat conjugate() const noexcept
        {
            return fast().conjugate();
        }

        inline quat inverse() const noexcept
        {
            return fast().inverse();
        }

        inline quat ln() const noexcept
        {
            return fast().ln();
        }

        inline quat exp() const noexcept
        {
            return fast().exp();
        }

        inline fastquat slerp(const quat& v, float t) const noexcept
        {
            return fast().slerp(v._value, t);
        }

        static inline fastquat slerp(
            const quat& v1, const quat& v2, float t) noexcept
        {
            return fastquat::slerp(v1._value, v2._value, t);
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
        static inline quat identity() noexcept
        {
            return fastquat::identity();
        }

        inline static quat barycentric(const quat& q0, const quat& q1,
            const quat& q2, float f, float g) noexcept
        {
            return fastquat::barycentric(q0.fast(), q1.fast(), q2.fast(), f, g);
        }

        inline static quat barycentric_v(const quat& q1, const quat& q2,
            const quat& q3, float f, float g) noexcept
        {
            return fastquat::barycentric_v(
                q1.fast(), q2.fast(), q3.fast(), f, g);
        }

        static inline quat from_pitch_yaw_roll(
            float pitch, float yaw, float roll) noexcept
        {
            return fastquat::from_pitch_yaw_roll(pitch, yaw, roll);
        }

        static inline quat from_pitch_yaw_roll(const vec3& angles) noexcept;

        static inline quat from_rotation_normal(
            const vec3& normal_axis, float angle) noexcept;

        static inline quat from_rotation_axis(
            const vec3& axis, float angle) noexcept;

        static inline quat rotation_matrix_to_quaternion(
            const fastmat4& matrix) noexcept;

    private:
        XMFLOAT4 _value;
    };
}  // namespace move::vectormath