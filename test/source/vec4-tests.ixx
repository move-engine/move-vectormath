// No pragma once because we're including the same file multiple times, using
// MVM_TEST_TYPE to make it "generic" without messing with Catch's ability to
// properly report the failures.

#include <type_traits>
#ifndef MVM_TEST_TYPE
#ifdef MVM_IS_TEST_INCLUDE
#error MVM_TEST_TYPE not defined
#else
// Includes to make maintaining this file easier
#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include <move/vectormath/vec4.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_test_macros.hpp"
#include "nlohmann/json_fwd.hpp"

#include "mvm_test_utils.hpp"
#define MVM_TEST_TYPE vec4f
#endif
#endif

#define __mvm_xstr(a) __mvm_str(a)
#define __mvm_str(a) #a

#define MVM_TEST_NAME(expr) "Test " __mvm_xstr(expr)
#define MVM_TEST_SCOPE(expr) "[move::vectormath::" __mvm_xstr(expr) "]"

SCENARIO(MVM_TEST_NAME(MVM_TEST_TYPE), MVM_TEST_SCOPE(MVM_TEST_TYPE))
{
    using namespace move::vectormath;
    using vec_type = MVM_TEST_TYPE;

    REQUIRE(vec_type::zero() == vec_type(0, 0, 0, 0));
    REQUIRE(vec_type::one() == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(1, 1, 1, 1) == vec_type(1, 1, 1, 1));
    REQUIRE(vec_type(0, 0, 0, 0) == vec_type(0, 0, 0, 0));
    REQUIRE(vec_type::filled(1) == vec_type(1, 1, 1, 1));

    REQUIRE(vec_type::infinity() ==
            vec_type(std::numeric_limits<vec_type::component_type>::infinity(),
                std::numeric_limits<vec_type::component_type>::infinity(),
                std::numeric_limits<vec_type::component_type>::infinity(),
                std::numeric_limits<vec_type::component_type>::infinity()));

    REQUIRE(vec_type::negative_infinity() ==
            vec_type(-std::numeric_limits<vec_type::component_type>::infinity(),
                -std::numeric_limits<vec_type::component_type>::infinity(),
                -std::numeric_limits<vec_type::component_type>::infinity(),
                -std::numeric_limits<vec_type::component_type>::infinity()));

    if constexpr (std::is_floating_point_v<vec_type::component_type>)
    {
        auto nan = vec_type::nan();
        REQUIRE(std::isnan(nan.x()));
        REQUIRE(std::isnan(nan.y()));
        REQUIRE(std::isnan(nan.z()));
        REQUIRE(std::isnan(nan.w()));
    }

    REQUIRE(vec_type::left() == vec_type(-1, 0, 0, 0));
    REQUIRE(vec_type::right() == vec_type(1, 0, 0, 0));
    REQUIRE(vec_type::down() == vec_type(0, -1, 0, 0));
    REQUIRE(vec_type::up() == vec_type(0, 1, 0, 0));
    REQUIRE(vec_type::backward() == vec_type(0, 0, -1, 0));
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

        THEN("Vector square length should work")
        {
            auto len = v1.squared_length();
            REQUIRE(len == Approx(4.0));
        }

        if constexpr (std::is_floating_point_v<vec_type::component_type>)
        {
            THEN("Vector length should work")
            {
                auto len = v1.length();
                REQUIRE(len == Approx(2.0));
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
                auto dot = vec_type::dot(v1, v2);
                auto dxm_dot = DirectX::XMVectorGetX(
                    DirectX::XMVector4Dot(dxm_v1, dxm_v2));

                REQUIRE(dot == Approx(dxm_dot));
            }

            THEN("The distance between the points should be correct")
            {
                auto dist = vec_type::distance_between_points(v1, v2);
                auto dxm_dist = DirectX::XMVectorGetX(DirectX::XMVector4Length(
                    DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            THEN("The square distance between the points should be correct")
            {
                auto dist = vec_type::squared_distance_between_points(v1, v2);
                auto dxm_dist =
                    DirectX::XMVectorGetX(DirectX::XMVector4LengthSq(
                        DirectX::XMVectorSubtract(dxm_v1, dxm_v2)));
                REQUIRE(dist == Approx(dxm_dist));
            }

            // Only run for float and double value types
            if constexpr (std::is_floating_point_v<vec_type::component_type>)
            {
                THEN("Vector4 cross should work")
                {
                    auto v3 = vec_type(1, -2, 3, -4);
                    auto dxm_v3 = vec4_to_dxm(v3);

                    auto cross = vec_type::cross(v1, v2, v3);
                    auto dxm_cross = vec4_from_dxm<vec_type>(
                        DirectX::XMVector4Cross(dxm_v1, dxm_v2, dxm_v3));

                    REQUIRE(cross.x() == Approx(dxm_cross.x()));
                    REQUIRE(cross.y() == Approx(dxm_cross.y()));
                    REQUIRE(cross.z() == Approx(dxm_cross.z()));
                    REQUIRE(cross.w() == Approx(dxm_cross.w()));
                }
            }
            if constexpr (std::is_floating_point_v<vec_type::component_type>)
            {
                THEN("Angle between normalized vectors should work")
                {
                    auto v1norm = v1.normalized();
                    auto v2norm = v2.normalized();
                    auto dxm_v1norm = vec4_to_dxm(v1norm);
                    auto dxm_v2norm = vec4_to_dxm(v2norm);
                    auto angle = vec_type::angle_between_normalized_vectors(
                        v1norm, v2norm);
                    auto dxm_angle = DirectX::XMVectorGetX(
                        DirectX::XMVector4AngleBetweenNormals(
                            dxm_v1norm, dxm_v2norm));

                    REQUIRE(angle == Approx(dxm_angle));
                }
            }

            if constexpr (std::is_floating_point_v<vec_type::component_type>)
            {
                THEN("Angle between non-normalized vectors should work")
                {
                    auto angle = vec_type::angle_between_vectors(v1, v2);
                    auto dxm_angle = DirectX::XMVectorGetX(
                        DirectX::XMVector4AngleBetweenVectors(dxm_v1, dxm_v2));

                    REQUIRE(angle == Approx(dxm_angle));
                }

                THEN("Reflection should be correct")
                {
                    auto ref = vec_type::reflect(v1, v2);
                    auto dxm_ref = vec4_from_dxm<vec_type>(
                        DirectX::XMVector4Reflect(dxm_v1, dxm_v2));

                    REQUIRE(ref.x() == Approx(dxm_ref.x()));
                    REQUIRE(ref.y() == Approx(dxm_ref.y()));
                    REQUIRE(ref.z() == Approx(dxm_ref.z()));
                    REQUIRE(ref.w() == Approx(dxm_ref.w()));
                }

                THEN("Refraction should be correct")
                {
                    auto ref = vec_type::refract(v1, v2, 0.5f);
                    auto dxm_ref = vec4_from_dxm<vec_type>(
                        DirectX::XMVector4Refract(dxm_v1, dxm_v2, 0.5f));

                    REQUIRE(ref.x() == Approx(dxm_ref.x()));
                    REQUIRE(ref.y() == Approx(dxm_ref.y()));
                    REQUIRE(ref.z() == Approx(dxm_ref.z()));
                    REQUIRE(ref.w() == Approx(dxm_ref.w()));
                }
            }

            THEN("Min should be correct")
            {
                auto min = vec_type::min(v1, v2);
                auto dxm_min = vec4_from_dxm<vec_type>(
                    DirectX::XMVectorMin(dxm_v1, dxm_v2));

                REQUIRE(min.x() == Approx(dxm_min.x()));
                REQUIRE(min.y() == Approx(dxm_min.y()));
                REQUIRE(min.z() == Approx(dxm_min.z()));
                REQUIRE(min.w() == Approx(dxm_min.w()));
            }

            THEN("Max should be correct")
            {
                auto max = vec_type::max(v1, v2);
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

#undef MVM_TEST_TYPE
#undef __mvm_xstr
#undef __mvm_str
#undef MVM_TEST_NAME
#undef MVM_TEST_SCOPE