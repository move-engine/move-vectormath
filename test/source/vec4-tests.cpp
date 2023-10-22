#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec4.hpp>

#include "mvm_test_utils.hpp"

SCENARIO("Test vec4f", "[move::vectormath::vec4f]")
{
    using namespace move::vectormath;
    using vec_type = vec4f;

    REQUIRE(vec_type::zero() == vec_type(0, 0, 0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(1, 1, 1, 1) == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(0, 0, 0, 0) == vec_type(0, 0, 0, 0));

    REQUIRE(vec_type::right() == vec_type(1, 0, 0, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1, 0, 0));
    REQUIRE(vec_type::forward() == vec_type(0, 0, 1, 0));

    REQUIRE(vec_type::x_axis() == vec_type(1, 0, 0, 0));
    REQUIRE(vec_type::y_axis() == vec_type(0, 1, 0, 0));
    REQUIRE(vec_type::z_axis() == vec_type(0, 0, 1, 0));
    REQUIRE(vec_type::w_axis() == vec_type(0, 0, 0, 1));

    GIVEN("A vector")
    {
        using Catch::Approx;

        vec_type v1(1, 1, 1, 1);
        DirectX::XMVECTOR dxm_v1 = vec4_to_dxm(v1);

        THEN("Vector length should work")
        {
            auto len = v1.length();
            REQUIRE(len == Approx(2.0));
        }

        THEN("Approximate vector length should work")
        {
            auto len = v1.length_approximate();
            REQUIRE(len == Approx(2.0));
        }

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(4.0));
        }

        WHEN("The vector is normalized")
        {
            auto nrm = v1.normalized();
            THEN("The value should be correct")
            {
                auto val = 1.0 / sqrt(4.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
                REQUIRE(nrm.w() == Approx(val));
            }
        }

        WHEN("The vector is approximately normalized")
        {
            auto nrm = v1.normalized_approximate();
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

        WHEN("The vector is normalized")
        {
            auto normalized = v1.normalized();
            v1.normalize();

            THEN("It should be identical to .normalized()")
            {
                REQUIRE(v1.x() == Approx(normalized.x()));
                REQUIRE(v1.y() == Approx(normalized.y()));
                REQUIRE(v1.z() == Approx(normalized.z()));
                REQUIRE(v1.w() == Approx(normalized.w()));
            }
        }

        AND_GIVEN("Another normalized vec4")
        {
            vec_type v2(1, 2, -5, 2);
            v2.normalize();

            DirectX::XMVECTOR dxm_v2 = vec4_to_dxm(v2);

            THEN("The dot product should be correct")
            {
                auto dot = v1.dot(v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector4Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = v1.distance_to_point(v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector4Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN(
                "The approximate distance between the points should be correct")
            {
                auto dist = v1.distance_to_point_approximate(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector4LengthEst(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = v1.squared_distance_to_point(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("Vector4 cross should work")
            {
                auto v3 = vec_type(1, -2, 3, -4);
                auto dxm_v3 = vec4_to_dxm(v3);

                auto cross = v1.cross(v2, v3);
                auto dxm_cross = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Cross(dxm_v1, dxm_v2, dxm_v3));

                REQUIRE(cross.x() == Approx(dxm_cross.x()));
                REQUIRE(cross.y() == Approx(dxm_cross.y()));
                REQUIRE(cross.z() == Approx(dxm_cross.z()));
                REQUIRE(cross.w() == Approx(dxm_cross.w()));
            }

            THEN("Angle between normalized vectors should work")
            {
                auto v1norm = v1.normalized();
                auto v2norm = v2.normalized();
                auto dxm_v1norm = vec4_to_dxm(v1norm);
                auto dxm_v2norm = vec4_to_dxm(v2norm);
                auto angle = v1norm.angle_between_normalized_vectors(v2norm);
                auto dxm_angle =
                    DirectX::XMVectorGetX(DirectX::XMVector4AngleBetweenNormals(
                        dxm_v1norm, dxm_v2norm));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Angle between non-normalized vectors should work")
            {
                auto angle = v1.angle_between_vectors(v2);
                auto dxm_angle = DirectX::XMVectorGetX(
                    DirectX::XMVector4AngleBetweenVectors(dxm_v1, dxm_v2));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Reflection should be correct")
            {
                auto ref = v1.reflect(v2);
                auto dxm_ref = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Reflect(dxm_v1, dxm_v2));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
                REQUIRE(ref.w() == Approx(dxm_ref.w()));
            }

            THEN("Refraction should be correct")
            {
                auto ref = v1.refract(v2, 0.5f);
                auto dxm_ref = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Refract(dxm_v1, dxm_v2, 0.5f));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
                REQUIRE(ref.w() == Approx(dxm_ref.w()));
            }

            THEN("Min should be correct")
            {
                auto min = v1.min(v2);
                auto dxm_min = vec4_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
                REQUIRE(min.z() == Approx(dxm_min.z()));
                REQUIRE(min.w() == Approx(dxm_min.w()));
            }

            THEN("Max should be correct")
            {
                auto max = v1.max(v2);
                auto dxm_max = vec4_from_dxm<vec_type>(
                    DirectX::XMVectorMax(dxm_v1, dxm_v2));

                REQUIRE(max.x() == Approx(dxm_max.x()));
                REQUIRE(max.y() == Approx(dxm_max.y()));
                REQUIRE(max.z() == Approx(dxm_max.z()));
                REQUIRE(max.w() == Approx(dxm_max.w()));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        auto vec = vec_type(1, 2, 3, 4);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3, 4}));

        vec_type res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = vec_type(1, 2, 3, 4);
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

SCENARIO("Test vec4d", "[move::vectormath::vec4d]")
{
    using namespace move::vectormath;
    using vec_type = vec4d;

    REQUIRE(vec_type::zero() == vec_type(0, 0, 0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(1, 1, 1, 1) == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(0, 0, 0, 0) == vec_type(0, 0, 0, 0));

    REQUIRE(vec_type::right() == vec_type(1, 0, 0, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1, 0, 0));
    REQUIRE(vec_type::forward() == vec_type(0, 0, 1, 0));

    REQUIRE(vec_type::x_axis() == vec_type(1, 0, 0, 0));
    REQUIRE(vec_type::y_axis() == vec_type(0, 1, 0, 0));
    REQUIRE(vec_type::z_axis() == vec_type(0, 0, 1, 0));
    REQUIRE(vec_type::w_axis() == vec_type(0, 0, 0, 1));

    GIVEN("A vector")
    {
        using Catch::Approx;

        vec_type v1(1, 1, 1, 1);
        DirectX::XMVECTOR dxm_v1 = vec4_to_dxm(v1);

        THEN("Vector length should work")
        {
            auto len = v1.length();
            REQUIRE(len == Approx(2.0));
        }

        THEN("Approximate vector length should work")
        {
            auto len = v1.length_approximate();
            REQUIRE(len == Approx(2.0));
        }

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(4.0));
        }

        WHEN("The vector is normalized")
        {
            auto nrm = v1.normalized();
            THEN("The value should be correct")
            {
                auto val = 1.0 / sqrt(4.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
                REQUIRE(nrm.w() == Approx(val));
            }
        }

        WHEN("The vector is approximately normalized")
        {
            auto nrm = v1.normalized_approximate();
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

        WHEN("The vector is normalized")
        {
            auto normalized = v1.normalized();
            v1.normalize();

            THEN("It should be identical to .normalized()")
            {
                REQUIRE(v1.x() == Approx(normalized.x()));
                REQUIRE(v1.y() == Approx(normalized.y()));
                REQUIRE(v1.z() == Approx(normalized.z()));
                REQUIRE(v1.w() == Approx(normalized.w()));
            }
        }

        AND_GIVEN("Another normalized vec4")
        {
            vec_type v2(1, 2, -5, 2);
            v2.normalize();

            DirectX::XMVECTOR dxm_v2 = vec4_to_dxm(v2);

            THEN("The dot product should be correct")
            {
                auto dot = v1.dot(v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector4Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = v1.distance_to_point(v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector4Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN(
                "The approximate distance between the points should be correct")
            {
                auto dist = v1.distance_to_point_approximate(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector4LengthEst(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = v1.squared_distance_to_point(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("Vector4 cross should work")
            {
                auto v3 = vec_type(1, -2, 3, -4);
                auto dxm_v3 = vec4_to_dxm(v3);

                auto cross = v1.cross(v2, v3);
                auto dxm_cross = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Cross(dxm_v1, dxm_v2, dxm_v3));

                REQUIRE(cross.x() == Approx(dxm_cross.x()));
                REQUIRE(cross.y() == Approx(dxm_cross.y()));
                REQUIRE(cross.z() == Approx(dxm_cross.z()));
                REQUIRE(cross.w() == Approx(dxm_cross.w()));
            }

            THEN("Angle between normalized vectors should work")
            {
                auto v1norm = v1.normalized();
                auto v2norm = v2.normalized();
                auto dxm_v1norm = vec4_to_dxm(v1norm);
                auto dxm_v2norm = vec4_to_dxm(v2norm);
                auto angle = v1norm.angle_between_normalized_vectors(v2norm);
                auto dxm_angle =
                    DirectX::XMVectorGetX(DirectX::XMVector4AngleBetweenNormals(
                        dxm_v1norm, dxm_v2norm));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Angle between non-normalized vectors should work")
            {
                auto angle = v1.angle_between_vectors(v2);
                auto dxm_angle = DirectX::XMVectorGetX(
                    DirectX::XMVector4AngleBetweenVectors(dxm_v1, dxm_v2));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Reflection should be correct")
            {
                auto ref = v1.reflect(v2);
                auto dxm_ref = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Reflect(dxm_v1, dxm_v2));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
                REQUIRE(ref.w() == Approx(dxm_ref.w()));
            }

            THEN("Refraction should be correct")
            {
                auto ref = v1.refract(v2, 0.5f);
                auto dxm_ref = vec4_from_dxm<vec_type>(
                    DirectX::XMVector4Refract(dxm_v1, dxm_v2, 0.5f));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
                REQUIRE(ref.w() == Approx(dxm_ref.w()));
            }

            THEN("Min should be correct")
            {
                auto min = v1.min(v2);
                auto dxm_min = vec4_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
                REQUIRE(min.z() == Approx(dxm_min.z()));
                REQUIRE(min.w() == Approx(dxm_min.w()));
            }

            THEN("Max should be correct")
            {
                auto max = v1.max(v2);
                auto dxm_max = vec4_from_dxm<vec_type>(
                    DirectX::XMVectorMax(dxm_v1, dxm_v2));

                REQUIRE(max.x() == Approx(dxm_max.x()));
                REQUIRE(max.y() == Approx(dxm_max.y()));
                REQUIRE(max.z() == Approx(dxm_max.z()));
                REQUIRE(max.w() == Approx(dxm_max.w()));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        auto vec = vec_type(1, 2, 3, 4);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3, 4}));

        vec_type res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        auto vec = vec_type(1, 2, 3, 4);
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
