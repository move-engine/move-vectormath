#pragma once

#include <cmath>
#include <cstddef>
#include <limits>
#include <type_traits>

#include <mv/math/geometry/Ray3.hpp>

namespace mv::math
{
    // Reciprocal direction/sign preparation follows Williams et al., "An
    // Efficient and Robust Ray-Box Intersection Algorithm" (JGT 2005).
    template <typename T>
        requires std::is_floating_point_v<T>
    class PreparedRay3
    {
    public:
        explicit PreparedRay3(const Ray3<T>& ray) noexcept :
            Ray_(ray),
            ReciprocalDirection_(Reciprocal(ray.Direction().Vector().X()),
                                 Reciprocal(ray.Direction().Vector().Y()),
                                 Reciprocal(ray.Direction().Vector().Z())),
            ParallelMask_(BuildParallelMask(ray.Direction().Vector()))
        {
        }

        [[nodiscard]] const Ray3<T>& Ray() const noexcept
        {
            return Ray_;
        }

        [[nodiscard]] const Vec3<T>& ReciprocalDirection() const noexcept
        {
            return ReciprocalDirection_;
        }

        [[nodiscard]] bool IsParallel(std::size_t axis) const noexcept
        {
            return (ParallelMask_ & (1U << axis)) != 0U;
        }

    private:
        [[nodiscard]] static T Reciprocal(T value) noexcept
        {
            return value == T(0)
                       ? std::copysign(std::numeric_limits<T>::infinity(),
                                       value)
                       : T(1) / value;
        }

        [[nodiscard]] static unsigned int BuildParallelMask(
            const Vec3<T>& direction) noexcept
        {
            return (IsEffectivelyParallel(direction.X()) ? 1U : 0U) |
                   (IsEffectivelyParallel(direction.Y()) ? 2U : 0U) |
                   (IsEffectivelyParallel(direction.Z()) ? 4U : 0U);
        }

        [[nodiscard]] static bool IsEffectivelyParallel(T value) noexcept
        {
            return value == T(0) || !std::isfinite(T(1) / value);
        }

        Ray3<T> Ray_;
        Vec3<T> ReciprocalDirection_;
        unsigned int ParallelMask_;
    };

    using PreparedRay3f = PreparedRay3<float>;
    using PreparedRay3d = PreparedRay3<double>;
}  // namespace mv::math

static_assert(std::is_trivially_copyable_v<mv::math::PreparedRay3f>);
static_assert(std::is_standard_layout_v<mv::math::PreparedRay3f>);
static_assert(std::is_trivially_copyable_v<mv::math::PreparedRay3d>);
static_assert(std::is_standard_layout_v<mv::math::PreparedRay3d>);
