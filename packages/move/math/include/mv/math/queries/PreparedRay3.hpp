#pragma once

#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <mv/math/geometry/Ray3.hpp>

namespace mv::math
{
    class PreparedRay3f
    {
    public:
        explicit PreparedRay3f(const Ray3f& ray) noexcept :
            Ray_(ray),
            ReciprocalDirection_(Reciprocal(ray.Direction().Vector().X()),
                                 Reciprocal(ray.Direction().Vector().Y()),
                                 Reciprocal(ray.Direction().Vector().Z())),
            ParallelMask_(BuildParallelMask(ray.Direction().Vector()))
        {
        }

        [[nodiscard]] const Ray3f& Ray() const noexcept
        {
            return Ray_;
        }

        [[nodiscard]] const Vec3f& ReciprocalDirection() const noexcept
        {
            return ReciprocalDirection_;
        }

        [[nodiscard]] bool IsParallel(std::size_t axis) const noexcept
        {
            return (ParallelMask_ & (1U << axis)) != 0U;
        }

    private:
        [[nodiscard]] static float Reciprocal(float value) noexcept
        {
            return value == 0.0F
                       ? std::copysign(std::numeric_limits<float>::infinity(),
                                       value)
                       : 1.0F / value;
        }

        [[nodiscard]] static unsigned int BuildParallelMask(
            const Vec3f& direction) noexcept
        {
            return (IsEffectivelyParallel(direction.X()) ? 1U : 0U) |
                   (IsEffectivelyParallel(direction.Y()) ? 2U : 0U) |
                   (IsEffectivelyParallel(direction.Z()) ? 4U : 0U);
        }

        [[nodiscard]] static bool IsEffectivelyParallel(float value) noexcept
        {
            return value == 0.0F || !std::isfinite(1.0F / value);
        }

        Ray3f Ray_;
        Vec3f ReciprocalDirection_;
        unsigned int ParallelMask_;
    };
}  // namespace mv::math

static_assert(std::is_trivially_copyable_v<mv::math::PreparedRay3f>);
static_assert(std::is_standard_layout_v<mv::math::PreparedRay3f>);
