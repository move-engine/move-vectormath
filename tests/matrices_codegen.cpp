#include <mv/math/Mat3.hpp>
#include <mv/math/Mat4.hpp>
#include <mv/math/transforms/Projection.hpp>
#include <mv/math/transforms/View.hpp>

#if defined(_MSC_VER)
#define MV_MATRIX_NOINLINE __declspec(noinline)
#else
#define MV_MATRIX_NOINLINE __attribute__((noinline))
#endif

extern "C" MV_MATRIX_NOINLINE void MatrixTransformVector(
    mv::math::Vec3f vector,
    const mv::math::Mat3f& matrix,
    mv::math::Vec3f& output) noexcept
{
    output = vector * matrix;
}

extern "C" MV_MATRIX_NOINLINE void MatrixMultiply(
    const mv::math::Mat3f& left,
    const mv::math::Mat3f& right,
    mv::math::Mat3f& output) noexcept
{
    output = left * right;
}

extern "C" MV_MATRIX_NOINLINE bool MatrixTryInverse(
    const mv::math::Mat3f& matrix, mv::math::Mat3f& output) noexcept
{
    const auto inverse = matrix.TryInverse();
    if (!inverse)
    {
        return false;
    }
    output = *inverse;
    return true;
}

extern "C" MV_MATRIX_NOINLINE void Matrix4TransformHomogeneous(
    mv::math::Vec4f vector,
    const mv::math::Mat4f& matrix,
    mv::math::Vec4f& output) noexcept
{
    output = vector * matrix;
}

extern "C" MV_MATRIX_NOINLINE void Matrix4Multiply(
    const mv::math::Mat4f& left,
    const mv::math::Mat4f& right,
    mv::math::Mat4f& output) noexcept
{
    output = left * right;
}

extern "C" MV_MATRIX_NOINLINE bool Matrix4TryInverse(
    const mv::math::Mat4f& matrix, mv::math::Mat4f& output) noexcept
{
    const auto inverse = matrix.TryInverse();
    if (!inverse)
    {
        return false;
    }
    output = *inverse;
    return true;
}

extern "C" MV_MATRIX_NOINLINE bool Matrix4PerspectiveReverseZ(
    float verticalFieldOfView,
    float aspectRatio,
    float nearPlane,
    float farPlane,
    mv::math::Mat4f& output) noexcept
{
    const auto projection = mv::math::TryPerspectiveFov(
        mv::math::Radiansf(verticalFieldOfView), aspectRatio, nearPlane,
        farPlane, mv::math::Handedness::Right, mv::math::ClipDepth::ZeroToOne,
        mv::math::DepthDirection::Reverse);
    if (!projection)
    {
        return false;
    }
    output = *projection;
    return true;
}

extern "C" MV_MATRIX_NOINLINE bool Matrix4LookAt(
    const mv::math::Point3f& eye,
    const mv::math::Point3f& target,
    const mv::math::Direction3f& up,
    mv::math::Mat4f& output) noexcept
{
    const auto view = mv::math::TryLookAtRH(eye, target, up);
    if (!view)
    {
        return false;
    }
    output = *view;
    return true;
}
