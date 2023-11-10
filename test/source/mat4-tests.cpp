// For MSVC, disable C4244: 'argument': conversion from 'value_type' to 'float',
// possible loss of data
#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable : 4244)
#endif

#include <math.h>
#include <stdexcept>

#include <catch2/catch_all.hpp>
#include <cereal/archives/binary.hpp>
#include "DirectXMath.h"
#include "catch2/catch_approx.hpp"
#include "catch2/catch_message.hpp"
#include "catch2/catch_test_macros.hpp"
#include "catch2/internal/catch_source_line_info.hpp"
#include "nlohmann/json_fwd.hpp"

// #include <move/vectormath.hpp>
#include <move/vectormath/mat4.hpp>
#include <move/vectormath/quat.hpp>

#include "mvm_test_utils.hpp"

SCENARIO("Test mat4f", "[move::vectormath::mat4f]")
{
    using namespace move::vectormath;
    using matrix_type = mat4f;
    using quat_type = quatf;
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

    constexpr double pi = 3.14159265358979323846;
    WHEN("A transformation matrix with only rotation is created")
    {
        auto trans = vec3_type(0, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), pi);
        auto scale = vec3_type(1, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with only translation is created")
    {
        auto trans = vec3_type(100, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), 0);
        auto scale = vec3_type(1, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with only scale is created")
    {
        auto trans = vec3_type(0, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), 0);
        auto scale = vec3_type(100, 10, 0.5);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with all components is created")
    {
        auto trans = vec3_type(110, -30, 50);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), pi / 2);
        auto scale = vec3_type(10, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);

            REQUIRE(equals(res, dxmres));
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

    GIVEN("A matrix and a vector4")
    {
        auto mat = matrix_type::create_translation(1.0f, 2.0f, 3.0f);
        auto vec = vec4_type(1.0f, 2.0f, 3.0f, 1.0f);

        WHEN("They are multiplied")
        {
            auto res = vec * mat;
            THEN("They should match the DXM results")
            {
                auto dxmmat = mat4_to_dxm(mat);
                auto dxmvec = vec4_to_dxm(vec);
                auto dxmres = DirectX::XMVector4Transform(dxmvec, dxmmat);
                REQUIRE(equals(res, dxmres));
            }
        }
    }
}

SCENARIO("Test mat4d", "[move::vectormath::mat4d]")
{
    using namespace move::vectormath;
    using matrix_type = mat4d;
    using quat_type = quatd;
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

    constexpr double pi = 3.14159265358979323846;
    WHEN("A transformation matrix with only rotation is created")
    {
        auto trans = vec3_type(0, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), pi);
        auto scale = vec3_type(1, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with only translation is created")
    {
        auto trans = vec3_type(100, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), 0);
        auto scale = vec3_type(1, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with only scale is created")
    {
        auto trans = vec3_type(0, 0, 0);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), 0);
        auto scale = vec3_type(100, 10, 0.5);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);
            REQUIRE(equals(res, dxmres));
        }
    }

    WHEN("A transformation matrix with all components is created")
    {
        auto trans = vec3_type(110, -30, 50);
        auto rot = quat_type::from_rotation_axis(vec3_type::up(), pi / 2);
        auto scale = vec3_type(10, 1, 1);

        auto mat = matrix_type::create_transformation(trans, rot, scale);
        THEN(
            "Multiplying a point by the matrix should yield the correct result")
        {
            auto zero = DirectX::XMVectorSet(0, 0, 0, 0);
            auto dxmtrans =
                DirectX::XMVectorSet(trans.x(), trans.y(), trans.z(), 0);

            auto dxmrot =
                DirectX::XMVectorSet(rot.x(), rot.y(), rot.z(), rot.w());

            auto dxmscale =
                DirectX::XMVectorSet(scale.x(), scale.y(), scale.z(), 0);

            auto mat_dxm = DirectX::XMMatrixAffineTransformation(
                dxmscale, zero, dxmrot, dxmtrans);

            auto point = vec4_type(1, 2, 3, 1);
            auto dxmpoint = vec4_to_dxm(point);

            auto res = point * mat;
            auto dxmres = DirectX::XMVector4Transform(dxmpoint, mat_dxm);

            REQUIRE(equals(res, dxmres));
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

    GIVEN("A matrix and a vector4")
    {
        auto mat = matrix_type::create_translation(1.0f, 2.0f, 3.0f);
        auto vec = vec4_type(1.0f, 2.0f, 3.0f, 1.0f);

        WHEN("They are multiplied")
        {
            auto res = vec * mat;
            THEN("They should match the DXM results")
            {
                auto dxmmat = mat4_to_dxm(mat);
                auto dxmvec = vec4_to_dxm(vec);
                auto dxmres = DirectX::XMVector4Transform(dxmvec, dxmmat);
                REQUIRE(equals(res, dxmres));
            }
        }
    }
}

#ifdef _MSC_VER
#pragma warning(pop)
#endif