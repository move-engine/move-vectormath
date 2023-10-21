#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec4.hpp>

SCENARIO("Test vec4", "[move::vectormath::vec4]")
{
    using namespace move::vectormath;
    REQUIRE(vec4f::zero() == vec4f(0, 0, 0, 0));
    REQUIRE(vec4f::one() == vec4f(1, 1, 1, 1));
    REQUIRE(vec4f(1, 1, 1, 1) == vec4f(1, 1, 1, 1));
    REQUIRE(vec4f(0, 0, 0, 0) == vec4f(0, 0, 0, 0));

    REQUIRE(vec4f::right() == vec4f(1, 0, 0, 0));
    REQUIRE(vec4f::up() == vec4f(0, 1, 0, 0));
    REQUIRE(vec4f::forward() == vec4f(0, 0, 1, 0));

    REQUIRE(vec4f::x_axis() == vec4f(1, 0, 0, 0));
    REQUIRE(vec4f::y_axis() == vec4f(0, 1, 0, 0));
    REQUIRE(vec4f::z_axis() == vec4f(0, 0, 1, 0));
    REQUIRE(vec4f::w_axis() == vec4f(0, 0, 0, 1));

    GIVEN("A vector")
    {
        vec4f v(1, 1, 1, 1);
        WHEN("The vector is normalized")
        {
            auto nrm = v.normalized();
            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(4.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
                REQUIRE(nrm.w() == Approx(val));
            }
        }

        WHEN("The vector is approximately normalized")
        {
            auto nrm = v.normalized_approximate();
            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(4.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
                REQUIRE(nrm.w() == Approx(val));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        auto vec = vec4f(1, 2, 3, 4);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3, 4}));

        vec4f res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = vec4f(1, 2, 3, 4);
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        vec4f res;
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(res);
        }
        REQUIRE(res == vec);
    }
}
