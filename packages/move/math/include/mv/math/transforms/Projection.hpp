#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Angles.hpp>
#include <mv/math/Mat4.hpp>

namespace mv::math
{
    enum class Handedness
    {
        Left,
        Right
    };

    enum class ClipDepth
    {
        ZeroToOne,
        NegativeOneToOne
    };

    enum class DepthDirection
    {
        Forward,
        Reverse
    };

    namespace detail
    {
        template <typename T>
        [[nodiscard]] constexpr T NearClipDepth(
            ClipDepth clipDepth, DepthDirection depthDirection) noexcept
        {
            if (depthDirection == DepthDirection::Reverse)
            {
                return T(1);
            }
            return clipDepth == ClipDepth::ZeroToOne ? T(0) : T(-1);
        }

        template <typename T>
        [[nodiscard]] constexpr T FarClipDepth(
            ClipDepth clipDepth, DepthDirection depthDirection) noexcept
        {
            if (depthDirection == DepthDirection::Forward)
            {
                return T(1);
            }
            return clipDepth == ClipDepth::ZeroToOne ? T(0) : T(-1);
        }
    }  // namespace detail

    // Perspective construction is derived from the required NDC values at
    // the near and far planes. The resulting convention variants were
    // cross-checked against DirectXMath XMMatrixPerspectiveFovLH/RH (MIT) and
    // GLM matrix_clip_space.inl perspective functions (MIT).
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryPerspectiveFov(
        Radians<T> verticalFieldOfView,
        T aspectRatio,
        T nearPlane,
        T farPlane,
        Handedness handedness,
        ClipDepth clipDepth,
        DepthDirection depthDirection) noexcept
    {
        const T fieldOfView = verticalFieldOfView.Value();
        constexpr T Pi = static_cast<T>(3.14159265358979323846264338327950288L);
        if (!(fieldOfView > T(0) && fieldOfView < Pi) ||
            !(aspectRatio > T(0)) || !(nearPlane > T(0)) ||
            !(farPlane > nearPlane) || !std::isfinite(fieldOfView) ||
            !std::isfinite(aspectRatio) || !std::isfinite(nearPlane) ||
            !std::isfinite(farPlane))
        {
            return std::nullopt;
        }

        const T halfTangent = std::tan(fieldOfView * T(0.5));
        if (!(halfTangent > T(0)) || !std::isfinite(halfTangent))
        {
            return std::nullopt;
        }

        const T yScale = T(1) / halfTangent;
        const T xScale = yScale / aspectRatio;
        const T nearDepth = detail::NearClipDepth<T>(clipDepth, depthDirection);
        const T farDepth = detail::FarClipDepth<T>(clipDepth, depthDirection);
        const T inverseDistanceDifference = T(1) / nearPlane - T(1) / farPlane;
        const T translation =
            (nearDepth - farDepth) / inverseDistanceDifference;
        const T depthAtInfinity = nearDepth - translation / nearPlane;
        const T handednessSign = handedness == Handedness::Left ? T(1) : T(-1);
        const T depthScale = handednessSign * depthAtInfinity;

        const Mat4<T> result(xScale, T(0), T(0), T(0), T(0), yScale, T(0), T(0),
                             T(0), T(0), depthScale, handednessSign, T(0), T(0),
                             translation, T(0));
        return result.IsFinite() ? std::optional<Mat4<T>>(result)
                                 : std::nullopt;
    }

    // Infinite-far perspective is the analytic farPlane -> infinity limit of
    // TryPerspectiveFov, avoiding a large finite sentinel and its lost depth
    // precision.
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryPerspectiveFovInfinite(
        Radians<T> verticalFieldOfView,
        T aspectRatio,
        T nearPlane,
        Handedness handedness,
        ClipDepth clipDepth,
        DepthDirection depthDirection) noexcept
    {
        const T fieldOfView = verticalFieldOfView.Value();
        constexpr T Pi = static_cast<T>(3.14159265358979323846264338327950288L);
        if (!(fieldOfView > T(0) && fieldOfView < Pi) ||
            !(aspectRatio > T(0)) || !(nearPlane > T(0)) ||
            !std::isfinite(fieldOfView) || !std::isfinite(aspectRatio) ||
            !std::isfinite(nearPlane))
        {
            return std::nullopt;
        }

        const T halfTangent = std::tan(fieldOfView * T(0.5));
        if (!(halfTangent > T(0)) || !std::isfinite(halfTangent))
        {
            return std::nullopt;
        }

        const T yScale = T(1) / halfTangent;
        const T xScale = yScale / aspectRatio;
        const T nearDepth = detail::NearClipDepth<T>(clipDepth, depthDirection);
        const T farDepth = detail::FarClipDepth<T>(clipDepth, depthDirection);
        const T translation = (nearDepth - farDepth) * nearPlane;
        const T handednessSign = handedness == Handedness::Left ? T(1) : T(-1);
        const T depthScale = handednessSign * farDepth;

        const Mat4<T> result(xScale, T(0), T(0), T(0), T(0), yScale, T(0), T(0),
                             T(0), T(0), depthScale, handednessSign, T(0), T(0),
                             translation, T(0));
        return result.IsFinite() ? std::optional<Mat4<T>>(result)
                                 : std::nullopt;
    }

    // Orthographic construction maps physical camera-space distances to the
    // requested NDC interval. Variants were cross-checked against DirectXMath
    // XMMatrixOrthographicOffCenterLH/RH (MIT) and GLM matrix_clip_space.inl
    // orthographic functions (MIT).
    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline std::optional<Mat4<T>> TryOrthographicOffCenter(
        T left,
        T right,
        T bottom,
        T top,
        T nearPlane,
        T farPlane,
        Handedness handedness,
        ClipDepth clipDepth,
        DepthDirection depthDirection) noexcept
    {
        if (right == left || top == bottom || !(farPlane > nearPlane) ||
            !std::isfinite(left) || !std::isfinite(right) ||
            !std::isfinite(bottom) || !std::isfinite(top) ||
            !std::isfinite(nearPlane) || !std::isfinite(farPlane))
        {
            return std::nullopt;
        }

        const T nearDepth = detail::NearClipDepth<T>(clipDepth, depthDirection);
        const T farDepth = detail::FarClipDepth<T>(clipDepth, depthDirection);
        const T handednessSign = handedness == Handedness::Left ? T(1) : T(-1);
        const T depthPerDistance =
            (farDepth - nearDepth) / (farPlane - nearPlane);
        const T depthScale = handednessSign * depthPerDistance;
        const T depthTranslation = nearDepth - depthPerDistance * nearPlane;

        const T inverseWidth = T(1) / (right - left);
        const T inverseHeight = T(1) / (top - bottom);
        const Mat4<T> result(T(2) * inverseWidth, T(0), T(0), T(0), T(0),
                             T(2) * inverseHeight, T(0), T(0), T(0), T(0),
                             depthScale, T(0), -(right + left) * inverseWidth,
                             -(top + bottom) * inverseHeight, depthTranslation,
                             T(1));
        return result.IsFinite() ? std::optional<Mat4<T>>(result)
                                 : std::nullopt;
    }
}  // namespace mv::math
