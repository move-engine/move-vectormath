#include <mv/math/semantic/Point3.hpp>

mv::math::Point3f TranslatePoint(mv::math::Point3f point,
                                 mv::math::Vec3f displacement)
{
    return point + displacement;
}
