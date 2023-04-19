#pragma once
#include "DirectXMath.h"
#include "common.hpp"

#if !defined(MOVE_FASTMAT4_HPP)
#define MOVE_FASTMAT4_HPP
#endif

namespace move::vectormath
{
    struct fastmat4
    {
        using XMMATRIX = fastmath::XMMATRIX;
        using CXMMATRIX = fastmath::CXMMATRIX;
        using XMFLOAT4X4 = fastmath::XMFLOAT4X4;

        inline fastmat4() noexcept : _mat(fastmath::XMMatrixIdentity())
        {
        }

        inline fastmat4(const XMMATRIX& m) noexcept : _mat(m)
        {
        }

        inline fastmat4(const XMFLOAT4X4& m) noexcept
            : _mat(fastmath::XMLoadFloat4x4(&m))
        {
        }

    public:
        inline operator XMMATRIX&() noexcept
        {
            return _mat;
        }

        inline operator CXMMATRIX() const noexcept
        {
            return _mat;
        }

        inline operator XMFLOAT4X4() noexcept
        {
            XMFLOAT4X4 res;
            fastmath::XMStoreFloat4x4(&res, _mat);
            return res;
        }

        inline fastmat4& operator=(const XMMATRIX& m) noexcept
        {
            _mat = m;
            return *this;
        }

        inline fastmat4& operator=(const XMFLOAT4X4& m) noexcept
        {
            _mat = fastmath::XMLoadFloat4x4(&m);
            return *this;
        }

        inline fastmat4 operator*(const fastmat4& m) const noexcept
        {
            return fastmath::XMMatrixMultiply(_mat, m._mat);
        }

        inline fastmat4& operator*=(const fastmat4& m) noexcept
        {
            _mat = fastmath::XMMatrixMultiply(_mat, m._mat);
            return *this;
        }

    public:
        inline fastmat4 inverse()
        {
            return fastmath::XMMatrixInverse(0, _mat);
        }

        inline fastmat4 inverse_get_determinant(float& outDeterminant)
        {
            fastmath::XMVECTOR detvec;
            auto res = fastmath::XMMatrixInverse(&detvec, _mat);
            outDeterminant = fastmath::XMVectorGetX(detvec);
            return res;
        }

    public:
        static inline fastmat4 create_identity() noexcept
        {
            return fastmath::XMMatrixIdentity();
        }

        static inline fastmat4 create_rotation_x(float angle) noexcept
        {
            return fastmath::XMMatrixRotationX(angle);
        }

        static inline fastmat4 create_rotation_y(float angle) noexcept
        {
            return fastmath::XMMatrixRotationY(angle);
        }

        static inline fastmat4 create_rotation_z(float angle) noexcept
        {
            return fastmath::XMMatrixRotationZ(angle);
        }

        static inline fastmat4 create_translation(
            float x, float y, float z) noexcept
        {
            return fastmath::XMMatrixTranslation(x, y, z);
        }

        static inline fastmat4 create_scale(float x, float y, float z) noexcept
        {
            return fastmath::XMMatrixScaling(x, y, z);
        }

        static inline fastmat4 create_perspective_rh(
            float fov, float aspect, float near, float far) noexcept
        {
            // TODO: Look at algorithm from Sony's vectormath.  It seems to be
            // faster.
            return fastmath::XMMatrixPerspectiveFovRH(fov, aspect, near, far);
        }

        static inline fastmat4 create_perspective_lh(
            float fov, float aspect, float near, float far) noexcept
        {
            // TODO: Look at algorithm from Sony's vectormath.  It seems to be
            // faster.
            return fastmath::XMMatrixPerspectiveFovLH(fov, aspect, near, far);
        }

        static inline fastmat4 create_perspective(
            float fov, float aspect, float near, float far) noexcept
        {
            return create_perspective_lh(fov, aspect, near, far);
        }

        static inline fastmat4 create_ortho_rh(
            float width, float height, float near, float far) noexcept
        {
            return fastmath::XMMatrixOrthographicRH(width, height, near, far);
        }

        static inline fastmat4 create_ortho_lh(
            float width, float height, float near, float far) noexcept
        {
            return fastmath::XMMatrixOrthographicLH(width, height, near, far);
        }

        static inline fastmat4 create_ortho(
            float width, float height, float near, float far) noexcept
        {
            return create_ortho_lh(width, height, near, far);
        }

        static inline fastmat4 create_ortho_off_center_rh(float left,
            float right, float bottom, float top, float near,
            float far) noexcept
        {
            return fastmath::XMMatrixOrthographicOffCenterRH(
                left, right, bottom, top, near, far);
        }

        static inline fastmat4 create_ortho_off_center_lh(float left,
            float right, float bottom, float top, float near,
            float far) noexcept
        {
            return fastmath::XMMatrixOrthographicOffCenterLH(
                left, right, bottom, top, near, far);
        }

    public:
        // Externally defined
        fastvec3 operator*(const fastvec3& v) const noexcept;
        fastvec4 operator*(const fastvec4& v) const noexcept;

    public:
        static fastmat4 create_look_at_rh(const fastvec3& eye,
            const fastvec3& target, const fastvec3& up) noexcept;

        static fastmat4 create_look_at_lh(const fastvec3& eye,
            const fastvec3& target, const fastvec3& up) noexcept;

        static fastmat4 create_look_at(const fastvec3& eye,
            const fastvec3& target, const fastvec3& up) noexcept;

        static fastmat4 create_transformation_matrix(
            const fastvec3& translation, const fastquat& rotation,
            const fastvec3& scale) noexcept;

    private:
        XMMATRIX _mat;
    };
}  // namespace move::vectormath