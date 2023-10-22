#pragma once

#include <rtm/macros.h>

#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <rtm/quatd.h>
#include <rtm/quatf.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#define MVM_TYPE_WRAPPER(name, vector_type) \
    struct name                             \
    {                                       \
        using type = vector_type;           \
    };

namespace move::vectormath::wrappers
{
    MVM_TYPE_WRAPPER(v4f, rtm::vector4f);
    MVM_TYPE_WRAPPER(v4d, rtm::vector4d);
    MVM_TYPE_WRAPPER(m3x4f, rtm::matrix3x4f);
    MVM_TYPE_WRAPPER(m3x4d, rtm::matrix3x4d);
    MVM_TYPE_WRAPPER(m4x4f, rtm::matrix4x4f);
    MVM_TYPE_WRAPPER(m4x4d, rtm::matrix4x4d);
    MVM_TYPE_WRAPPER(qf, rtm::quatf);
    MVM_TYPE_WRAPPER(qd, rtm::quatd);
}  // namespace move::vectormath::wrappers