#include <mv/math/Mat3.hpp>

#if defined(_MSC_VER)
#define MV_MATRIX_NOINLINE __declspec(noinline)
#else
#define MV_MATRIX_NOINLINE __attribute__((noinline))
#endif

extern "C" MV_MATRIX_NOINLINE mv::math::Vec3f MatrixTransformVector(
    mv::math::Vec3f vector, const mv::math::Mat3f& matrix) noexcept
{
    return vector * matrix;
}

extern "C" MV_MATRIX_NOINLINE mv::math::Mat3f MatrixMultiply(
    const mv::math::Mat3f& left, const mv::math::Mat3f& right) noexcept
{
    return left * right;
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
