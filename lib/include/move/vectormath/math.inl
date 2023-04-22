#pragma once

#include "DirectXMath.h"
#include "all.hpp"

#if !defined(MVM_INL)
#if defined(MOVE_FASTMATH_NO_INLINE)
#define MVM_INL
#else
#define MVM_INL inline
#endif
#endif

namespace move::vectormath
{
    // fastmat4
    MVM_INL fastvec3 fastmat4::operator*(const fastvec3& v) const noexcept
    {
        return internal::DirectX::XMVector3Transform(v, _mat);
    }

    MVM_INL fastvec4 fastmat4::operator*(const fastvec4& v) const noexcept
    {
        return internal::DirectX::XMVector4Transform(v, _mat);
    }

    MVM_INL fastmat4 fastmat4::create_look_at_rh(const fastvec3& eye,
        const fastvec3& target, const fastvec3& up) noexcept
    {
        return internal::DirectX::XMMatrixLookAtRH(eye, target, up);
    }

    MVM_INL fastmat4 fastmat4::create_look_at_lh(const fastvec3& eye,
        const fastvec3& target, const fastvec3& up) noexcept
    {
        return internal::DirectX::XMMatrixLookAtLH(eye, target, up);
    }

    MVM_INL fastmat4 fastmat4::create_look_at(const fastvec3& eye,
        const fastvec3& target, const fastvec3& up) noexcept
    {
        return create_look_at_lh(eye, target, up);
    }

    MVM_INL fastmat4 fastmat4::create_transformation_matrix(
        const fastvec3& translation, const fastquat& rotation,
        const fastvec3& scale) noexcept
    {
        return internal::DirectX::XMMatrixTransformation(fastvec3::zero(),
            rotation, scale, fastvec3::zero(), rotation, translation);
    }

    // Quat
    MVM_INL fastquat fastquat::from_pitch_yaw_roll(
        const fastvec3& angles) noexcept
    {
        return fastquat{
            internal::DirectX::XMQuaternionRotationRollPitchYawFromVector(
                angles)};
    }

    MVM_INL fastquat fastquat::from_rotation_normal(
        const fastvec3& normal_axis, float angle) noexcept
    {
        return fastquat{
            internal::DirectX::XMQuaternionRotationNormal(normal_axis, angle)};
    }

    MVM_INL fastquat fastquat::from_rotation_axis(
        const fastvec3& axis, float angleRad) noexcept
    {
        return fastquat{
            internal::DirectX::XMQuaternionRotationAxis(axis, angleRad)};
    }

    MVM_INL fastquat fastquat::rotation_matrix_to_quaternion(
        const fastmat4& matrix) noexcept
    {
        return internal::DirectX::XMQuaternionRotationMatrix(matrix);
    }

    MVM_INL fastmat4 fastquat::to_rotation_matrix() const noexcept
    {
        return internal::DirectX::XMMatrixRotationQuaternion(_quat);
    }

    MVM_INL void fastquat::to_axis_angle(
        fastvec3& axis, float& angleRad) noexcept
    {
        internal::DirectX::XMVECTOR& _axis = axis;
        internal::DirectX::XMQuaternionToAxisAngle(&_axis, &angleRad, _quat);
    }

    MVM_INL fastvec3 fastquat::operator*(const fastvec3& v) const noexcept
    {
        return internal::DirectX::XMVector3Rotate(v, _quat);
    }

    // fastvec3
    inline fastvec3 fastvec3::operator*(const fastquat& v) const noexcept
    {
        return internal::DirectX::XMVector3Rotate(_vec, v);
    }

    // mat4
    MVM_INL mat4 mat4::create_look_at_rh(
        const vec3& eye, const vec3& target, const vec3& up) noexcept
    {
        return mat4(
            fastmat4::create_look_at_rh(eye.fast(), target.fast(), up.fast()));
    }

    MVM_INL mat4 mat4::create_look_at_lh(
        const vec3& eye, const vec3& target, const vec3& up) noexcept
    {
        return mat4(
            fastmat4::create_look_at_lh(eye.fast(), target.fast(), up.fast()));
    }

    MVM_INL mat4 mat4::create_look_at(
        const vec3& eye, const vec3& target, const vec3& up) noexcept
    {
        return mat4(
            fastmat4::create_look_at(eye.fast(), target.fast(), up.fast()));
    }

    MVM_INL mat4 mat4::create_transformation_matrix(const vec3& translation,
        const quat& rotation, const vec3& scale) noexcept
    {
        return mat4(fastmat4::create_transformation_matrix(
            translation.fast(), rotation.fast(), scale.fast()));
    }
}  // namespace move::vectormath