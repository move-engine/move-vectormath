#pragma once

#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstddef>
#include <cstdint>
#include <optional>
#include <type_traits>

#include <mv/math/Mat4.hpp>
#include <mv/math/geometry/Plane3.hpp>
#include <mv/math/transforms/Projection.hpp>

namespace mv::math
{
    enum class FrustumPlane : std::uint8_t
    {
        Left,
        Right,
        Bottom,
        Top,
        Near,
        Far
    };

    enum class FrustumPlaneMask : std::uint8_t
    {
        None = 0U,
        Left = 1U << 0U,
        Right = 1U << 1U,
        Bottom = 1U << 2U,
        Top = 1U << 3U,
        Near = 1U << 4U,
        Far = 1U << 5U,
        All = (1U << 6U) - 1U
    };

    [[nodiscard]] constexpr FrustumPlaneMask operator|(
        FrustumPlaneMask left, FrustumPlaneMask right) noexcept
    {
        return static_cast<FrustumPlaneMask>(static_cast<std::uint8_t>(left) |
                                             static_cast<std::uint8_t>(right));
    }

    [[nodiscard]] constexpr FrustumPlaneMask operator&(
        FrustumPlaneMask left, FrustumPlaneMask right) noexcept
    {
        return static_cast<FrustumPlaneMask>(static_cast<std::uint8_t>(left) &
                                             static_cast<std::uint8_t>(right));
    }

    [[nodiscard]] constexpr FrustumPlaneMask operator~(
        FrustumPlaneMask value) noexcept
    {
        return static_cast<FrustumPlaneMask>(
            static_cast<std::uint8_t>(FrustumPlaneMask::All) &
            ~static_cast<std::uint8_t>(value));
    }

    constexpr FrustumPlaneMask& operator|=(FrustumPlaneMask& left,
                                           FrustumPlaneMask right) noexcept
    {
        left = left | right;
        return left;
    }

    constexpr FrustumPlaneMask& operator&=(FrustumPlaneMask& left,
                                           FrustumPlaneMask right) noexcept
    {
        left = left & right;
        return left;
    }

    [[nodiscard]] constexpr FrustumPlaneMask PlaneMask(
        FrustumPlane plane) noexcept
    {
        return static_cast<FrustumPlaneMask>(
            1U << static_cast<std::uint8_t>(plane));
    }

    [[nodiscard]] constexpr bool HasPlane(FrustumPlaneMask mask,
                                          FrustumPlane plane) noexcept
    {
        return (mask & PlaneMask(plane)) != FrustumPlaneMask::None;
    }

    namespace detail
    {
        template <typename T>
        [[nodiscard]] inline std::optional<Plane3<T>> TryPlaneFromCoefficients(
            T x, T y, T z, T offset) noexcept
        {
            const T maximum = std::max({std::abs(x), std::abs(y), std::abs(z)});
            if (!(maximum > T(0)) || !std::isfinite(maximum) ||
                !std::isfinite(offset))
            {
                return std::nullopt;
            }
            const Vec3<T> scaled(x / maximum, y / maximum, z / maximum);
            const T scaledLength = Length(scaled);
            const auto normal = Normal3<T>::TryFrom(scaled);
            if (!normal || !(scaledLength > T(0)) ||
                !std::isfinite(scaledLength))
            {
                return std::nullopt;
            }
            return Plane3<T>::TryFromNormalAndOffset(
                *normal, offset / (maximum * scaledLength));
        }
    }  // namespace detail

    template <typename T>
        requires std::is_floating_point_v<T>
    class Frustum3
    {
    public:
        using Component = T;

        [[nodiscard]] static std::optional<Frustum3> TryFromPlanes(
            const std::array<Plane3<T>, 6>& planes,
            FrustumPlaneMask activePlanes = FrustumPlaneMask::All) noexcept
        {
            constexpr FrustumPlaneMask Required =
                FrustumPlaneMask::Left | FrustumPlaneMask::Right |
                FrustumPlaneMask::Bottom | FrustumPlaneMask::Top |
                FrustumPlaneMask::Near;
            if ((activePlanes & Required) != Required)
            {
                return std::nullopt;
            }
            return Frustum3(planes, activePlanes, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Frustum3> TryFromViewProjection(
            const Mat4<T>& viewProjection,
            ClipDepth clipDepth,
            DepthDirection depthDirection) noexcept
        {
            if (!viewProjection.IsFinite())
            {
                return std::nullopt;
            }

            // Gribb and Hartmann, "Fast Extraction of Viewing Frustum Planes
            // from the World-View-Projection Matrix" (2001), transposed for
            // Move's row vectors and extended to explicit ZO/NO and reverse-Z
            // policies. Extracted normals point into the frustum.
            const Vec4<T> column0 = viewProjection.Column(0U);
            const Vec4<T> column1 = viewProjection.Column(1U);
            const Vec4<T> column2 = viewProjection.Column(2U);
            const Vec4<T> column3 = viewProjection.Column(3U);
            const auto makePlane = [](const Vec4<T>& coefficients)
            {
                return detail::TryPlaneFromCoefficients(
                    coefficients.X(), coefficients.Y(), coefficients.Z(),
                    coefficients.W());
            };

            const auto left = makePlane(column3 + column0);
            const auto right = makePlane(column3 - column0);
            const auto bottom = makePlane(column3 + column1);
            const auto top = makePlane(column3 - column1);
            const auto lowerDepth = makePlane(clipDepth == ClipDepth::ZeroToOne
                                                  ? column2
                                                  : column3 + column2);
            const auto upperDepth = makePlane(column3 - column2);
            const auto& nearPlane = depthDirection == DepthDirection::Forward
                                        ? lowerDepth
                                        : upperDepth;
            const auto& farPlane = depthDirection == DepthDirection::Forward
                                       ? upperDepth
                                       : lowerDepth;
            if (!left || !right || !bottom || !top || !nearPlane)
            {
                return std::nullopt;
            }

            std::array<Plane3<T>, 6> planes{*left, *right,     *bottom,
                                            *top,  *nearPlane, Plane3<T>()};
            FrustumPlaneMask active = FrustumPlaneMask::All;
            if (farPlane)
            {
                planes[static_cast<std::size_t>(FrustumPlane::Far)] = *farPlane;
            }
            else
            {
                active &= ~FrustumPlaneMask::Far;
            }
            return Frustum3(planes, active, UncheckedTag{});
        }

        [[nodiscard]] const Plane3<T>& Plane(FrustumPlane plane) const noexcept
        {
            assert(IsActive(plane));
            return Planes_[static_cast<std::size_t>(plane)];
        }

        [[nodiscard]] bool IsActive(FrustumPlane plane) const noexcept
        {
            return HasPlane(ActivePlanes_, plane);
        }

        [[nodiscard]] FrustumPlaneMask ActivePlanes() const noexcept
        {
            return ActivePlanes_;
        }

    private:
        struct UncheckedTag
        {
        };

        Frustum3(const std::array<Plane3<T>, 6>& planes,
                 FrustumPlaneMask activePlanes,
                 UncheckedTag) noexcept :
            Planes_(planes), ActivePlanes_(activePlanes)
        {
        }

        std::array<Plane3<T>, 6> Planes_{};
        FrustumPlaneMask ActivePlanes_{FrustumPlaneMask::All};
    };

    using Frustum3f = Frustum3<float>;
    using Frustum3d = Frustum3<double>;
}  // namespace mv::math
