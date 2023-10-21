#include <math.h>
#include <catch2/catch_all.hpp>
#include <move/vectormath.hpp>

#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "move/vectormath/common.hpp"

SCENARIO("Test fastvec3", "[move::vectormath::fastvec3]")
{
    using namespace move::vectormath;
    REQUIRE(fastvec3::zero() == fastvec3(0, 0, 0));
    REQUIRE(fastvec3::one() == fastvec3(1, 1, 1));
    REQUIRE(fastvec3(1, 1, 1) == fastvec3(1, 1, 1));
    REQUIRE(fastvec3(0, 0, 0) == fastvec3(0, 0, 0));

    REQUIRE(fastvec3::right() == fastvec3(1, 0, 0));
    REQUIRE(fastvec3::up() == fastvec3(0, 1, 0));
    REQUIRE(fastvec3::forward() == fastvec3(0, 0, 1));

    REQUIRE(fastvec3::x_axis() == fastvec3(1, 0, 0));
    REQUIRE(fastvec3::y_axis() == fastvec3(0, 1, 0));
    REQUIRE(fastvec3::z_axis() == fastvec3(0, 0, 1));

    GIVEN("A vector")
    {
        fastvec3 v(1, 1, 1);
        WHEN("The vector is normalized")
        {
            auto nrm = v.normalized();

            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(3.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
            }
        }
    }

    // TODO: Write more tests
}