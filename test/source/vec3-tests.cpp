#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/vec3.hpp>

#include <DirectXMath.h>

template <typename vec_type>
inline vec_type vec3_from_dxm(const DirectX::XMVECTOR& vec)
{
    return vec_type(DirectX::XMVectorGetX(vec), DirectX::XMVectorGetY(vec),
        DirectX::XMVectorGetZ(vec));
}

template <typename vec_type>
inline DirectX::XMVECTOR vec3_to_dxm(const vec_type& vec)
{
    return DirectX::XMVectorSet(vec.x(), vec.y(), vec.z(), 0.0f);
}

SCENARIO("Test vec3f", "[move::vectormath::vec3f]")
{
    using namespace move::vectormath;
    using vec_type = vec3f;

    REQUIRE(vec_type::zero() == vec_type(0, 0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1, 1));
    REQUIRE(vec_type(1, 1, 1) == vec_type(1, 1, 1));
    REQUIRE(vec_type(0, 0, 0) == vec_type(0, 0, 0));

    REQUIRE(vec_type(1, 1, 1) != vec_type(0, 1, 1));
    REQUIRE(vec_type(1, 1, 1) != vec_type(1, 0, 1));
    REQUIRE(vec_type(1, 1, 1) != vec_type(1, 1, 0));

    REQUIRE(vec_type::right() == vec_type(1, 0, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1, 0));
    REQUIRE(vec_type::forward() == vec_type(0, 0, 1));

    REQUIRE(vec_type::x_axis() == vec_type(1, 0, 0));
    REQUIRE(vec_type::y_axis() == vec_type(0, 1, 0));
    REQUIRE(vec_type::z_axis() == vec_type(0, 0, 1));

    GIVEN("A vector")
    {
        using Catch::Approx;

        vec_type v1(1, 1, 1);
        DirectX::XMVECTOR dxm_v1 = vec3_to_dxm(v1);

        THEN("Vector length should work")
        {
            auto len = v1.length();
            REQUIRE(len == Approx(sqrt(3.0)));
        }

        THEN("Approximate vector length should work")
        {
            auto len = v1.length_approximate();
            REQUIRE(len == Approx(sqrt(3.0)));
        }

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(3.0));
        }

        WHEN("The vector is normalized")
        {
            auto nrm = v1.normalized();
            THEN("The value should be correct")
            {
                auto val = 1.0 / sqrt(3.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
            }
        }

        WHEN("The vector is approximately normalized")
        {
            auto nrm = v1.normalized_approximate();
            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(3.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
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
            }
        }

        AND_GIVEN("Another normalized vec3")
        {
            vec_type v2(1, 2, -5);
            v2.normalize();

            DirectX::XMVECTOR dxm_v2 = vec3_to_dxm(v2);

            THEN("The dot product should be correct")
            {
                auto dot = v1.dot(v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector3Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = v1.distance_to_point(v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN(
                "The approximate distance between the points should be correct")
            {
                auto dist = v1.distance_to_point_approximate(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = v1.squared_distance_to_point(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The distance to a line should be correct")
            {
                auto dist = v1.distance_to_line(vec_type(0, 0, 0), v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector3LinePointDistance(
                        DirectX::XMVectorSet(0, 0, 0, 0), dxm_v2, dxm_v1));

                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("Vector3 cross should work")
            {
                auto cross = v1.cross(v2);
                auto dxm_cross = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Cross(dxm_v1, dxm_v2));

                REQUIRE(cross.x() == Approx(dxm_cross.x()));
                REQUIRE(cross.y() == Approx(dxm_cross.y()));
                REQUIRE(cross.z() == Approx(dxm_cross.z()));
            }

            THEN("Angle between normalized vectors should work")
            {
                auto v1norm = v1.normalized();
                auto v2norm = v2.normalized();
                auto dxm_v1norm = vec3_to_dxm(v1norm);
                auto dxm_v2norm = vec3_to_dxm(v2norm);
                auto angle = v1norm.angle_between_normalized_vectors(v2norm);
                auto dxm_angle =
                    DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(
                        dxm_v1norm, dxm_v2norm));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Angle between non-normalized vectors should work")
            {
                auto angle = v1.angle_between_vectors(v2);
                auto dxm_angle = DirectX::XMVectorGetX(
                    DirectX::XMVector3AngleBetweenVectors(dxm_v1, dxm_v2));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Reflection should be correct")
            {
                auto ref = v1.reflect(v2);
                auto dxm_ref = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Reflect(dxm_v1, dxm_v2));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
            }

            THEN("Refraction should be correct")
            {
                auto ref = v1.refract(v2, 0.5f);
                auto dxm_ref = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Refract(dxm_v1, dxm_v2, 0.5f));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
            }

            THEN("Min should be correct")
            {
                auto min = v1.min(v2);
                auto dxm_min = vec3_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
                REQUIRE(min.z() == Approx(dxm_min.z()));
            }

            THEN("Max should be correct")
            {
                auto max = v1.max(v2);
                auto dxm_max = vec3_from_dxm<vec_type>(
                    DirectX::XMVectorMax(dxm_v1, dxm_v2));

                REQUIRE(max.x() == Approx(dxm_max.x()));
                REQUIRE(max.y() == Approx(dxm_max.y()));
                REQUIRE(max.z() == Approx(dxm_max.z()));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        using vec_type = vec3f;
        auto vec = vec_type(1, 2, 3);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3}));

        vec_type res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        using vec_type = vec3f;
        auto vec = vec_type(1, 2, 3);
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

SCENARIO("Test vec3d", "[move::vectormath::vec3d]")
{
    using namespace move::vectormath;
    using vec_type = vec3d;

    REQUIRE(vec_type::zero() == vec_type(0, 0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1, 1));
    REQUIRE(vec_type(1, 1, 1) == vec_type(1, 1, 1));
    REQUIRE(vec_type(0, 0, 0) == vec_type(0, 0, 0));

    REQUIRE(vec_type(1, 1, 1) != vec_type(0, 1, 1));
    REQUIRE(vec_type(1, 1, 1) != vec_type(1, 0, 1));
    REQUIRE(vec_type(1, 1, 1) != vec_type(1, 1, 0));

    REQUIRE(vec_type::right() == vec_type(1, 0, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1, 0));
    REQUIRE(vec_type::forward() == vec_type(0, 0, 1));

    REQUIRE(vec_type::x_axis() == vec_type(1, 0, 0));
    REQUIRE(vec_type::y_axis() == vec_type(0, 1, 0));
    REQUIRE(vec_type::z_axis() == vec_type(0, 0, 1));

    GIVEN("A vector")
    {
        using Catch::Approx;

        vec_type v1(1, 1, 1);
        DirectX::XMVECTOR dxm_v1 = vec3_to_dxm(v1);

        THEN("Vector length should work")
        {
            auto len = v1.length();
            REQUIRE(len == Approx(sqrt(3.0)));
        }

        THEN("Approximate vector length should work")
        {
            auto len = v1.length_approximate();
            REQUIRE(len == Approx(sqrt(3.0)));
        }

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(3.0));
        }

        WHEN("The vector is normalized")
        {
            auto nrm = v1.normalized();
            THEN("The value should be correct")
            {
                auto val = 1.0 / sqrt(3.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
            }
        }

        WHEN("The vector is approximately normalized")
        {
            auto nrm = v1.normalized_approximate();
            THEN("The value should be correct")
            {
                using namespace Catch;
                auto val = 1.0 / sqrt(3.0);
                REQUIRE(nrm.x() == Approx(val));
                REQUIRE(nrm.y() == Approx(val));
                REQUIRE(nrm.z() == Approx(val));
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
            }
        }

        AND_GIVEN("Another normalized vec3")
        {
            vec_type v2(1, 2, -5);
            v2.normalize();

            DirectX::XMVECTOR dxm_v2 = vec3_to_dxm(v2);

            THEN("The dot product should be correct")
            {
                auto dot = v1.dot(v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector3Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = v1.distance_to_point(v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector3Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN(
                "The approximate distance between the points should be correct")
            {
                auto dist = v1.distance_to_point_approximate(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector3LengthEst(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = v1.squared_distance_to_point(v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector3LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("Vector3 cross should work")
            {
                auto cross = v1.cross(v2);
                auto dxm_cross = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Cross(dxm_v1, dxm_v2));

                REQUIRE(cross.x() == Approx(dxm_cross.x()));
                REQUIRE(cross.y() == Approx(dxm_cross.y()));
                REQUIRE(cross.z() == Approx(dxm_cross.z()));
            }

            THEN("Angle between normalized vectors should work")
            {
                auto v1norm = v1.normalized();
                auto v2norm = v2.normalized();
                auto dxm_v1norm = vec3_to_dxm(v1norm);
                auto dxm_v2norm = vec3_to_dxm(v2norm);
                auto angle = v1norm.angle_between_normalized_vectors(v2norm);
                auto dxm_angle =
                    DirectX::XMVectorGetX(DirectX::XMVector3AngleBetweenNormals(
                        dxm_v1norm, dxm_v2norm));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Angle between non-normalized vectors should work")
            {
                auto angle = v1.angle_between_vectors(v2);
                auto dxm_angle = DirectX::XMVectorGetX(
                    DirectX::XMVector3AngleBetweenVectors(dxm_v1, dxm_v2));

                REQUIRE(angle == Approx(dxm_angle));
            }

            THEN("Reflection should be correct")
            {
                auto ref = v1.reflect(v2);
                auto dxm_ref = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Reflect(dxm_v1, dxm_v2));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
            }

            THEN("Refraction should be correct")
            {
                auto ref = v1.refract(v2, 0.5f);
                auto dxm_ref = vec3_from_dxm<vec_type>(
                    DirectX::XMVector3Refract(dxm_v1, dxm_v2, 0.5f));

                REQUIRE(ref.x() == Approx(dxm_ref.x()));
                REQUIRE(ref.y() == Approx(dxm_ref.y()));
                REQUIRE(ref.z() == Approx(dxm_ref.z()));
            }

            THEN("Min should be correct")
            {
                auto min = v1.min(v2);
                auto dxm_min = vec3_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
                REQUIRE(min.z() == Approx(dxm_min.z()));
            }

            THEN("Max should be correct")
            {
                auto max = v1.max(v2);
                auto dxm_max = vec3_from_dxm<vec_type>(
                    DirectX::XMVectorMax(dxm_v1, dxm_v2));

                REQUIRE(max.x() == Approx(dxm_max.x()));
                REQUIRE(max.y() == Approx(dxm_max.y()));
                REQUIRE(max.z() == Approx(dxm_max.z()));
            }
        }
    }

    THEN("JSON serialization should work")
    {
        using vec_type = vec3f;
        auto vec = vec_type(1, 2, 3);
        nlohmann::json j = vec;
        REQUIRE(j == nlohmann::json::array({1, 2, 3}));

        vec_type res = j;
        REQUIRE(res == vec);
    }

    THEN("Cereal serialization should work")
    {
        using vec_type = vec3f;
        auto vec = vec_type(1, 2, 3);
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
