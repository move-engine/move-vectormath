#include <mv/math/geometry/Segment3.hpp>
#include <mv/math/queries/ClosestPointQueries.hpp>

mv::math::PointSegmentClosest3f PhaseCClosestPoint(
    const mv::math::Point3f& point, const mv::math::Segment3f& segment) noexcept
{
    return mv::math::ClosestPoints(point, segment);
}

mv::math::SegmentSegmentClosest3f PhaseCSegmentPair(
    const mv::math::Segment3f& first,
    const mv::math::Segment3f& second) noexcept
{
    return mv::math::ClosestPoints(first, second);
}

mv::math::PointCapsuleClosest3f PhaseCCapsulePoint(
    const mv::math::Point3f& point, const mv::math::Capsule3f& capsule) noexcept
{
    return mv::math::ClosestPoints(point, capsule);
}
