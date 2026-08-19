#pragma once

#include <cmath>
#include <optional>

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

    struct RayTriangleOptions
    {
        BackFaceMode BackFaces = BackFaceMode::Include;
        float ParallelTolerance = 1.0e-7F;
    };

    namespace detail
    {
        struct RayPlaneSolution
        {
            float Distance;
            float Denominator;
        };

        // Standard plane-equation solve; cross-checked against GLM
        // intersectRayPlane (MIT). Move accepts t=0 and rejects non-finite
        // hits.
        [[nodiscard]] inline std::optional<RayPlaneSolution>
        TryIntersectRayPlane(const Ray3f& ray,
                             const Plane3f& plane,
                             float parallelTolerance) noexcept
        {
            const float tolerance = std::abs(parallelTolerance);
            if (!ray.IsFinite() || !std::isfinite(tolerance))
            {
                return std::nullopt;
            }

            const float denominator =
                Dot(plane.Normal().Vector(), ray.Direction().Vector());
            if (!std::isfinite(denominator) ||
                std::abs(denominator) <= tolerance)
            {
                return std::nullopt;
            }

            const float distance =
                -plane.SignedDistance(ray.Origin()) / denominator;
            if (!(distance >= 0.0F) || !std::isfinite(distance))
            {
                return std::nullopt;
            }
            return RayPlaneSolution{distance, denominator};
        }

        struct RayTriangleSolution
        {
            float Distance;
            float SecondWeight;
            float ThirdWeight;
            float Determinant;
        };

        // Moller-Trumbore ray/triangle test (JGT 1997,
        // doi:10.1080/10867651.1997.10487468); Move adds explicit culling,
        // finite-input, tolerance, and boundary policy.
        [[nodiscard]] inline std::optional<RayTriangleSolution>
        TryIntersectRayTriangle(const Ray3f& ray,
                                const Triangle3f& triangle,
                                RayTriangleOptions options) noexcept
        {
            const float tolerance = std::abs(options.ParallelTolerance);
            if (!ray.IsFinite() || !triangle.IsFinite() ||
                !std::isfinite(tolerance))
            {
                return std::nullopt;
            }

            const Vec3f edge01 = triangle.Edge01();
            const Vec3f edge02 = triangle.Edge02();
            const Vec3f directionCrossEdge02 =
                Cross(ray.Direction().Vector(), edge02);
            const float determinant = Dot(edge01, directionCrossEdge02);
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

            const float reciprocalDeterminant = 1.0F / determinant;
            const Vec3f fromFirst = ray.Origin() - triangle.First();
            const float secondWeight =
                Dot(fromFirst, directionCrossEdge02) * reciprocalDeterminant;
            if (!(secondWeight >= 0.0F && secondWeight <= 1.0F))
            {
                return std::nullopt;
            }

            const Vec3f fromFirstCrossEdge01 = Cross(fromFirst, edge01);
            const float thirdWeight =
                Dot(ray.Direction().Vector(), fromFirstCrossEdge01) *
                reciprocalDeterminant;
            if (!(thirdWeight >= 0.0F && secondWeight + thirdWeight <= 1.0F))
            {
                return std::nullopt;
            }

            const float distance =
                Dot(edge02, fromFirstCrossEdge01) * reciprocalDeterminant;
            if (!(distance >= 0.0F) || !std::isfinite(distance))
            {
                return std::nullopt;
            }

            return RayTriangleSolution{distance, secondWeight, thirdWeight,
                                       determinant};
        }
    }  // namespace detail

    [[nodiscard]] inline std::optional<RayPlaneHit3f> Intersect(
        const Ray3f& ray,
        const Plane3f& plane,
        float parallelTolerance = 1.0e-7F) noexcept
    {
        const auto solution =
            detail::TryIntersectRayPlane(ray, plane, parallelTolerance);
        if (!solution)
        {
            return std::nullopt;
        }
        return RayPlaneHit3f{
            solution->Distance, ray.PointAt(solution->Distance), plane.Normal(),
            solution->Denominator < 0.0F ? FaceOrientation::Front
                                         : FaceOrientation::Back};
    }

    [[nodiscard]] inline bool Intersects(
        const Ray3f& ray,
        const Plane3f& plane,
        float parallelTolerance = 1.0e-7F) noexcept
    {
        return detail::TryIntersectRayPlane(ray, plane, parallelTolerance)
            .has_value();
    }

    [[nodiscard]] inline std::optional<RayTriangleHit3f> Intersect(
        const Ray3f& ray,
        const Triangle3f& triangle,
        RayTriangleOptions options = {}) noexcept
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

        return RayTriangleHit3f{
            solution->Distance, ray.PointAt(solution->Distance), *normal,
            Vec3f(1.0F - solution->SecondWeight - solution->ThirdWeight,
                  solution->SecondWeight, solution->ThirdWeight),
            solution->Determinant > 0.0F ? FaceOrientation::Front
                                         : FaceOrientation::Back};
    }

    [[nodiscard]] inline bool Intersects(
        const Ray3f& ray,
        const Triangle3f& triangle,
        RayTriangleOptions options = {}) noexcept
    {
        return detail::TryIntersectRayTriangle(ray, triangle, options)
            .has_value();
    }
}  // namespace mv::math
