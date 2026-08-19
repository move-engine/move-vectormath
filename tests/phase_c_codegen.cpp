#include <cstddef>
#include <cstdint>

#include <mv/math/PhaseC.hpp>

extern "C" void mv_phase_c_intersect_aabbs(const mv::math::Ray3f* ray,
                                           const mv::math::Aabb3f* boxes,
                                           std::uint8_t* results,
                                           std::size_t count)
{
    const mv::math::PreparedRay3f prepared(*ray);
    for (std::size_t index = 0; index < count; ++index)
    {
        results[index] = static_cast<std::uint8_t>(
            mv::math::Intersects(prepared, boxes[index]));
    }
}

extern "C" void mv_phase_c_intersect_triangles(
    const mv::math::Ray3f* rays,
    const mv::math::Triangle3f* triangles,
    std::uint8_t* results,
    std::size_t count)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        results[index] = static_cast<std::uint8_t>(
            mv::math::Intersects(rays[index], triangles[index]));
    }
}

extern "C" void mv_phase_c_closest_points_on_segments(
    const mv::math::Point3f* points,
    const mv::math::Segment3f* segments,
    mv::math::Point3f* output,
    std::size_t count)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = mv::math::ClosestPoint(points[index], segments[index]);
    }
}
