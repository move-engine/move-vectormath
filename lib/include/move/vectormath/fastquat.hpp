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
        using XMVECTOR = internal::DirectX::XMVECTOR;
        using CXMVECTOR = internal::DirectX::CXMVECTOR;
        using XMFLOAT4 = internal::DirectX::XMFLOAT4;

        inline fastquat() noexcept
            : _quat(internal::DirectX::XMQuaternionIdentity())
        {
        }

        inline fastquat(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _quat(internal::DirectX::XMVectorSet(x, y, z, w))
        {
        }

        inline fastquat(const XMVECTOR& v) noexcept : _quat(v)
        {
        }

        inline fastquat(const XMFLOAT4& v) noexcept
            : _quat(internal::DirectX::XMLoadFloat4(&v))
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

        inline void store(XMFLOAT4& v) noexcept
        {
            XMStoreFloat4(&v, _quat);
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
            _quat = internal::DirectX::XMLoadFloat4(&v);
            return *this;
        }

        inline fastquat& operator*=(const fastquat& v) noexcept
        {
            _quat = internal::DirectX::XMQuaternionMultiply(_quat, v._quat);
            return *this;
        }

        inline fastquat operator*(const fastquat& v) const noexcept
        {
            return internal::DirectX::XMQuaternionMultiply(_quat, v._quat);
        }

        inline fastquat operator-() const noexcept
        {
            return inverse();
        }

    public:
        inline bool operator==(const fastquat& rhs) const noexcept
        {
            return internal::DirectX::XMQuaternionEqual(_quat, rhs._quat);
        }

        inline bool operator!=(const fastquat& rhs) const noexcept
        {
            return internal::DirectX::XMQuaternionNotEqual(_quat, rhs._quat);
        }

        inline bool is_nan() const noexcept
        {
            return internal::DirectX::XMQuaternionIsNaN(_quat);
        }

        inline bool is_infinite() const noexcept
        {
            return internal::DirectX::XMQuaternionIsInfinite(_quat);
        }

        inline bool is_identity() const noexcept
        {
            return internal::DirectX::XMQuaternionIsIdentity(_quat);
        }

        inline float length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMQuaternionLength(_quat));
        }

        inline float squared_length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMQuaternionLengthSq(_quat));
        }

        inline float reciprocal_length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMQuaternionReciprocalLength(_quat));
        }

        inline fastquat normalized_approximate() const noexcept
        {
            return internal::DirectX::XMQuaternionNormalizeEst(_quat);
        }

        inline fastquat normalized() const noexcept
        {
            return internal::DirectX::XMQuaternionNormalize(_quat);
        }

        inline float dot(const fastquat& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMQuaternionDot(_quat, v._quat));
        }

        inline void normalize() noexcept
        {
            _quat = internal::DirectX::XMQuaternionNormalize(_quat);
        }

        inline fastquat conjugate() const noexcept
        {
            return internal::DirectX::XMQuaternionConjugate(_quat);
        }

        inline fastquat inverse() const noexcept
        {
            return internal::DirectX::XMQuaternionInverse(_quat);
        }

        inline fastquat ln() const noexcept
        {
            return internal::DirectX::XMQuaternionLn(_quat);
        }

        inline fastquat exp() const noexcept
        {
            return internal::DirectX::XMQuaternionExp(_quat);
        }

        inline fastquat slerp(const fastquat& q1, float t) const noexcept
        {
            return slerp(*this, q1, t);
        }

        static inline fastquat slerp(
            const fastquat& q1, const fastquat& q2, float t) noexcept
        {
            return internal::DirectX::XMQuaternionSlerp(q1._quat, q2._quat, t);
        }

    public:
        inline float x() const noexcept
        {
            return internal::DirectX::XMVectorGetX(_quat);
        }

        inline float y() const noexcept
        {
            return internal::DirectX::XMVectorGetY(_quat);
        }

        inline float z() const noexcept
        {
            return internal::DirectX::XMVectorGetZ(_quat);
        }

        inline float w() const noexcept
        {
            return internal::DirectX::XMVectorGetW(_quat);
        }

        inline fastquat& set_x(float x) noexcept
        {
            _quat = internal::DirectX::XMVectorSetX(_quat, x);
            return *this;
        }

        inline fastquat& set_y(float y) noexcept
        {
            _quat = internal::DirectX::XMVectorSetY(_quat, y);
            return *this;
        }

        inline fastquat& set_z(float z) noexcept
        {
            _quat = internal::DirectX::XMVectorSetZ(_quat, z);
            return *this;
        }

        inline fastquat& set_w(float w) noexcept
        {
            _quat = internal::DirectX::XMVectorSetW(_quat, w);
            return *this;
        }

    public:
        static inline fastquat identity() noexcept
        {
            return internal::DirectX::XMQuaternionIdentity();
        }

        inline static fastquat barycentric(const fastquat& q0,
            const fastquat& q1, const fastquat& q2, float f, float g) noexcept
        {
            return fastquat(internal::DirectX::XMQuaternionBaryCentric(
                q0._quat, q1._quat, q2._quat, f, g));
        }

        inline static fastquat barycentric_v(const fastquat& q1,
            const fastquat& q2, const fastquat& q3, float f, float g) noexcept
        {
            return internal::DirectX::XMQuaternionBaryCentricV(q1._quat,
                q2._quat, q3._quat, internal::DirectX::XMVectorSet(f, f, f, f),
                internal::DirectX::XMVectorSet(g, g, g, g));
        }

        static inline fastquat from_pitch_yaw_roll(
            float pitch, float yaw, float roll) noexcept
        {
            return internal::DirectX::XMQuaternionRotationRollPitchYaw(
                pitch, yaw, roll);
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