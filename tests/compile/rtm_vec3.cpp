#include <rtm/vector4f.h>

extern "C" rtm::vector4f CompileFixtureRtmVec3(rtm::vector4f left,
                                               rtm::vector4f right,
                                               float scale) noexcept
{
    return rtm::vector_mul(rtm::vector_add(left, right), scale);
}
