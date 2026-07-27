#include <mv/math/semantic/Point3.hpp>

mv::math::Point3f MovePoint(mv::math::Point3f point,
                            mv::math::Vec3f displacement)
{
    return point + displacement;
}
