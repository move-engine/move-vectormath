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

#include <move/vectormath/quat.hpp>

#include "mvm_test_utils.hpp"

SCENARIO("Test quatf", "[move::vectormath::quatf]")
{
    using Catch::Approx;
    using namespace move::vectormath;
    using matrix_type = mat4f;
    using quat_type = quat4f;
    using vec3_type = typename matrix_type::vec3_type;
    using vec4_type = typename matrix_type::vec4_type;

    WHEN("An identity quaternion is created")
    {
        auto ident = quat_type::identity();
        THEN("It should be valid")
        {
            REQUIRE(!ident.is_nan());
            REQUIRE(!ident.is_infinite());
            REQUIRE(ident.is_identity());
            REQUIRE(ident.length() == 1);
            REQUIRE(ident.squared_length() == 1);
            REQUIRE(ident.reciprocal_length() == 1);
            REQUIRE(ident == ident);
        }

        THEN("It should be equal to (0, 0, 0, 1)")
        {
            REQUIRE(ident == quat_type(0.0f, 0.0f, 0.0f, 1.0f));
        }

        AND_WHEN("It is multiplied against a vector")
        {
            auto vec = vec3_type(1.0f, 2.0f, 3.0f);
            auto result = ident * vec;

            THEN("The result should be the same vector")
            {
                REQUIRE(result == vec);
            }
        }
    }

    WHEN("An angle-axis quaternion is created")
    {
        auto angle = 90.0f;
        auto axis = vec3_type(0.0f, 0.0f, 1.0f);
        auto quat = quat_type::from_axis_angle(axis, angle);

        THEN("It should be valid")
        {
            REQUIRE(!quat.is_nan());
            REQUIRE(!quat.is_infinite());
            REQUIRE(!quat.is_identity());
        }

        THEN("It should be equal to the DXM result")
        {
            auto dxm_quat = DirectX::XMQuaternionRotationAxis(
                DirectX::XMVectorSet(0, 0, 1, 0), angle);
            auto dxm_result = vec4_from_dxm<vec4_type>(dxm_quat);
            REQUIRE(equals(quat, dxm_result));
        }

        THEN("Negation should work")
        {
            auto neg = -quat;
            REQUIRE(neg.x() == -quat.x());
            REQUIRE(neg.y() == -quat.y());
            REQUIRE(neg.z() == -quat.z());
            REQUIRE(neg.w() == -quat.w());
        }

        THEN("Conjugation should be the same as DXM")
        {
            auto conj = quat.conjugate();
            auto dxm_quat = vec4_to_dxm(quat);
            auto dxm_result = DirectX::XMQuaternionConjugate(dxm_quat);
            auto result = vec4_from_dxm<vec4_type>(dxm_result);
            REQUIRE(equals(conj, result));
        }

        THEN("Inversion should be the same as DXM")
        {
            auto inv = quat.inverse();
            auto dxm_quat = vec4_to_dxm(quat);
            auto dxm_result = DirectX::XMQuaternionInverse(dxm_quat);
            auto result = vec4_from_dxm<vec4_type>(dxm_result);
            REQUIRE(equals(inv, result));
        }

        // THEN("quat::ln() should be the same as DXM")
        // {
        //     auto dxm_quat = vec4_to_dxm(quat);
        //     auto dxm_result = DirectX::XMQuaternionLn(dxm_quat);

        //     auto ln = quat.ln();
        //     REQUIRE(equals(ln, dxm_result));
        // }

        // TODO: exp
        // TODO: slerp
    }

    WHEN("Two unequal quaternions are created")
    {
        using move::vectormath::deg_to_rad;
        auto quat1 = quat_type::from_axis_angle(
            vec3_type(0.0f, 0.0f, 1.0f), deg_to_rad(90.0f));
        auto quat2 = quat_type::from_axis_angle(
            vec3_type(0.0f, 0.0f, 1.0f), deg_to_rad(180.0f));

        THEN("They should not be equal")
        {
            REQUIRE(quat1 != quat2);
        }

        THEN("Their dot product should be correct")
        {
            // TODO: Why is the angle half of what I'd expect?
            REQUIRE(
                std::acos(quat1.dot(quat2)) == Approx(deg_to_rad(90.) / 2.));
        }

        THEN("Their dot product should be the same as DXM")
        {
            auto dxm_quat1 = vec4_to_dxm(quat1);
            auto dxm_quat2 = vec4_to_dxm(quat2);
            auto dxm_result = DirectX::XMQuaternionDot(dxm_quat1, dxm_quat2);

            REQUIRE(
                quat1.dot(quat2) == Approx(DirectX::XMVectorGetX(dxm_result)));
        }

        THEN("Multiplying them should have the same result as DXM")
        {
            auto dxm_quat1 = vec4_to_dxm(quat1);
            auto dxm_quat2 = vec4_to_dxm(quat2);
            auto dxm_result =
                DirectX::XMQuaternionMultiply(dxm_quat1, dxm_quat2);
            auto result = vec4_from_dxm<vec4_type>(dxm_result);
            REQUIRE(equals(quat1 * quat2, result));
        }
    }
}