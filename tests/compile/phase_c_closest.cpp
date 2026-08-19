#include <mv/math/geometry/Segment3.hpp>
#include <mv/math/queries/ClosestPointQueries.hpp>

mv::math::PointSegmentClosest3f PhaseCClosestPoint(
    const mv::math::Point3f& point, const mv::math::Segment3f& segment) noexcept
{
    return mv::math::ClosestPoints(point, segment);
}
