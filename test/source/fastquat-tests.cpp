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
            fastquat::from_rotation_axis(fastvec3::up(), deg_to_rad(90));

        WHEN("The rotation is applied to a vector")
        {
            fastvec3 v = fastvec3(1, 0, 0) * rotation;
            THEN("The result is correct")
            {
                REQUIRE(v.x() == Catch::Approx(0).margin(0.0000001f));
                REQUIRE(v.y() == Catch::Approx(0).margin(0.0000001f));
                REQUIRE(v.z() == Catch::Approx(-1).margin(0.0000001f));
            }
        }
    }
}