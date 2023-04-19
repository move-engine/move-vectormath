#pragma once

#include "DirectXMath.h"
#include "all.hpp"
#include "move/vectormath/common.hpp"

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
        return fastmath::XMVector3Transform(v, _mat);
    }

    MVM_INL fastvec4 fastmat4::operator*(const fastvec4& v) const noexcept
    {
        return fastmath::XMVector4Transform(v, _mat);
    }

    MVM_INL fastmat4 fastmat4::create_look_at_rh(const fastvec3& eye,
        const fastvec3& target, const fastvec3& up) noexcept
    {
        return fastmath::XMMatrixLookAtRH(eye, target, up);
    }

    MVM_INL fastmat4 fastmat4::create_look_at_lh(const fastvec3& eye,
        const fastvec3& target, const fastvec3& up) noexcept
    {
        return fastmath::XMMatrixLookAtLH(eye, target, up);
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
        return fastmath::XMMatrixTransformation(fastvec3::zero(), rotation,
            scale, fastvec3::zero(), rotation, translation);
    }

    // Quat
    MVM_INL fastquat fastquat::rotation_roll_pitch_yaw_from_vector(
        const fastvec3& angles) noexcept
    {
        return fastquat{
            fastmath::XMQuaternionRotationRollPitchYawFromVector(angles)};
    }

    MVM_INL fastquat fastquat::rotation_normal(
        const fastvec3& normal_axis, float angle) noexcept
    {
        return fastquat{
            fastmath::XMQuaternionRotationNormal(normal_axis, angle)};
    }

    MVM_INL fastquat fastquat::rotation_axis(
        const fastvec3& axis, float angleRad) noexcept
    {
        return fastquat{fastmath::XMQuaternionRotationAxis(axis, angleRad)};
    }

    MVM_INL fastquat fastquat::rotation_matrix_to_quaternion(
        const fastmat4& matrix) noexcept
    {
        return fastmath::XMQuaternionRotationMatrix(matrix);
    }

    MVM_INL void fastquat::to_axis_angle(
        fastvec3& axis, float& angleRad) noexcept
    {
        fastmath::XMVECTOR& _axis = axis;
        fastmath::XMQuaternionToAxisAngle(&_axis, &angleRad, _quat);
    }

    MVM_INL fastvec3 fastquat::operator*(const fastvec3& v) const noexcept
    {
        return fastmath::XMVector3Rotate(v, _quat);
    }

    // fastvec3
    inline fastvec3 fastvec3::operator*(const fastquat& v) const noexcept
    {
        return fastmath::XMVector3Rotate(v, _vec);
    }

}  // namespace move::vectormath