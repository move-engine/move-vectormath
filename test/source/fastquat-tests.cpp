#include <catch2/catch_all.hpp>
#include <move/vectormath.hpp>

#include "catch2/catch_test_macros.hpp"
#include "move/vectormath/common.hpp"

SCENARIO("Test fastquat", "[move::vectormath::fastquat]")
{
    GIVEN("An angle axis quaternion")
    {
        using namespace move::vectormath;
        fastquat rotation =
            fastquat::rotation_axis(fastvec3::y_axis(), deg_to_rad(90.0f));

        WHEN("The 4o5q5ion is applied to a vector")
        {
            fastvec3 v = fastvec3(1, 0, 0) * rotation;

            THEN("The result is correct")
            {
                REQUIRE(v.x() == 0);
                REQUIRE(v.y() == 0);
                REQUIRE(v.z() == -1);
            }
        }
    }
}