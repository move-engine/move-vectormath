#pragma once

#include <cmath>
#include <limits>
#include <optional>
#include <type_traits>

#include <mv/math/geometry/Plane3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/geometry/Triangle3.hpp>
#include <mv/math/queries/QueryTypes.hpp>

namespace mv::math
{
    enum class BackFaceMode
    {
        Include,
        Cull
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RayTriangleOptions
    {
        BackFaceMode BackFaces = BackFaceMode::Include;
        T ParallelTolerance = std::numeric_limits<T>::epsilon() * T(8);
    };

    using RayTriangleOptionsf = RayTriangleOptions<float>;
    using RayTriangleOptionsd = RayTriangleOptions<double>;

    namespace detail
    {
        template <typename T>
        struct RayPlaneSolution
        {
            T Distance;
            T Denominator;
        };

        // Standard plane-equation solve; cross-checked against GLM
        // intersectRayPlane (MIT). Move accepts t=0 and rejects non-finite
        // hits.
        template <typename T>
        [[nodiscard]] inline std::optional<RayPlaneSolution<T>>
        TryIntersectRayPlane(const Ray3<T>& ray,
                             const Plane3<T>& plane,
                             T parallelTolerance) noexcept
        {
            const T tolerance = std::abs(parallelTolerance);
            if (!std::isfinite(tolerance))
            {
                return std::nullopt;
            }

            const T denominator =
                Dot(plane.Normal().Vector(), ray.Direction().Vector());
            if (!std::isfinite(denominator) ||
                std::abs(denominator) <= tolerance)
            {
                return std::nullopt;
            }

            const T distance =
                -plane.SignedDistance(ray.Origin()) / denominator;
            if (!(distance >= T(0)) || !std::isfinite(distance))
            {
                return std::nullopt;
            }
            return RayPlaneSolution<T>{distance, denominator};
        }

        template <typename T>
        struct RayTriangleSolution
        {
            T Distance;
            T SecondWeight;
            T ThirdWeight;
            T Determinant;
        };

        // Moller-Trumbore ray/triangle test (JGT 1997,
        // doi:10.1080/10867651.1997.10487468); Move adds explicit culling,
        // finite-construction, tolerance, and boundary policy.
        template <typename T>
        [[nodiscard]] inline std::optional<RayTriangleSolution<T>>
        TryIntersectRayTriangle(const Ray3<T>& ray,
                                const Triangle3<T>& triangle,
                                RayTriangleOptions<T> options) noexcept
        {
            const T tolerance = std::abs(options.ParallelTolerance);
            if (!std::isfinite(tolerance))
            {
                return std::nullopt;
            }

            const Vec3<T> edge01 = triangle.Edge01();
            const Vec3<T> edge02 = triangle.Edge02();
            const Vec3<T> directionCrossEdge02 =
                Cross(ray.Direction().Vector(), edge02);
            const T determinant = Dot(edge01, directionCrossEdge02);
            if (!std::isfinite(determinant))
            {
                return std::nullopt;
            }

            if (options.BackFaces == BackFaceMode::Cull)
            {
                if (!(determinant > tolerance))
                {
                    return std::nullopt;
                }
            }
            else if (std::abs(determinant) <= tolerance)
            {
                return std::nullopt;
            }

            const T reciprocalDeterminant = T(1) / determinant;
            const Vec3<T> fromFirst = ray.Origin() - triangle.First();
            const T secondWeight =
                Dot(fromFirst, directionCrossEdge02) * reciprocalDeterminant;
            if (!(secondWeight >= T(0) && secondWeight <= T(1)))
            {
                return std::nullopt;
            }

            const Vec3<T> fromFirstCrossEdge01 = Cross(fromFirst, edge01);
            const T thirdWeight =
                Dot(ray.Direction().Vector(), fromFirstCrossEdge01) *
                reciprocalDeterminant;
            if (!(thirdWeight >= T(0) && secondWeight + thirdWeight <= T(1)))
            {
                return std::nullopt;
            }

            const T distance =
                Dot(edge02, fromFirstCrossEdge01) * reciprocalDeterminant;
            if (!(distance >= T(0)) || !std::isfinite(distance))
            {
                return std::nullopt;
            }

            return RayTriangleSolution<T>{distance, secondWeight, thirdWeight,
                                          determinant};
        }
    }  // namespace detail

    template <typename T>
    [[nodiscard]] inline std::optional<RayPlaneHit3<T>> Intersect(
        const Ray3<T>& ray,
        const Plane3<T>& plane,
        T parallelTolerance = std::numeric_limits<T>::epsilon() * T(8)) noexcept
    {
        const auto solution =
            detail::TryIntersectRayPlane(ray, plane, parallelTolerance);
        if (!solution)
        {
            return std::nullopt;
        }
        return RayPlaneHit3<T>{
            solution->Distance, ray.PointAt(solution->Distance), plane.Normal(),
            solution->Denominator < T(0) ? FaceOrientation::Front
                                         : FaceOrientation::Back};
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(
        const Ray3<T>& ray,
        const Plane3<T>& plane,
        T parallelTolerance = std::numeric_limits<T>::epsilon() * T(8)) noexcept
    {
        return detail::TryIntersectRayPlane(ray, plane, parallelTolerance)
            .has_value();
    }

    template <typename T>
    [[nodiscard]] inline std::optional<RayTriangleHit3<T>> Intersect(
        const Ray3<T>& ray,
        const Triangle3<T>& triangle,
        RayTriangleOptions<T> options = {}) noexcept
    {
        const auto solution =
            detail::TryIntersectRayTriangle(ray, triangle, options);
        if (!solution)
        {
            return std::nullopt;
        }

        const auto normal = triangle.TryNormal();
        if (!normal)
        {
            return std::nullopt;
        }

        return RayTriangleHit3<T>{
            solution->Distance, ray.PointAt(solution->Distance), *normal,
            Vec3<T>(T(1) - solution->SecondWeight - solution->ThirdWeight,
                    solution->SecondWeight, solution->ThirdWeight),
            solution->Determinant > T(0) ? FaceOrientation::Front
                                         : FaceOrientation::Back};
    }

    template <typename T>
    [[nodiscard]] inline bool Intersects(
        const Ray3<T>& ray,
        const Triangle3<T>& triangle,
        RayTriangleOptions<T> options = {}) noexcept
    {
        return detail::TryIntersectRayTriangle(ray, triangle, options)
            .has_value();
    }
}  // namespace mv::math
