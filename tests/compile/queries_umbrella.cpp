#include <mv/math/Queries.hpp>

bool QueriesUmbrellaRayAabb(const mv::math::Ray3f& ray,
                            const mv::math::Aabb3f& box) noexcept
{
    return mv::math::Intersects(ray, box);
}
