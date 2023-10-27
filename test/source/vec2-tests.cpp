#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec2.hpp>

#include "mvm_test_utils.hpp"

SCENARIO("Test vec2f", "[move::vectormath::vec2f]")
{
    using namespace move::vectormath;
    using vec_type = vec2f;

    REQUIRE(vec_type::zero() == vec_type(0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1));
    REQUIRE(vec_type(1, 1) == vec_type(1, 1));
    REQUIRE(vec_type(0, 0) == vec_type(0, 0));

    REQUIRE(vec_type(1, 1) != vec_type(0, 0));
    REQUIRE(vec_type(1, 1) != vec_type(0, 1));
    REQUIRE(vec_type(1, 1) != vec_type(1, 0));

    REQUIRE(vec_type::right() == vec_type(1, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1));

    REQUIRE(vec_type::x_axis() == vec_type(1, 0));
    REQUIRE(vec_type::y_axis() == vec_type(0, 1));

    GIVEN("A vector")
    {
        using Catch::Approx;

        vec_type v1(1, 1);
        DirectX::XMVECTOR dxm_v1 = vec2_to_dxm(v1);

        THEN("Vector length should work")
        {
            auto len = v1.length();
            REQUIRE(len == Approx(sqrt(2.0)));
        }

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(2.0));
        }

        WHEN("The vector is normalized")
        {
            REQUIRE(v1.x() == Approx(1.0));

            auto nrm = v1.normalized();
            THEN("The value should be correct")
            {
                auto val = 1.0 / sqrt(2.0);

                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
            }
        }

        WHEN("The vector is normalized")
        {
            auto normalized = v1.normalized();
            v1.normalize();

            THEN("It should be identical to .normalized()")
            {
                REQUIRE(v1.x() == Approx(normalized.x()));
                REQUIRE(v1.y() == Approx(normalized.y()));
            }
        }

        AND_GIVEN("Another normalized vec2")
        {
            using value_type = typename vec_type::component_type;
            vec_type v2(1, value_type(0.4));
            v2.normalize();

            DirectX::XMVECTOR dxm_v2 = vec2_to_dxm(v2);

            THEN("The dot product should be correct")
            {
                auto dot = vec_type::dot(v1, v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector2Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = vec_type::distance_between_points(v1, v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector2Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = vec_type::squared_distance_between_points(v1, v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector2LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The distance to a line should be correct")
            {
                auto dist = vec_type::distance_to_line(v1, vec_type(0, 0), v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector2LinePointDistance(
                        DirectX::XMVectorSet(0, 0, 0, 0), dxm_v2, dxm_v1));

                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("Vector3 cross should work")
            {
                auto cross = vec_type::cross(v1, v2);
                auto dxm_cross = DirectX::XMVectorGetX(DirectX::XMVector2Cross(
                    DirectX::XMVectorSet(v1.x(), v1.y(), 0, 0),
                    DirectX::XMVectorSet(v2.x(), v2.y(), 0, 0)));

                REQUIRE(cross == Approx(dxm_cross));
            }

            THEN("Angle between normalized vectors should work")
            {
                auto v1norm = v1.normalized();
                auto v2norm = v2.normalized();
                auto dxm_v1norm = vec2_to_dxm(v1norm);
                auto dxm_v2norm = vec2_to_dxm(v2norm);
                auto angle =
                    vec_type::angle_between_normalized_vectors(v1norm, v2norm);
                auto dxm_angle =
                    DirectX::XMVectorGetX(DirectX::XMVector2AngleBetweenNormals(
                        dxm_v1norm, dxm_v2norm));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Angle between non-normalized vectors should work")
            {
                auto angle = vec_type::angle_between_vectors(v1, v2);
                auto dxm_angle = DirectX::XMVectorGetX(
                    DirectX::XMVector2AngleBetweenVectors(dxm_v1, dxm_v2));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Reflection should be correct")
            {
                auto ref = vec_type::reflect(v1, v2);
                auto dxm_ref = vec2_from_dxm<vec_type>(
                    DirectX::XMVector2Reflect(dxm_v1, dxm_v2));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
            }

            THEN("Refraction should be correct")
            {
                auto ref = vec_type::refract(v1, v2, 0.5f);
                auto dxm_ref = vec2_from_dxm<vec_type>(
                    DirectX::XMVector2Refract(dxm_v1, dxm_v2, 0.5f));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
            }

            THEN("Min should be correct")
            {
                auto min = vec_type::min(v1, v2);
                auto dxm_min = vec2_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
            }

            THEN("Max should be correct")
            {
                auto max = vec_type::max(v1, v2);
                auto dxm_max = vec2_from_dxm<vec_type>(
                    DirectX::XMVectorMax(dxm_v1, dxm_v2));

                REQUIRE(max.x() == Approx(dxm_max.x()));
                REQUIRE(max.y() == Approx(dxm_max.y()));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        using vec_type = vec2f;
        auto vec = vec_type(1, 2);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2}));

        vec_type res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        using vec_type = vec2f;
        auto vec = vec_type(1, 2);
        std::stringstream ss;
        {
            cereal::BinaryOutputArchive oarchive(ss);
            oarchive(vec);
        }
        vec_type res;
        {
            cereal::BinaryInputArchive iarchive(ss);
            iarchive(res);
        }
        REQUIRE(res == vec);
    }
}
