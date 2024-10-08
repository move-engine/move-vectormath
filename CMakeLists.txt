cmake_minimum_required(VERSION 3.14)

include(cmake/cpm.cmake)

project(
    move-vectormath
    VERSION 0.2.0
    DESCRIPTION "The vectormath module of the Move engine"
    HOMEPAGE_URL "https://github.com/move-engine/move-vectormath"
    LANGUAGES NONE
)

# Dependencies
add_subdirectory(packages/cmake/rtm)

# Library
add_library(move-vectormath INTERFACE)
target_include_directories(move-vectormath INTERFACE packages/move/math/include)

# Aliases
add_library(move::vectormath ALIAS move-vectormath)
add_library(move::math ALIAS move-vectormath)
add_library(move-math ALIAS move-vectormath)