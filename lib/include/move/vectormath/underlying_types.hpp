#pragma once

#include <rtm/macros.h>

#define MVM_RTM_WRAPPER(name, type)                  \
    struct name                                      \
    {                                                \
        type v;                                      \
        inline operator type&() noexcept             \
        {                                            \
            return v;                                \
        }                                            \
        inline operator const type&() const noexcept \
        {                                            \
            return v;                                \
        }                                            \
        inline name& operator=(const type& other)    \
        {                                            \
            v = other;                               \
            return *this;                            \
        }                                            \
        name() = default;                            \
        name(const name&) = default;                 \
    }
