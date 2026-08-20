#include <mv/math/geometry/Ray3.hpp>

mv::math::Point3f PointAlongRay(const mv::math::Ray3f& ray,
                                float distance) noexcept
{
    return ray.PointAt(distance);
}
