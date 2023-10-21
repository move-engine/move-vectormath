#pragma once

// If <move/error.hpp> exists, include that instead of defining our own
// error handling macros.
#if __has_include(<move/error.hpp>)
#include <move/error.hpp>
#else
#include <fmt/format.h>

#ifndef MOVE_WINDOWS
#ifdef _WIN32
#define MOVE_WINDOWS
#endif
#endif

void move_break_execution(const char* file, int line, const char* reason);
bool move_attach_debugger();
bool move_debug_break();

#if defined(MOVE_WINDOWS)
#define __MOVE_ERR_VA_ARGS(...) , __VA_ARGS__
#else
#define __MOVE_ERR_VA_ARGS(...) __VA_OPT__(, ) __VA_ARGS__
#endif

#define MOVE_ERROR(error, ...)                                               \
    {                                                                        \
        auto formatted = fmt::format(error __MOVE_ERR_VA_ARGS(__VA_ARGS__)); \
        move_break_execution(__FILE__, __LINE__, formatted.c_str());         \
    }

#define MOVE_ERROR_IF(condition, error_string, ...)               \
    if (condition)                                                \
    {                                                             \
        MOVE_ERROR(error_string __MOVE_ERR_VA_ARGS(__VA_ARGS__)); \
    }

#define MOVE_REL_ASSERT(condition, ...)                          \
    MOVE_ERROR_IF(!(condition), "Assertion failed: " #condition, \
        __MOVE_ERR_VA_ARGS(__VA_ARGS__))

#define MOVE_FATAL_ERROR(error, ...) \
    MOVE_ERROR(error __MOVE_ERR_VA_ARGS(__VA_ARGS__))

#define MOVE_FATAL_ERROR_IF(condition, error_string, ...)               \
    if (condition)                                                      \
    {                                                                   \
        MOVE_FATAL_ERROR(error_string __MOVE_ERR_VA_ARGS(__VA_ARGS__)); \
    }

#if defined(NDEBUG)
#define MOVE_ASSERT(condition)
#define MOVE_ASSERT_MSG(condition, msg)
#define MOVE_RUN_DEBUG_ONLY(...)
#define MOVE_RUN_RELEASE_ONLY(run) run

#else
#define MOVE_ASSERT(condition) MOVE_REL_ASSERT(condition)
#define MOVE_ASSERT_MSG(condition, msg, ...) \
    MOVE_FATAL_ERROR_IF(!(condition), msg __MOVE_ERR_VA_ARGS(__VA_ARGS__))

#define MOVE_RUN_DEBUG_ONLY(run) run

#define MOVE_RUN_RELEASE_ONLY(...)
#endif

#if !defined(assert)
#define assert(...) MOVE_ASSER(__MOVE_ERR_VA_ARGS(__VA_ARGS__))
#endif

#define MOVE_DEBUG() move_attach_debugger()
#define MOVE_DEBUG_BREAK() move_debug_break()

/*
#if defined(MOVE_WINDOWS) && !defined(NDEBUG)
#include <yvals.h>

#if defined(_STL_REPORT_ERROR)
#undef _STL_REPORT_ERROR
#endif

#define _STL_REPORT_ERROR(mesg) MOVE_ERROR(mesg)
#endif
*/
#endif