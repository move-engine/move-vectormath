// No pragma once because we're including the same file multiple times, using
// MVM_BOUNDS_TEST_BOUNDS_TYPE to make it "generic" without messing with
// Catch's ability to properly report the failures.

#include <type_traits>
#ifndef MVM_BOUNDS_TEST_BOUNDS_TYPE
#ifdef MVM_IS_BOUNDS_TEST_CPP
#error MVM_BOUNDS_TEST_BOUNDS_TYPE not defined
#else
// Includes to make maintaining this file easier
#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include <move/vectormath/bounds.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

#include "mvm_test_utils.hpp"
#define MVM_BOUNDS_TEST_BOUNDS_TYPE bounds2f
#endif
#endif

#define __mvm_xstr(a) __mvm_str(a)
#define __mvm_str(a) #a

#define MVM_TEST_NAME(expr) "Test " __mvm_xstr(expr)
#define MVM_TEST_SCOPE(expr) "[move::vectormath::" __mvm_xstr(expr) "]"

SCENARIO(MVM_TEST_NAME(MVM_BOUNDS_TEST_BOUNDS_TYPE),
    MVM_TEST_SCOPE(MVM_BOUNDS_TEST_BOUNDS_TYPE))
{
    using Catch::Approx;
    using namespace move::vectormath;
    using namespace DirectX;

    using bounds_type = MVM_BOUNDS_TEST_BOUNDS_TYPE;
    using vec_type = bounds_type::vec_type;

    WHEN("A default bounding box is created")
    {
        bounds_type bounds;

        THEN("Its min and max should be zero")
        {
            REQUIRE(bounds.min() == vec_type::zero());
            REQUIRE(bounds.max() == vec_type::zero());
        }

        AND_WHEN("A point is encapsulated")
        {
            vec_type point = vec_type::filled(10);
            bounds.encapsulate(point);

            THEN("Its min and max should be as expected")
            {
                REQUIRE(bounds.min() == vec_type::zero());
                REQUIRE(bounds.max() == vec_type::filled(10));
            }
        }
    }

    WHEN("A bounding box centered on a point is created")
    {
        bounds_type bounds(vec_type::filled(10));

        THEN("Its min and max should be as expected")
        {
            REQUIRE(bounds.min() == vec_type::filled(10));
            REQUIRE(bounds.max() == vec_type::filled(10));
        }

        AND_WHEN("A point is encapsulated")
        {
            vec_type point = vec_type::filled(20);
            bounds.encapsulate(point);

            THEN("Its min and max should be as expected")
            {
                REQUIRE(bounds.min() == vec_type::filled(10));
                REQUIRE(bounds.max() == vec_type::filled(20));
            }

            AND_WHEN("Zero is encapsulated")
            {
                bounds.encapsulate(vec_type::zero());

                THEN("Its min and max should be as expected")
                {
                    REQUIRE(bounds.min() == vec_type::zero());
                    REQUIRE(bounds.max() == vec_type::filled(20));
                }
            }
        }
    }

    WHEN("A bounding box with arbitrary bounds is created")
    {
        bounds_type bounds(vec_type::filled(10), vec_type::filled(30));

        THEN("Its min and max should be non-zero")
        {
            REQUIRE(bounds.min() != vec_type::zero());
            REQUIRE(bounds.max() != vec_type::zero());
        }

        THEN("Its min and max should be as expected")
        {
            REQUIRE(bounds.min() == vec_type::filled(10));
            REQUIRE(bounds.max() == vec_type::filled(30));
        }

        THEN("It should correctly contain a point within the bounds")
        {
            vec_type point = vec_type::filled(20);
            REQUIRE(bounds.contains(point));
        }

        THEN("It should correctly not contain a point outside of the bounds")
        {
            vec_type point = vec_type::filled(40);
            REQUIRE(!bounds.contains(point));
        }
    }
}

#undef MVM_BOUNDS_TEST_BOUNDS_TYPE
#undef __mvm_xstr
#undef __mvm_str
#undef MVM_TEST_NAME
#undef MVM_TEST_SCOPE