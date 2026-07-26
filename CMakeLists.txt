cmake_minimum_required(VERSION 3.14)

include(cmake/cpm.cmake)

project(
    move-vectormath
    VERSION 0.2.0
    DESCRIPTION "The vectormath module of the Move engine"
    HOMEPAGE_URL "https://github.com/move-engine/move-vectormath"
    LANGUAGES CXX
)

# Dependencies
add_subdirectory(packages/cmake/rtm)

# Library
add_library(move-vectormath INTERFACE)
target_include_directories(move-vectormath INTERFACE packages/move/math/include)
target_link_libraries(move-vectormath INTERFACE rtm)
target_compile_features(move-vectormath INTERFACE cxx_std_20)

option(MOVE_VECTORMATH_BUILD_STANDALONE_TESTS
       "Build dependency-light public API and numerical contract tests" OFF)
if(MOVE_VECTORMATH_BUILD_STANDALONE_TESTS)
    enable_testing()
    add_executable(move-vectormath-standalone-tests
                   tests/standalone.cpp)
    target_link_libraries(move-vectormath-standalone-tests
                          PRIVATE move-vectormath)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(move-vectormath-standalone-tests
                               PRIVATE -Wall -Wextra -Wconversion)
    elseif(MSVC)
        target_compile_options(move-vectormath-standalone-tests PRIVATE /W4)
    endif()
    add_test(NAME move-vectormath-standalone
             COMMAND move-vectormath-standalone-tests)
endif()

# Aliases
add_library(move::vectormath ALIAS move-vectormath)
add_library(move::math ALIAS move-vectormath)
add_library(move-math ALIAS move-vectormath)
