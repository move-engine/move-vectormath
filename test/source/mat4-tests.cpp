#include <math.h>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "catch2/catch_approx.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_source_line_info.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/mat4.hpp>
#include <sstream>
#include <stdexcept>

#include <DirectXMath.h>

template <typename mat_type>
inline mat_type mat4_from_dxm(const DirectX::XMMATRIX& vec)
{
    using namespace DirectX;
    using value_type = typename mat_type::component_type;
    using vector_type = typename mat_type::vector_type;
    using vector_type_raw = typename vector_type::underlying_type;
    using vector_type_raw_type = typename vector_type_raw::type;

    auto row0 = XMVectorGetX(vec.r[0]);
    auto row1 = XMVectorGetX(vec.r[1]);
    auto row2 = XMVectorGetX(vec.r[2]);
    auto row3 = XMVectorGetX(vec.r[3]);

    return mat_type(vector_type_raw_type(row0), vector_type_raw_type(row1),
        vector_type_raw_type(row2), vector_type_raw_type(row3));
}

template <typename mat_type>
inline DirectX::XMMATRIX mat4_to_dxm(const mat_type& mat)
{
    using namespace DirectX;
    using value_type = typename mat_type::component_type;

    auto row0 =
        XMVectorSet(value_type(mat.get(0, 0)), value_type(mat.get(0, 1)),
            value_type(mat.get(0, 2)), value_type(mat.get(0, 3)));
    auto row1 =
        XMVectorSet(value_type(mat.get(1, 0)), value_type(mat.get(1, 1)),
            value_type(mat.get(1, 2)), value_type(mat.get(1, 3)));
    auto row2 =
        XMVectorSet(value_type(mat.get(2, 0)), value_type(mat.get(2, 1)),
            value_type(mat.get(2, 2)), value_type(mat.get(2, 3)));
    auto row3 =
        XMVectorSet(value_type(mat.get(3, 0)), value_type(mat.get(3, 1)),
            value_type(mat.get(3, 2)), value_type(mat.get(3, 3)));

    return XMMATRIX(row0, row1, row2, row3);
}

template <typename mat_type>
inline bool equals(const mat_type& mvm, const DirectX::XMMATRIX& dxm)
{
    using Catch::Approx;
    for (int i = 0; i < 4; ++i)
    {
        for (int j = 0; j < 4; ++j)
        {
            auto mvm_val = mvm.get(i, j);
            auto dxm_val = DirectX::XMVectorGetByIndex(dxm.r[i], j);
            if (mvm_val != Approx(dxm_val))
            {
                // Report to catch that there was an error
                std::stringstream ss;
                ss << "mvm[" << i << "][" << j << "] != dxm[" << i << "][" << j
                   << "] (" << mvm_val << " != " << dxm_val << ")";

                throw std::runtime_error(ss.str());

                return false;
            }
        }
    }
    return true;
}

SCENARIO("Test mat4f", "[move::vectormath::mat4f]")
{
    using namespace move::vectormath;
    using matrix_type = mat4f;
    using vec3_type = typename matrix_type::vec3_type;
    using vec4_type = typename matrix_type::vec4_type;

    WHEN("An identity matrix is created")
    {
        auto ident = matrix_type::create_identity();
        THEN("It should match the DirectXMath identity matrix")
        {
            auto ident_dxm = DirectX::XMMatrixIdentity();
            auto ident_dxm2 = mat4_to_dxm(ident);
            REQUIRE(equals(ident, ident_dxm));
        }
    }

    WHEN("An X rotation matrix is created")
    {
        auto rot = matrix_type::create_rotation_x(1.0f);
        THEN("It should match the DirectXMath rotation matrix")
        {
            auto rot_dxm = DirectX::XMMatrixRotationX(1.0f);
            REQUIRE(equals(rot, rot_dxm));
        }
    }

    WHEN("A Y rotation matrix is created")
    {
        auto rot = matrix_type::create_rotation_y(1.0f);
        THEN("It should match the DirectXMath rotation matrix")
        {
            auto rot_dxm = DirectX::XMMatrixRotationY(1.0f);
            REQUIRE(equals(rot, rot_dxm));
        }
    }

    WHEN("A Z rotation matrix is created")
    {
        auto rot = matrix_type::create_rotation_z(1.0f);
        THEN("It should match the DirectXMath rotation matrix")
        {
            auto rot_dxm = DirectX::XMMatrixRotationZ(1.0f);
            REQUIRE(equals(rot, rot_dxm));
        }
    }

    WHEN("A translation matrix is created")
    {
        auto mat = matrix_type::create_translation(1.0f, 2.0f, 3.0f);
        THEN("It should match the DirectXMath translation matrix")
        {
            auto mat_dxm = DirectX::XMMatrixTranslation(1.0f, 2.0f, 3.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("A scaling matrix is created")
    {
        auto mat = matrix_type::create_scale(1.0f, 2.0f, 3.0f);
        THEN("It should match the DirectXMath scaling matrix")
        {
            auto mat_dxm = DirectX::XMMatrixScaling(1.0f, 2.0f, 3.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An RH perspective matrix is created")
    {
        auto mat = matrix_type::create_perspective_rh(1.0f, 2.0f, 3.0f, 4.0f);
        THEN("It should match the DirectXMath perspective matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixPerspectiveFovRH(1.0f, 2.0f, 3.0f, 4.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("A LH perspective matrix is created")
    {
        auto mat = matrix_type::create_perspective_lh(1.0f, 2.0f, 3.0f, 4.0f);
        THEN("It should match the DirectXMath perspective matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixPerspectiveFovLH(1.0f, 2.0f, 3.0f, 4.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An RH ortho matrix is created")
    {
        auto mat = matrix_type::create_ortho_rh(1.0f, 2.0f, 3.0f, 4.0f);
        THEN("It should match the DirectXMath ortho matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixOrthographicRH(1.0f, 2.0f, 3.0f, 4.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("A LH ortho matrix is created")
    {
        auto mat = matrix_type::create_ortho_lh(1.0f, 2.0f, 3.0f, 4.0f);
        THEN("It should match the DirectXMath ortho matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixOrthographicLH(1.0f, 2.0f, 3.0f, 4.0f);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An off-center RH matrix is created")
    {
        auto mat = matrix_type::create_ortho_off_center_rh(
            1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);

        THEN("It should match the DirectXMath off-center matrix")
        {
            auto mat_dxm = DirectX::XMMatrixOrthographicOffCenterRH(
                1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An off-center LH matrix is created")
    {
        auto mat = matrix_type::create_ortho_off_center_lh(
            1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0f);

        THEN("It should match the DirectXMath off-center matrix")
        {
            auto mat_dxm = DirectX::XMMatrixOrthographicOffCenterLH(
                1.0f, 2.0f, 3.0f, 4.0f, 5.0f, 6.0);
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An RH look-at matrix is created")
    {
        auto mat = matrix_type::create_look_at_rh(
            vec3_type(10, 10, 10), vec3_type(0, 0, 0), vec3_type(0, 1, 0));

        THEN("It should match the DirectXMath look-at matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixLookAtRH(DirectX::XMVectorSet(10, 10, 10, 0),
                    DirectX::XMVectorSet(0, 0, 0, 0),
                    DirectX::XMVectorSet(0, 1, 0, 0));
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("An RH look-at matrix is created")
    {
        auto mat = matrix_type::create_look_at_lh(
            vec3_type(10, 10, 10), vec3_type(0, 0, 0), vec3_type(0, 1, 0));

        THEN("It should match the DirectXMath look-at matrix")
        {
            auto mat_dxm =
                DirectX::XMMatrixLookAtLH(DirectX::XMVectorSet(10, 10, 10, 0),
                    DirectX::XMVectorSet(0, 0, 0, 0),
                    DirectX::XMVectorSet(0, 1, 0, 0));
            REQUIRE(equals(mat, mat_dxm));
        }
    }

    WHEN("A transformation matrix is created")
    {
        auto trans = vec3_type(1, 2, 3);
        auto rot = vec4_type(1, 2, 3, 4)
                       .normalized();  // TODO: Refactor this once
                                       // quats are implemented with RTM
        auto scale = vec3_type(1, 2, 3);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN("It should match the DirectXMath transformation matrix")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixTransformation(
                zero, dxmrot, dxmscale, zero, dxmrot, dxmtrans);
            // TODO: Fix this
            // REQUIRE(equals(mat, mat_dxm));
        }
    }

    GIVEN("Two sets of matrices")
    {
        auto mat1 = matrix_type::create_translation(1.0f, 2.0f, 3.0f);
        auto mat2 = matrix_type::create_scale(1.0f, 2.0f, 3.0f);
        auto dxm1 = mat4_to_dxm(mat1);
        auto dxm2 = mat4_to_dxm(mat2);

        REQUIRE(equals(mat1, dxm1));
        REQUIRE(equals(mat2, dxm2));

        WHEN("They are multiplied")
        {
            auto mat3 = mat1 * mat2;
            auto dxm3 = DirectX::XMMatrixMultiply(dxm1, dxm2);

            THEN("They should match")
            {
                REQUIRE(equals(mat3, dxm3));
            }
        }
    }
}