#pragma once

#include <rtm/macros.h>

#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#define MVM_VECTOR_TYPE_WRAPPER(name, vector_type) \
    struct name                                    \
    {                                              \
        using type = vector_type;                  \
    };

namespace move::vectormath::wrappers
{
    MVM_VECTOR_TYPE_WRAPPER(v4fw, rtm::vector4f);
    MVM_VECTOR_TYPE_WRAPPER(v4dw, rtm::vector4d);
}  // namespace move::vectormath::wrappers