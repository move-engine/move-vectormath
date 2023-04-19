#pragma once
#include <DirectXMath.h>
#include "common.hpp"

#if !defined(MOVE_FASTQUAT_HPP)
#define MOVE_FASTQUAT_HPP
#endif

namespace move::vectormath
{
    struct fastquat
    {
        using XMVECTOR = fastmath::XMVECTOR;
        using CXMVECTOR = fastmath::CXMVECTOR;
        using XMFLOAT4 = fastmath::XMFLOAT4;

        inline fastquat() noexcept : _quat(fastmath::XMQuaternionIdentity())
        {
        }

        inline fastquat(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _quat(fastmath::XMVectorSet(x, y, z, w))
        {
        }

        inline fastquat(const XMVECTOR& v) noexcept : _quat(v)
        {
        }

        inline fastquat(const XMFLOAT4& v) noexcept
            : _quat(fastmath::XMLoadFloat4(&v))
        {
        }

    public:
        inline operator XMVECTOR&() noexcept
        {
            return _quat;
        }

        inline operator CXMVECTOR() const noexcept
        {
            return _quat;
        }

        inline operator XMFLOAT4() const noexcept
        {
            XMFLOAT4 v;
            XMStoreFloat4(&v, _quat);
            return v;
        }

        inline fastquat& operator=(const XMVECTOR& v) noexcept
        {
            _quat = v;
            return *this;
        }

        inline fastquat& operator=(const XMFLOAT4& v) noexcept
        {
            _quat = fastmath::XMLoadFloat4(&v);
            return *this;
        }

        inline fastquat& operator+=(const fastquat& v) noexcept
        {
            _quat = fastmath::XMVectorAdd(_quat, v._quat);
            return *this;
        }

        inline fastquat& operator-=(const fastquat& v) noexcept
        {
            _quat = fastmath::XMVectorSubtract(_quat, v._quat);
            return *this;
        }

        inline fastquat& operator*=(const fastquat& v) noexcept
        {
            _quat = fastmath::XMVectorMultiply(_quat, v._quat);
            return *this;
        }

        inline fastquat& operator/=(const fastquat& v) noexcept
        {
            _quat = fastmath::XMVectorDivide(_quat, v._quat);
            return *this;
        }

        inline fastquat operator+(const fastquat& v) const noexcept
        {
            return fastmath::XMVectorAdd(_quat, v._quat);
        }

        inline fastquat operator-(const fastquat& v) const noexcept
        {
            return fastmath::XMVectorSubtract(_quat, v._quat);
        }

        inline fastquat operator*(const fastquat& v) const noexcept
        {
            return fastmath::XMVectorMultiply(_quat, v._quat);
        }

        inline fastquat operator/(const fastquat& v) const noexcept
        {
            return fastmath::XMVectorDivide(_quat, v._quat);
        }

        inline fastquat operator-() const noexcept
        {
            return fastmath::XMVectorNegate(_quat);
        }

    public:
        inline bool operator==(const fastquat& rhs) const noexcept
        {
            return fastmath::XMQuaternionEqual(_quat, rhs._quat);
        }

        inline bool operator!=(const fastquat& rhs) const noexcept
        {
            return fastmath::XMQuaternionNotEqual(_quat, rhs._quat);
        }

        inline bool is_nan() const noexcept
        {
            return fastmath::XMQuaternionIsNaN(_quat);
        }

        inline bool is_infinite() const noexcept
        {
            return fastmath::XMQuaternionIsInfinite(_quat);
        }

        inline bool is_identity() const noexcept
        {
            return fastmath::XMQuaternionIsIdentity(_quat);
        }

        inline float dot(const fastquat& v) const noexcept
        {
            return fastmath::XMVectorGetX(
                fastmath::XMQuaternionDot(_quat, v._quat));
        }

        inline float squared_length() const noexcept
        {
            return fastmath::XMVectorGetX(
                fastmath::XMQuaternionLengthSq(_quat));
        }

        inline float reciprocal_length() const noexcept
        {
            return fastmath::XMVectorGetX(
                fastmath::XMQuaternionReciprocalLength(_quat));
        }

        inline float length() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMQuaternionLength(_quat));
        }

        inline fastquat normalize_est() const noexcept
        {
            return fastmath::XMQuaternionNormalizeEst(_quat);
        }

        inline fastquat normalize() const noexcept
        {
            return fastmath::XMQuaternionNormalize(_quat);
        }

        inline fastquat conjugate() const noexcept
        {
            return fastmath::XMQuaternionConjugate(_quat);
        }

        inline fastquat inverse() const noexcept
        {
            return fastmath::XMQuaternionInverse(_quat);
        }

        inline XMVECTOR ln() const noexcept
        {
            return fastmath::XMQuaternionLn(_quat);
        }

        inline XMVECTOR exp() const noexcept
        {
            return fastmath::XMQuaternionExp(_quat);
        }

        inline fastquat slerp(const fastquat& q1, float t) const noexcept
        {
            return slerp(*this, q1, t);
        }

        static inline fastquat slerp(
            const fastquat& q1, const fastquat& q2, float t) noexcept
        {
            return fastmath::XMQuaternionSlerp(q1._quat, q2._quat, t);
        }

    public:
        inline float x() const noexcept
        {
            return fastmath::XMVectorGetX(_quat);
        }

        inline float y() const noexcept
        {
            return fastmath::XMVectorGetY(_quat);
        }

        inline float z() const noexcept
        {
            return fastmath::XMVectorGetZ(_quat);
        }

        inline float w() const noexcept
        {
            return fastmath::XMVectorGetW(_quat);
        }

        inline fastquat& set_x(float x) noexcept
        {
            _quat = fastmath::XMVectorSetX(_quat, x);
            return *this;
        }

        inline fastquat& set_y(float y) noexcept
        {
            _quat = fastmath::XMVectorSetY(_quat, y);
            return *this;
        }

        inline fastquat& set_z(float z) noexcept
        {
            _quat = fastmath::XMVectorSetZ(_quat, z);
            return *this;
        }

        inline fastquat& set_w(float w) noexcept
        {
            _quat = fastmath::XMVectorSetW(_quat, w);
            return *this;
        }

    public:
        static inline fastquat identity() noexcept
        {
            return fastmath::XMQuaternionIdentity();
        }

        inline static fastquat barycentric(const fastquat& q0,
            const fastquat& q1, const fastquat& q2, float f, float g) noexcept
        {
            return fastquat(fastmath::XMQuaternionBaryCentric(
                q0._quat, q1._quat, q2._quat, f, g));
        }

        inline static fastquat barycentric_v(const fastquat& q1,
            const fastquat& q2, const fastquat& q3, float f, float g) noexcept
        {
            return fastmath::XMQuaternionBaryCentricV(q1._quat, q2._quat,
                q3._quat, fastmath::XMVectorSet(f, f, f, f),
                fastmath::XMVectorSet(g, g, g, g));
        }

        static inline fastquat from_pitch_yaw_roll(
            float pitch, float yaw, float roll) noexcept
        {
            return fastmath::XMQuaternionRotationRollPitchYaw(pitch, yaw, roll);
        }

        static inline fastquat from_pitch_yaw_roll(
            const fastvec3& angles) noexcept;

        static inline fastquat from_rotation_normal(
            const fastvec3& normal_axis, float angle) noexcept;

        static inline fastquat from_rotation_axis(
            const fastvec3& axis, float angle) noexcept;

        static inline fastquat rotation_matrix_to_quaternion(
            const fastmat4& matrix) noexcept;

    public:
        void to_axis_angle(fastvec3& axis, float& angle) noexcept;
        fastvec3 operator*(const fastvec3& v) const noexcept;
        fastmat4 to_rotation_matrix() const noexcept;

    private:
        XMVECTOR _quat;
    };
}  // namespace move::vectormath