// No pragma once because we're including the same file multiple times, using
// MVM_TEST_TYPE to make it "generic" without messing with Catch's ability to
// properly report the failures.

#include <type_traits>
#include "move/vectormath/octahedral.hpp"
#if !defined(MVM_TEST_VEC2_TYPE) || !defined(MVM_TEST_VEC3_TYPE)
#ifdef MVM_IS_TEST_INCLUDE
#error MVM_TEST_VEC2_TYPE not defined
#error MVM_TEST_VEC3_TYPE not defined
#else
// Includes to make maintaining this file easier
#include <catch2/catch_all.hpp>
#include <move/vectormath/vec2.hpp>
#include <move/vectormath/vec3.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include "mvm_test_utils.hpp"
#define MVM_TEST_VEC2_TYPE vec2f
#define MVM_TEST_VEC3_TYPE vec3f
#endif
#endif

#define __mvm_xstr(a) __mvm_str(a)
#define __mvm_str(a) #a

#define MVM_TEST_NAME(expr) "Test " __mvm_xstr(expr)
#define MVM_TEST_SCOPE(expr) "[move::vectormath::" __mvm_xstr(expr) "]"

SCENARIO(MVM_TEST_NAME(Octahedral), MVM_TEST_SCOPE(Octahedral))
{
    using Approx = Catch::Approx;
    using namespace move::vectormath;
    using vec2_type = MVM_TEST_VEC2_TYPE;
    using vec3_type = MVM_TEST_VEC3_TYPE;

    GIVEN("A normalized 3d vector")
    {
        vec3_type vec(1, 1, 1);
        vec.normalize();

        WHEN("Mapped to octahedral")
        {
            vec2_type oct = move::vectormath::octahedral_mapping::encode(vec);

            THEN("The result is in the range [0, 1]")
            {
                REQUIRE(oct.x() >= 0);
                REQUIRE(oct.x() <= 1);
                REQUIRE(oct.y() >= 0);
                REQUIRE(oct.y() <= 1);
            }

            THEN("The result is in the octahedron")
            {
                REQUIRE(oct.x() + oct.y() <= 1);
            }

            THEN("The result can be decoded back to the original vector")
            {
                vec3_type decoded =
                    move::vectormath::octahedral_mapping::decode(oct);
                REQUIRE(decoded.x() == Approx(vec.x()));
                REQUIRE(decoded.y() == Approx(vec.y()));
                REQUIRE(decoded.z() == Approx(vec.z()));
            }
        }
    }
}

#undef MVM_TEST_TYPE
#undef __mvm_xstr
#undef __mvm_str
#undef MVM_TEST_NAME
#undef MVM_TEST_SCOPE