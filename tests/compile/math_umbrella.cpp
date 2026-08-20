#include <mv/math/Math.hpp>

bool SpatialQuery(const mv::math::Ray3f& ray,
                  const mv::math::Triangle3f& triangle) noexcept
{
    return mv::math::Intersects(ray, triangle);
}
