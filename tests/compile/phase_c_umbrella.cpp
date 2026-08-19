#include <mv/math/PhaseC.hpp>

bool PhaseCSpatial(const mv::math::Ray3f& ray,
                   const mv::math::Triangle3f& triangle) noexcept
{
    return mv::math::Intersects(ray, triangle);
}
