#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

#include <move/vectormath/vec3.hpp>

SCENARIO("Test vec3", "[move::vectormath::vec3]")
{
    using namespace move::vectormath;
    REQUIRE(vec3::zero() == vec3(0, 0, 0));
    REQUIRE(vec3::one() == vec3(1, 1, 1));
    REQUIRE(vec3(1, 1, 1) == vec3(1, 1, 1));
    REQUIRE(vec3(0, 0, 0) == vec3(0, 0, 0));

    REQUIRE(vec3::right() == vec3(1, 0, 0));
    REQUIRE(vec3::up() == vec3(0, 1, 0));
    REQUIRE(vec3::forward() == vec3(0, 0, 1));

    REQUIRE(vec3::x_axis() == vec3(1, 0, 0));
    REQUIRE(vec3::y_axis() == vec3(0, 1, 0));
    REQUIRE(vec3::z_axis() == vec3(0, 0, 1));

    GIVEN("A vector")
    {
        vec3 v(1, 1, 1);
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

    THEN("JSON serialization should work")
    {
        auto vec = vec3(1, 2, 3);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3}));
        vec3 res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = vec3(1, 2, 3);
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        vec3 res;
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(res);
        }
        REQUIRE(res == vec);
    }
}
