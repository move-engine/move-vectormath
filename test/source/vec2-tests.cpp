#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

#include <move/vectormath.hpp>

SCENARIO("Test vec2", "[move::vectormath::vec2]")
{
    using namespace move::vectormath;
    REQUIRE(vec2::zero() == vec2(0, 0));
    REQUIRE(vec2::one() == vec2(1, 1));
    REQUIRE(vec2(1, 1) == vec2(1, 1));
    REQUIRE(vec2(0, 0) == vec2(0, 0));

    REQUIRE(vec2::right() == vec2(1, 0));
    REQUIRE(vec2::up() == vec2(0, 1));

    REQUIRE(vec2::x_axis() == vec2(1, 0));
    REQUIRE(vec2::y_axis() == vec2(0, 1));

    GIVEN("A vector")
    {
        vec2 v(1, 1);
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

    THEN("JSON serialization should work")
    {
        auto vec = vec2(1, 2);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2}));

        vec2 res;
        j.get_to(res);
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = vec2(1, 2);
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        vec2 res;
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(res);
        }
        REQUIRE(res == vec);
    }
}

SCENARIO("Test ivec2", "[move::vectormath::ivec2]")
{
    using namespace move::vectormath;
    REQUIRE(ivec2::zero() == ivec2(0, 0));
    REQUIRE(ivec2::one() == ivec2(1, 1));
    REQUIRE(ivec2(1, 1) == ivec2(1, 1));
    REQUIRE(ivec2(0, 0) == ivec2(0, 0));

    REQUIRE(ivec2::right() == ivec2(1, 0));
    REQUIRE(ivec2::up() == ivec2(0, 1));

    REQUIRE(ivec2::x_axis() == ivec2(1, 0));
    REQUIRE(ivec2::y_axis() == ivec2(0, 1));

    THEN("JSON serialization should work")
    {
        auto vec = ivec2(1, 2);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2}));
        ivec2 res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = ivec2(1, 2);
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        ivec2 res;
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(res);
        }
        REQUIRE(res == vec);
    }
}