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

function(move_vectormath_enable_strict_warnings target)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        target_compile_options(${target} PRIVATE -Wall -Wextra -Wconversion)
        if(MOVE_VECTORMATH_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE -Werror)
        endif()
    elseif(MSVC)
        target_compile_options(${target} PRIVATE /W4)
        if(MOVE_VECTORMATH_WARNINGS_AS_ERRORS)
            target_compile_options(${target} PRIVATE /WX)
        endif()
    endif()
endfunction()

option(MOVE_VECTORMATH_WARNINGS_AS_ERRORS
       "Treat warnings in vectormath-owned test targets as errors" OFF)
option(MOVE_VECTORMATH_BUILD_TESTS "Build the complete Catch2 test suite" OFF)
option(MOVE_VECTORMATH_BUILD_STANDALONE_TESTS
       "Build dependency-light public API and numerical contract tests" OFF)
option(MOVE_VECTORMATH_BUILD_REPRESENTATION_TESTS
       "Build scalar and RTM core math and representation tests" OFF)
option(MOVE_VECTORMATH_BUILD_SEMANTIC_TRANSFORM_TESTS
       "Build semantic transform tests" OFF)
option(MOVE_VECTORMATH_BUILD_GEOMETRY_QUERY_TESTS
       "Build geometry and spatial query tests" OFF)
option(MOVE_VECTORMATH_BUILD_MATRIX_TESTS
       "Build general matrix tests" OFF)

if(MOVE_VECTORMATH_BUILD_TESTS OR
   MOVE_VECTORMATH_BUILD_STANDALONE_TESTS OR
   MOVE_VECTORMATH_BUILD_REPRESENTATION_TESTS OR
   MOVE_VECTORMATH_BUILD_SEMANTIC_TRANSFORM_TESTS OR
   MOVE_VECTORMATH_BUILD_GEOMETRY_QUERY_TESTS OR
   MOVE_VECTORMATH_BUILD_MATRIX_TESTS)
    enable_testing()
endif()

if(MOVE_VECTORMATH_BUILD_MATRIX_TESTS)
    add_executable(move-vectormath-matrix-rtm-tests tests/matrices.cpp)
    target_link_libraries(move-vectormath-matrix-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(move-vectormath-matrix-rtm-tests)
    add_test(NAME move-vectormath-matrix-rtm
             COMMAND move-vectormath-matrix-rtm-tests)

    add_executable(move-vectormath-matrix-scalar-tests tests/matrices.cpp)
    target_link_libraries(move-vectormath-matrix-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-matrix-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(move-vectormath-matrix-scalar-tests)
    add_test(NAME move-vectormath-matrix-scalar
             COMMAND move-vectormath-matrix-scalar-tests)

    add_library(move-vectormath-matrix-rtm-codegen OBJECT
                tests/matrices_codegen.cpp)
    target_link_libraries(move-vectormath-matrix-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(move-vectormath-matrix-rtm-codegen)

    add_library(move-vectormath-matrix-scalar-codegen OBJECT
                tests/matrices_codegen.cpp)
    target_link_libraries(move-vectormath-matrix-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-matrix-scalar-codegen
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-matrix-scalar-codegen)
endif()

if(MOVE_VECTORMATH_BUILD_GEOMETRY_QUERY_TESTS)
    add_executable(move-vectormath-geometry-query-rtm-tests
                   tests/geometry_queries.cpp)
    target_link_libraries(move-vectormath-geometry-query-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-geometry-query-rtm-tests)
    add_test(NAME move-vectormath-geometry-query-rtm
             COMMAND move-vectormath-geometry-query-rtm-tests)

    add_executable(move-vectormath-geometry-query-scalar-tests
                   tests/geometry_queries.cpp)
    target_link_libraries(move-vectormath-geometry-query-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-geometry-query-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-geometry-query-scalar-tests)
    add_test(NAME move-vectormath-geometry-query-scalar
             COMMAND move-vectormath-geometry-query-scalar-tests)

    add_library(move-vectormath-geometry-query-rtm-codegen OBJECT
                tests/geometry_queries_codegen.cpp)
    target_link_libraries(move-vectormath-geometry-query-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-geometry-query-rtm-codegen)

    add_library(move-vectormath-geometry-query-scalar-codegen OBJECT
                tests/geometry_queries_codegen.cpp)
    target_link_libraries(move-vectormath-geometry-query-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-geometry-query-scalar-codegen
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-geometry-query-scalar-codegen)
endif()

if(MOVE_VECTORMATH_BUILD_REPRESENTATION_TESTS)
    add_executable(move-vectormath-representation-rtm-tests
                   tests/representation.cpp)
    target_link_libraries(move-vectormath-representation-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-representation-rtm-tests)
    add_test(NAME move-vectormath-representation-rtm
             COMMAND move-vectormath-representation-rtm-tests)

    add_executable(move-vectormath-representation-scalar-tests
                   tests/representation.cpp)
    target_link_libraries(move-vectormath-representation-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-representation-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-representation-scalar-tests)
    add_test(NAME move-vectormath-representation-scalar
             COMMAND move-vectormath-representation-scalar-tests)

    add_library(move-vectormath-representation-rtm-codegen OBJECT
                tests/representation_codegen.cpp)
    target_link_libraries(move-vectormath-representation-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-representation-rtm-codegen)

    add_library(move-vectormath-representation-scalar-codegen OBJECT
                tests/representation_codegen.cpp)
    target_link_libraries(move-vectormath-representation-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-representation-scalar-codegen
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-representation-scalar-codegen)
endif()

if(MOVE_VECTORMATH_BUILD_SEMANTIC_TRANSFORM_TESTS)
    add_executable(move-vectormath-semantic-transform-rtm-tests
                   tests/semantic_transforms.cpp)
    target_link_libraries(move-vectormath-semantic-transform-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-semantic-transform-rtm-tests)
    add_test(NAME move-vectormath-semantic-transform-rtm
             COMMAND move-vectormath-semantic-transform-rtm-tests)

    add_executable(move-vectormath-semantic-transform-scalar-tests
                   tests/semantic_transforms.cpp)
    target_link_libraries(move-vectormath-semantic-transform-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-semantic-transform-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-semantic-transform-scalar-tests)
    add_test(NAME move-vectormath-semantic-transform-scalar
             COMMAND move-vectormath-semantic-transform-scalar-tests)

    add_library(move-vectormath-semantic-transform-rtm-codegen OBJECT
                tests/semantic_transforms_codegen.cpp)
    target_link_libraries(move-vectormath-semantic-transform-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-semantic-transform-rtm-codegen)

    add_library(move-vectormath-semantic-transform-scalar-codegen OBJECT
                tests/semantic_transforms_codegen.cpp)
    target_link_libraries(move-vectormath-semantic-transform-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(
        move-vectormath-semantic-transform-scalar-codegen
        PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-semantic-transform-scalar-codegen)
endif()

if(MOVE_VECTORMATH_BUILD_TESTS)
    CPMAddPackage(
        NAME Catch2
        GITHUB_REPOSITORY catchorg/Catch2
        VERSION 3.5.2
        GIT_TAG v3.5.2
        OPTIONS
            "CATCH_INSTALL_DOCS OFF"
            "CATCH_INSTALL_EXTRAS OFF"
    )

    add_executable(
        move-vectormath-tests
        packages/move/math/tests/mat3x3.cpp
        packages/move/math/tests/mat4x4.cpp
        packages/move/math/tests/quat.cpp
        packages/move/math/tests/smoke.cpp
        packages/move/math/tests/vec2.cpp
        packages/move/math/tests/vec3.cpp
        packages/move/math/tests/vec4.cpp
    )
    target_link_libraries(move-vectormath-tests
                          PRIVATE move-vectormath Catch2::Catch2WithMain)
    move_vectormath_enable_strict_warnings(move-vectormath-tests)
    if(CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
        # Catch2's assertion decomposition compares test literals inside its
        # own templates. The standalone target still checks the public API
        # with -Wconversion; suppress the third-party expansion noise here.
        target_compile_options(move-vectormath-tests PRIVATE -Wno-conversion)
    endif()

    list(APPEND CMAKE_MODULE_PATH "${Catch2_SOURCE_DIR}/extras")
    include(Catch)
    catch_discover_tests(move-vectormath-tests DISCOVERY_MODE PRE_TEST)
endif()

if(MOVE_VECTORMATH_BUILD_STANDALONE_TESTS)
    add_executable(move-vectormath-standalone-tests
                   tests/standalone.cpp)
    target_link_libraries(move-vectormath-standalone-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(move-vectormath-standalone-tests)
    add_test(NAME move-vectormath-standalone
             COMMAND move-vectormath-standalone-tests)
endif()

# Aliases
add_library(move::vectormath ALIAS move-vectormath)
add_library(move::math ALIAS move-vectormath)
add_library(move-math ALIAS move-vectormath)
