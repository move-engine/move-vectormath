#pragma once

#include <cstddef>
#include <cstring>
#include <type_traits>
#include <vector>
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

template <typename T>
struct capture_archive
{
    using is_loading = std::false_type;
    std::vector<T> values;

    template <typename U, std::size_t N>
    void operator()(U (&arr)[N])
    {
        values.clear();
        values.reserve(N);
        for (std::size_t i = 0; i < N; ++i)
        {
            values.push_back(static_cast<T>(arr[i]));
        }
    }

    template <typename... Args>
    void operator()(Args&... args)
    {
        values.clear();
        values.reserve(sizeof...(Args));
        (values.push_back(static_cast<T>(args)), ...);
    }
};

template <typename T>
struct replay_archive
{
    using is_loading = std::true_type;
    const std::vector<T>& values;

    template <typename U, std::size_t N>
    void operator()(U (&arr)[N])
    {
        for (std::size_t i = 0; i < N; ++i)
        {
            arr[i] = static_cast<U>(values.at(i));
        }
    }

    template <typename... Args>
    void operator()(Args&... args)
    {
        std::size_t index = 0;
        ((args = static_cast<std::remove_reference_t<Args>>(
              values.at(index++))),
         ...);
    }
};
