#include <catch2/catch_all.hpp>
#include <move/vectormath.hpp>

#include "catch2/catch_test_macros.hpp"
#include "move/vectormath/common.hpp"

SCENARIO("Test fastmat4", "[move::vectormath::fastmat4]")
{
    GIVEN("A transformation matrix")
    {
        using namespace move::vectormath;
        fastmat4 transformation = fastmat4::create_transformation_matrix(
            fastvec3(30, 30, 30), fastquat::identity(), fastvec3::one());

        WHEN("The transformation is applied to a vector")
        {
            fastvec3 v = fastvec3::zero();
            v = transformation * v;

            THEN("The result is correct")
            {
                REQUIRE(v.x() == (30));
                REQUIRE(v.y() == (30));
                REQUIRE(v.z() == (30));
            }
        }
    }

    // TODO: Write more tests
}