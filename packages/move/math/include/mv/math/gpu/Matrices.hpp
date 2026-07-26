#pragma once

#include <cstddef>
#include <cstdint>
#include <type_traits>

#include <mv/math/gpu/Vectors.hpp>
#include <mv/math/packed/Vectors.hpp>

namespace mv::math::gpu
{
    enum class MatrixMajor : std::uint8_t
    {
        Row,
        Column
    };

    template <MatrixMajor Major>
    struct alignas(16) Mat3f16
    {
        Float3Slot16 Vectors[3]{};
    };

    template <MatrixMajor Major>
    struct alignas(16) Mat4f16
    {
        PackedVec4f Vectors[4]{};
    };

    using RowMajorMat3f16 = Mat3f16<MatrixMajor::Row>;
    using ColumnMajorMat3f16 = Mat3f16<MatrixMajor::Column>;
    using RowMajorMat4f16 = Mat4f16<MatrixMajor::Row>;
    using ColumnMajorMat4f16 = Mat4f16<MatrixMajor::Column>;
}  // namespace mv::math::gpu

static_assert(sizeof(mv::math::gpu::RowMajorMat3f16) == 48);
static_assert(alignof(mv::math::gpu::RowMajorMat3f16) == 16);
static_assert(sizeof(mv::math::gpu::ColumnMajorMat3f16) == 48);
static_assert(alignof(mv::math::gpu::ColumnMajorMat3f16) == 16);
static_assert(sizeof(mv::math::gpu::RowMajorMat4f16) == 64);
static_assert(alignof(mv::math::gpu::RowMajorMat4f16) == 16);
static_assert(sizeof(mv::math::gpu::ColumnMajorMat4f16) == 64);
static_assert(alignof(mv::math::gpu::ColumnMajorMat4f16) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::gpu::RowMajorMat3f16>);
static_assert(std::is_trivially_copyable_v<mv::math::gpu::RowMajorMat4f16>);
