#include <math.h>
#include <cmath>
#include <stdexcept>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "DirectXMath.h"
#include "catch2/catch_approx.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_source_line_info.hpp"
#include "move/vectormath/common.hpp"
#include "nlohmann/json_fwd.hpp"

#include <move/vectormath/point.hpp>

#include "mvm_test_utils.hpp"

SCENARIO("Test pointf", "[move::vectormath::pointf]")
{
    using Catch::Approx;
    using namespace move::vectormath;
    using point_type = pointf;

    WHEN("A point is created")
    {
        point_type pt = {0, 1, 2};

        THEN("It should be equal to an equivalent point")
        {
            point_type pt2 = {0, 1, 2};
            REQUIRE(pt == pt2);
        }

        THEN("It should not be equal to an inequivalent point")
        {
            point_type pt2 = {0, 1, 3};
            REQUIRE(pt != pt2);
        }
    }
}