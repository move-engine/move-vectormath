#include <math.h>

#include <catch2/catch_all.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"

#include <move/vectormath.hpp>
#include "move/vectormath/common.hpp"

SCENARIO("Test fastvec2", "[move::vectormath::fastvec2]")
{
    using namespace move::vectormath;
    REQUIRE(fastvec2::zero() == fastvec2(0, 0));
    REQUIRE(fastvec2::one() == fastvec2(1, 1));
    REQUIRE(fastvec2(1, 1) == fastvec2(1, 1));
    REQUIRE(fastvec2(0, 0) == fastvec2(0, 0));

    REQUIRE(fastvec2::right() == fastvec2(1, 0));
    REQUIRE(fastvec2::up() == fastvec2(0, 1));

    REQUIRE(fastvec2::x_axis() == fastvec2(1, 0));
    REQUIRE(fastvec2::y_axis() == fastvec2(0, 1));

    GIVEN("A vector")
    {
        fastvec2 v(1, 1);
        WHEN("The vector is normalized")
        {
            auto nrm = v.normalized();
            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(2.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
            }
        }
    }

    // TODO: Write more tests
}