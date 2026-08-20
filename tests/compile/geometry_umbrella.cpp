#include <mv/math/Geometry.hpp>

mv::math::Point3f GeometryUmbrellaRayPoint(const mv::math::Ray3f& ray,
                                           float distance) noexcept
{
    return ray.PointAt(distance);
}
