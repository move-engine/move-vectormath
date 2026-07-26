#pragma once

#include <cstddef>
#include <string>
#include <string_view>
#include <type_traits>
#include <typeinfo>
#include <vector>

namespace mvm_test
{
    template <typename T>
    inline const char* type_name()
    {
        return typeid(T).name();
    }
}  // namespace mvm_test

#define REPEAT_FOR_EACH_TYPE_WRAPPER(op, vectype)                \
    template <move::math::Acceleration Accel, typename Type>     \
    inline void op##_one()                                       \
    {                                                            \
        using tested_type = vectype<Type, Accel>;                \
        DYNAMIC_SECTION(mvm_test::type_name<tested_type>())      \
        {                                                        \
            op<tested_type>();                                   \
        }                                                        \
    }                                                            \
    template <move::math::Acceleration Accel, typename... Types> \
    inline void op##_multi()                                     \
    {                                                            \
        (op##_one<Accel, Types>(), ...);                         \
    }

#define REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(op, tgtype)    \
    template <typename Type>                                \
    inline void op##_one()                                  \
    {                                                       \
        using tested_type = tgtype<Type>;                   \
        DYNAMIC_SECTION(mvm_test::type_name<tested_type>()) \
        {                                                   \
            op<tested_type>();                              \
        }                                                   \
    }                                                       \
    template <typename... Types>                            \
    inline void op##_multi()                                \
    {                                                       \
        (op##_one<Types>(), ...);                           \
    }

inline std::string alloc_appended_name(std::string_view lhs,
                                       std::string_view rhs)
{
    std::string result(lhs);
    result.append(rhs);
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
        ((args =
              static_cast<std::remove_reference_t<Args>>(values.at(index++))),
         ...);
    }
};
