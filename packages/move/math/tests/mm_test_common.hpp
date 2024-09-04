#pragma once

#include <cstring>
#include "move/string.hpp"
#define REPEAT_FOR_EACH_TYPE_WRAPPER(op, vectype)                \
    template <move::math::Acceleration Accel, typename... Types> \
    inline void op##_multi()                                     \
    {                                                            \
        (op<vectype<Types, Accel>>(), ...);                      \
    }

#define REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(op, tgtype) \
    template <typename... Types>                         \
    inline void op##_multi()                             \
    {                                                    \
        (op<tgtype<Types>>(), ...);                      \
    }

inline const char* alloc_appended_name(move::string_view lhs,
                                       move::string_view rhs)
{
    size_t len = lhs.size() + rhs.size() + 1;
    char* result = (char*)movemm::alloc(len);
    std::memcpy((void*)result, lhs.data(), lhs.size());
    std::memcpy((void*)(result + lhs.size()), rhs.data(), rhs.size());
    result[len - 1] = '\0';
    return result;
}