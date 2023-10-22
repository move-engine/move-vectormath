#pragma once

#include <rtm/macros.h>
#include <rtm/scalard.h>

namespace move::vectormath
{
    inline float deg_to_rad(float deg)
    {
        return rtm::scalar_deg_to_rad(deg);
    }

    inline float rad_to_deg(float rad)
    {
        return rtm::scalar_rad_to_deg(rad);
    }
}  // namespace move::vectormath