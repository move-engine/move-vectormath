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
option(MOVE_VECTORMATH_BUILD_PHASE_A_TESTS
       "Build API-v2 Phase A scalar and RTM architecture proof tests" OFF)
option(MOVE_VECTORMATH_BUILD_PHASE_B_TESTS
       "Build API-v2 Phase B semantic transform tests" OFF)

if(MOVE_VECTORMATH_BUILD_TESTS OR
   MOVE_VECTORMATH_BUILD_STANDALONE_TESTS OR
   MOVE_VECTORMATH_BUILD_PHASE_A_TESTS OR
   MOVE_VECTORMATH_BUILD_PHASE_B_TESTS)
    enable_testing()
endif()

if(MOVE_VECTORMATH_BUILD_PHASE_A_TESTS)
    add_executable(move-vectormath-phase-a-rtm-tests tests/phase_a.cpp)
    target_link_libraries(move-vectormath-phase-a-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-a-rtm-tests)
    add_test(NAME move-vectormath-phase-a-rtm
             COMMAND move-vectormath-phase-a-rtm-tests)

    add_executable(move-vectormath-phase-a-scalar-tests tests/phase_a.cpp)
    target_link_libraries(move-vectormath-phase-a-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-phase-a-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-a-scalar-tests)
    add_test(NAME move-vectormath-phase-a-scalar
             COMMAND move-vectormath-phase-a-scalar-tests)

    add_library(move-vectormath-phase-a-rtm-codegen OBJECT
                tests/phase_a_codegen.cpp)
    target_link_libraries(move-vectormath-phase-a-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-a-rtm-codegen)

    add_library(move-vectormath-phase-a-scalar-codegen OBJECT
                tests/phase_a_codegen.cpp)
    target_link_libraries(move-vectormath-phase-a-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-phase-a-scalar-codegen
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-a-scalar-codegen)
endif()

if(MOVE_VECTORMATH_BUILD_PHASE_B_TESTS)
    add_executable(move-vectormath-phase-b-rtm-tests tests/phase_b.cpp)
    target_link_libraries(move-vectormath-phase-b-rtm-tests
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-b-rtm-tests)
    add_test(NAME move-vectormath-phase-b-rtm
             COMMAND move-vectormath-phase-b-rtm-tests)

    add_executable(move-vectormath-phase-b-scalar-tests tests/phase_b.cpp)
    target_link_libraries(move-vectormath-phase-b-scalar-tests
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-phase-b-scalar-tests
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-b-scalar-tests)
    add_test(NAME move-vectormath-phase-b-scalar
             COMMAND move-vectormath-phase-b-scalar-tests)

    add_library(move-vectormath-phase-b-rtm-codegen OBJECT
                tests/phase_b_codegen.cpp)
    target_link_libraries(move-vectormath-phase-b-rtm-codegen
                          PRIVATE move-vectormath)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-b-rtm-codegen)

    add_library(move-vectormath-phase-b-scalar-codegen OBJECT
                tests/phase_b_codegen.cpp)
    target_link_libraries(move-vectormath-phase-b-scalar-codegen
                          PRIVATE move-vectormath)
    target_compile_definitions(move-vectormath-phase-b-scalar-codegen
                               PRIVATE MV_MATH_FORCE_SCALAR=1)
    move_vectormath_enable_strict_warnings(
        move-vectormath-phase-b-scalar-codegen)
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
