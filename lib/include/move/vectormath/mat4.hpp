#pragma once
#include "DirectXMath.h"
#include "fastmat4.hpp"

#if !defined(MOVE_MAT4_HPP)
#define MOVE_MAT4_HPP
#endif

namespace move::vectormath
{
    struct mat4
    {
        using XMMATRIX = internal::DirectX::XMMATRIX;
        using XMFLOAT4X4 = internal::DirectX::XMFLOAT4X4;

        inline mat4() noexcept : _mat()
        {
            internal::DirectX::XMStoreFloat4x4(
                &_mat, internal::DirectX::XMMatrixIdentity());
        }

        inline mat4(const XMFLOAT4X4& m) noexcept : _mat(m)
        {
        }

    public:
        inline fastmat4 fast() const noexcept
        {
            return fastmat4(_mat);
        }

    public:
        inline operator XMFLOAT4X4&() noexcept
        {
            return _mat;
        }

        inline mat4 operator*(const mat4& m) const noexcept
        {
            return mat4(fast() * m.fast());
        }

        inline mat4& operator*=(const mat4& m) noexcept
        {
            (fast() * m.fast()).store(_mat);
            return *this;
        }

    public:
        inline mat4 inverse()
        {
            return mat4(fast().inverse());
        }

        inline mat4 inverse_get_determinant(float& outDeterminant)
        {
            return mat4(fast().inverse_get_determinant(outDeterminant));
        }

    public:
        static inline mat4 create_identity() noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixIdentity()));
        }

        static inline mat4 create_rotation_x(float angle) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixRotationX(angle)));
        }

        static inline mat4 create_rotation_y(float angle) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixRotationY(angle)));
        }

        static inline mat4 create_rotation_z(float angle) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixRotationZ(angle)));
        }

        static inline mat4 create_translation(
            float x, float y, float z) noexcept
        {
            return mat4(
                fastmat4(internal::DirectX::XMMatrixTranslation(x, y, z)));
        }

        static inline mat4 create_scale(float x, float y, float z) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixScaling(x, y, z)));
        }

        static inline mat4 create_perspective_rh(
            float fov, float aspect, float near, float far) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixPerspectiveFovRH(
                fov, aspect, near, far)));
        }

        static inline mat4 create_perspective_lh(
            float fov, float aspect, float near, float far) noexcept
        {
            return mat4(fastmat4(internal::DirectX::XMMatrixPerspectiveFovLH(
                fov, aspect, near, far)));
        }

        static inline mat4 create_perspective(
            float fov, float aspect, float near, float far) noexcept
        {
            return create_perspective_lh(fov, aspect, near, far);
        }

        static inline mat4 create_ortho_rh(
            float width, float height, float near, float far) noexcept
        {
            return mat4(fastmat4::create_ortho_rh(width, height, near, far));
        }

        static inline mat4 create_ortho_lh(
            float width, float height, float near, float far) noexcept
        {
            return mat4(fastmat4::create_ortho_lh(width, height, near, far));
        }

        static inline mat4 create_ortho(
            float width, float height, float near, float far) noexcept
        {
            return create_ortho_lh(width, height, near, far);
        }

        static inline mat4 create_ortho_off_center_rh(float left, float right,
            float bottom, float top, float near, float far) noexcept
        {
            return mat4(fastmat4::create_ortho_off_center_rh(
                left, right, bottom, top, near, far));
        }

        static inline mat4 create_ortho_off_center_lh(float left, float right,
            float bottom, float top, float near, float far) noexcept
        {
            return mat4(fastmat4::create_ortho_off_center_lh(
                left, right, bottom, top, near, far));
        }

    public:
        // Externally defined
        vec3 operator*(const vec3& v) const noexcept;
        vec4f operator*(const vec4f& v) const noexcept;

        inline float operator[](size_t index) const noexcept
        {
            return _mat.m[index / 4][index % 4];
        }

    public:
        static mat4 create_look_at_rh(
            const vec3& eye, const vec3& target, const vec3& up) noexcept;

        static mat4 create_look_at_lh(
            const vec3& eye, const vec3& target, const vec3& up) noexcept;

        static mat4 create_look_at(
            const vec3& eye, const vec3& target, const vec3& up) noexcept;

        static mat4 create_transformation_matrix(const vec3& translation,
            const quat& rotation, const vec3& scale) noexcept;

    private:
        XMFLOAT4X4 _mat;
    };
}  // namespace move::vectormath