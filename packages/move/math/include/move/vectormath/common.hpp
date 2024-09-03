#pragma once
#include <cmath>

#include <move/math/macros.hpp>

namespace move::math
{
    enum class Acceleration
    {
        Scalar,
        RTM,
        Default = Scalar
    };

    template <typename T>
    MVM_INLINE_NODISCARD T sqrt(const T& value)
    {
        return std::sqrt(value);
    }

    template <typename T>
    MVM_INLINE_NODISCARD T sqrt_reciprocal(const T& value)
    {
        return T(1) / std::sqrt(value);
    }
}  // namespace move::math