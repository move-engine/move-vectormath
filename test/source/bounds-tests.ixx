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

    using bounds_type = MVM_BOUNDS_TEST_BOUNDS_TYPE;
    using vec_type = bounds_type::vec_type;
    using component_type = typename vec_type::component_type;

    WHEN("A default bounding box is created")
    {
        bounds_type bounds;

        THEN("All values should be as expected")
        {
            REQUIRE(bounds.min() == vec_type::zero());
            REQUIRE(bounds.max() == vec_type::zero());
            REQUIRE(bounds.center() == vec_type::zero());
            REQUIRE(bounds.size() == vec_type::zero());
            REQUIRE(bounds.extents() == vec_type::zero());
        }

        THEN("The closest point should be zero")
        {
            vec_type point = vec_type::filled(10);
            REQUIRE(bounds.closest_point(point) == vec_type::zero());
        }

        AND_WHEN("A point is encapsulated")
        {
            vec_type point = vec_type::filled(10);
            bounds.encapsulate(point);

            THEN("All values should be as expected")
            {
                REQUIRE(bounds.min() == vec_type::zero());
                REQUIRE(bounds.max() == vec_type::filled(10));
                REQUIRE(bounds.center() == vec_type::filled(5));
                REQUIRE(bounds.size() == vec_type::filled(10));
                REQUIRE(bounds.extents() == vec_type::filled(5));
            }

            if constexpr (std::is_unsigned_v<component_type>)
            {
                THEN("The closest point to (0, 0) should be (0, 0)")
                {
                    vec_type point = vec_type(0);
                    REQUIRE(bounds.closest_point(point) == vec_type::zero());
                }
            }
            else
            {
                THEN("The closest point to (-100, 0, 0) should be (0, 0, 0)")
                {
                    vec_type point = vec_type(-100);
                    REQUIRE(bounds.closest_point(point) == vec_type::zero());
                }
            }

            THEN("The closest point to (100, 0, 0) should be (10, 0, 0)")
            {
                vec_type point = vec_type(100);
                REQUIRE(bounds.closest_point(point) == vec_type(10));
            }
        }
    }

    WHEN("A bounding box centered on a point is created")
    {
        bounds_type bounds(vec_type::filled(10));

        THEN("All values should be as expected")
        {
            REQUIRE(bounds.min() == vec_type::filled(10));
            REQUIRE(bounds.max() == vec_type::filled(10));
            REQUIRE(bounds.center() == vec_type::filled(10));
            REQUIRE(bounds.size() == vec_type::zero());
            REQUIRE(bounds.extents() == vec_type::zero());
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

    GIVEN(
        "A bounding box from (10, 10, 10) to (20, 20, 20), another from (15, "
        "15, 15) to (25, 25, 25), and another from (30, 30, 30) to (35, 35, "
        "35)")
    {
        bounds_type bounds1(vec_type::filled(10), vec_type::filled(20));
        bounds_type bounds2(vec_type::filled(15), vec_type::filled(25));
        bounds_type bounds3(vec_type::filled(30), vec_type::filled(35));

        THEN("bounds1 should intersect bounds2, but not bounds3")
        {
            REQUIRE(bounds1.intersects(bounds2));
            REQUIRE(!bounds1.intersects(bounds3));

            REQUIRE(bounds2.intersects(bounds1));
            REQUIRE(!bounds2.intersects(bounds3));

            REQUIRE(!bounds3.intersects(bounds1));
            REQUIRE(!bounds3.intersects(bounds2));
        }

        AND_WHEN("bounds3 encapsulates bounds1")
        {
            bounds3.encapsulate(bounds1);

            THEN("Bounds3 should now be (10, 10, 10) to (35, 35, 35)")
            {
                REQUIRE(bounds3.min() == vec_type::filled(10));
                REQUIRE(bounds3.max() == vec_type::filled(35));
            }

            THEN("bounds3 should now intersect both")
            {
                REQUIRE(bounds1.intersects(bounds3));
                REQUIRE(bounds3.intersects(bounds1));

                REQUIRE(bounds2.intersects(bounds3));
                REQUIRE(bounds3.intersects(bounds2));
            }
        }

        AND_WHEN("bounds3 is expanded by 6")
        {
            bounds3.expand(vec_type::filled(6));

            THEN("Bounds3 should now be (24, 24, 24) to (41, 41, 41)")
            {
                REQUIRE(bounds3.min() == vec_type::filled(24));
                REQUIRE(bounds3.max() == vec_type::filled(41));
            }

            THEN("bounds2 should intersect bounds3, but bounds1 should not")
            {
                REQUIRE(bounds2.intersects(bounds3));
                REQUIRE(bounds3.intersects(bounds2));

                REQUIRE(!bounds1.intersects(bounds3));
                REQUIRE(!bounds3.intersects(bounds1));
            }
        }
    }
}

#undef MVM_BOUNDS_TEST_BOUNDS_TYPE
#undef __mvm_xstr
#undef __mvm_str
#undef MVM_TEST_NAME
#undef MVM_TEST_SCOPE