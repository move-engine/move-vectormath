#include <mv/math/geometry/Aabb3.hpp>
#include <mv/math/geometry/Ray3.hpp>
#include <mv/math/queries/BoundsQueries.hpp>

bool PhaseCRayAabb(const mv::math::Ray3f& ray,
                   const mv::math::Aabb3f& box) noexcept
{
    return mv::math::Intersects(ray, box);
}
