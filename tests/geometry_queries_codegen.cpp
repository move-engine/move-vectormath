#include <cstddef>
#include <cstdint>

#include <mv/math/Queries.hpp>

extern "C" void mv_intersect_aabbs(const mv::math::Ray3f* ray,
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

extern "C" void mv_intersect_triangles(const mv::math::Ray3f* rays,
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

extern "C" void mv_closest_points_on_segments(
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

extern "C" void mv_segment_pair_distances(const mv::math::Segment3f* first,
                                          const mv::math::Segment3f* second,
                                          float* output,
                                          std::size_t count)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = mv::math::DistanceSquared(first[index], second[index]);
    }
}

extern "C" void mv_intersect_capsules(const mv::math::Capsule3f* first,
                                      const mv::math::Capsule3f* second,
                                      std::uint8_t* output,
                                      std::size_t count)
{
    for (std::size_t index = 0; index < count; ++index)
    {
        output[index] = static_cast<std::uint8_t>(
            mv::math::Intersects(first[index], second[index]));
    }
}
