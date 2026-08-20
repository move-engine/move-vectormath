#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Mat4.hpp>
#include <mv/math/Vec2.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/transforms/Projection.hpp>

namespace mv::math
{
    enum class ViewportYAxis
    {
        Up,
        Down
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    class Viewport
    {
    public:
        [[nodiscard]] static std::optional<Viewport> TryFrom(
            T x,
            T y,
            T width,
            T height,
            T minimumDepth = T(0),
            T maximumDepth = T(1)) noexcept
        {
            if (!(width > T(0)) || !(height > T(0)) ||
                !(maximumDepth > minimumDepth) || !std::isfinite(x) ||
                !std::isfinite(y) || !std::isfinite(width) ||
                !std::isfinite(height) || !std::isfinite(minimumDepth) ||
                !std::isfinite(maximumDepth))
            {
                return std::nullopt;
            }
            return Viewport(x, y, width, height, minimumDepth, maximumDepth,
                            UncheckedTag{});
        }

        [[nodiscard]] T X() const noexcept
        {
            return X_;
        }
        [[nodiscard]] T Y() const noexcept
        {
            return Y_;
        }
        [[nodiscard]] T Width() const noexcept
        {
            return Width_;
        }
        [[nodiscard]] T Height() const noexcept
        {
            return Height_;
        }
        [[nodiscard]] T MinimumDepth() const noexcept
        {
            return MinimumDepth_;
        }
        [[nodiscard]] T MaximumDepth() const noexcept
        {
            return MaximumDepth_;
        }

    private:
        struct UncheckedTag
        {
        };

        Viewport(T x,
                 T y,
                 T width,
                 T height,
                 T minimumDepth,
                 T maximumDepth,
                 UncheckedTag) noexcept :
            X_(x),
            Y_(y),
            Width_(width),
            Height_(height),
            MinimumDepth_(minimumDepth),
            MaximumDepth_(maximumDepth)
        {
        }

        T X_;
        T Y_;
        T Width_;
        T Height_;
        T MinimumDepth_;
        T MaximumDepth_;
    };

    using Viewportf = Viewport<float>;
    using Viewportd = Viewport<double>;

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>> TryProjectPoint(
        const Mat4<T>& worldToClip, const Point3<T>& worldPoint) noexcept
    {
        // Homogeneous projection follows the OpenGL/Direct3D pipeline and is
        // cross-checked against GLM project/unProject and DirectXMath
        // XMVector3Project/Unproject (both MIT). W failure is explicit.
        const Vec4<T> clip =
            Vec4<T>(worldPoint.X(), worldPoint.Y(), worldPoint.Z(), T(1)) *
            worldToClip;
        if (clip.W() == T(0) || !std::isfinite(clip.W()))
        {
            return std::nullopt;
        }
        const Point3<T> result(clip.X() / clip.W(), clip.Y() / clip.W(),
                               clip.Z() / clip.W());
        return std::isfinite(result.X()) && std::isfinite(result.Y()) &&
                       std::isfinite(result.Z())
                   ? std::optional<Point3<T>>(result)
                   : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>> TryUnprojectPointWithInverse(
        const Mat4<T>& clipToWorld,
        const Point3<T>& normalizedDevicePoint) noexcept
    {
        const Vec4<T> world =
            Vec4<T>(normalizedDevicePoint.X(), normalizedDevicePoint.Y(),
                    normalizedDevicePoint.Z(), T(1)) *
            clipToWorld;
        if (world.W() == T(0) || !std::isfinite(world.W()))
        {
            return std::nullopt;
        }
        const Point3<T> result(world.X() / world.W(), world.Y() / world.W(),
                               world.Z() / world.W());
        return std::isfinite(result.X()) && std::isfinite(result.Y()) &&
                       std::isfinite(result.Z())
                   ? std::optional<Point3<T>>(result)
                   : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>> TryUnprojectPoint(
        const Mat4<T>& worldToClip,
        const Point3<T>& normalizedDevicePoint) noexcept
    {
        const auto inverse = worldToClip.TryInverse();
        return inverse ? TryUnprojectPointWithInverse(*inverse,
                                                      normalizedDevicePoint)
                       : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>> TryProjectToViewport(
        const Mat4<T>& worldToClip,
        const Point3<T>& worldPoint,
        const Viewport<T>& viewport,
        ClipDepth clipDepth,
        ViewportYAxis yAxis = ViewportYAxis::Down) noexcept
    {
        const auto ndc = TryProjectPoint(worldToClip, worldPoint);
        if (!ndc)
        {
            return std::nullopt;
        }
        const T normalizedX = ndc->X() * T(0.5) + T(0.5);
        const T normalizedY = yAxis == ViewportYAxis::Down
                                  ? T(0.5) - ndc->Y() * T(0.5)
                                  : ndc->Y() * T(0.5) + T(0.5);
        const T normalizedDepth = clipDepth == ClipDepth::ZeroToOne
                                      ? ndc->Z()
                                      : ndc->Z() * T(0.5) + T(0.5);
        return Point3<T>(viewport.X() + normalizedX * viewport.Width(),
                         viewport.Y() + normalizedY * viewport.Height(),
                         viewport.MinimumDepth() +
                             normalizedDepth * (viewport.MaximumDepth() -
                                                viewport.MinimumDepth()));
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Point3<T>>
    TryUnprojectFromViewportWithInverse(
        const Mat4<T>& clipToWorld,
        const Point3<T>& viewportPoint,
        const Viewport<T>& viewport,
        ClipDepth clipDepth,
        ViewportYAxis yAxis = ViewportYAxis::Down) noexcept
    {
        const T normalizedX =
            (viewportPoint.X() - viewport.X()) / viewport.Width();
        const T normalizedY =
            (viewportPoint.Y() - viewport.Y()) / viewport.Height();
        const T normalizedDepth =
            (viewportPoint.Z() - viewport.MinimumDepth()) /
            (viewport.MaximumDepth() - viewport.MinimumDepth());
        const T ndcX = normalizedX * T(2) - T(1);
        const T ndcY = yAxis == ViewportYAxis::Down ? T(1) - normalizedY * T(2)
                                                    : normalizedY * T(2) - T(1);
        const T ndcZ = clipDepth == ClipDepth::ZeroToOne
                           ? normalizedDepth
                           : normalizedDepth * T(2) - T(1);
        return TryUnprojectPointWithInverse(clipToWorld,
                                            Point3<T>(ndcX, ndcY, ndcZ));
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Ray3<T>> TryMakeViewportRay(
        const Mat4<T>& clipToWorld,
        const Vec2<T>& viewportPosition,
        const Viewport<T>& viewport,
        ClipDepth clipDepth,
        DepthDirection depthDirection,
        ViewportYAxis yAxis = ViewportYAxis::Down) noexcept
    {
        const T nearDepth = depthDirection == DepthDirection::Forward
                                ? viewport.MinimumDepth()
                                : viewport.MaximumDepth();
        const T farDepth = depthDirection == DepthDirection::Forward
                               ? viewport.MaximumDepth()
                               : viewport.MinimumDepth();
        const T sampleDepth = nearDepth + (farDepth - nearDepth) * T(0.5);
        const auto nearPoint = TryUnprojectFromViewportWithInverse(
            clipToWorld,
            Point3<T>(viewportPosition.X(), viewportPosition.Y(), nearDepth),
            viewport, clipDepth, yAxis);
        const auto samplePoint = TryUnprojectFromViewportWithInverse(
            clipToWorld,
            Point3<T>(viewportPosition.X(), viewportPosition.Y(), sampleDepth),
            viewport, clipDepth, yAxis);
        if (!nearPoint || !samplePoint)
        {
            return std::nullopt;
        }
        const auto direction =
            Direction3<T>::TryFrom(*samplePoint - *nearPoint);
        return direction
                   ? Ray3<T>::TryFromOriginDirection(*nearPoint, *direction)
                   : std::nullopt;
    }

    template <typename T>
    [[nodiscard]] inline std::optional<Ray3<T>> TryMakePerspectiveViewportRay(
        const Mat4<T>& clipToWorld,
        const Point3<T>& cameraOrigin,
        const Vec2<T>& viewportPosition,
        const Viewport<T>& viewport,
        ClipDepth clipDepth,
        DepthDirection depthDirection,
        ViewportYAxis yAxis = ViewportYAxis::Down) noexcept
    {
        const auto nearRay =
            TryMakeViewportRay(clipToWorld, viewportPosition, viewport,
                               clipDepth, depthDirection, yAxis);
        if (!nearRay)
        {
            return std::nullopt;
        }
        const auto direction =
            Direction3<T>::TryFrom(nearRay->Origin() - cameraOrigin);
        return direction
                   ? Ray3<T>::TryFromOriginDirection(cameraOrigin, *direction)
                   : std::nullopt;
    }
}  // namespace mv::math
