#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <move/vectormath.hpp>

#include "mm_test_common.hpp"

SCENARIO("Umbrella header smoke tests")
{
    using namespace move::math;

    WHEN("Using vec4 helpers through move/vectormath.hpp")
    {
        float4 value(1.0f, 2.0f, 3.0f, 4.0f);
        float4 normalized = value.normalized();

        THEN("The normalized vector has unit length")
        {
            REQUIRE(normalized.length() == Catch::Approx(1.0f));
        }

        THEN("The angle helper returns the expected angle")
        {
            float angle = float4::angle_between_vectors(
                float4(1.0f, 0.0f, 0.0f, 0.0f),
                float4(0.0f, 1.0f, 0.0f, 0.0f));
            REQUIRE(angle == Catch::Approx(pi<float>() / 2.0f));
        }
    }

    WHEN("Loading quaternion components from an array")
    {
        quatf value;
        float data[4] = {1.0f, 2.0f, 3.0f, 4.0f};
        value.load_array(data);

        THEN("The components are loaded correctly")
        {
            REQUIRE(value.get_x() == Catch::Approx(1.0f));
            REQUIRE(value.get_y() == Catch::Approx(2.0f));
            REQUIRE(value.get_z() == Catch::Approx(3.0f));
            REQUIRE(value.get_w() == Catch::Approx(4.0f));
        }
    }

    WHEN("Normalizing and serializing vec2 values")
    {
        storage_float2 value(3.0f, 4.0f);
        value.normalize();

        capture_archive<float> saver;
        value.serialize(saver);

        storage_float2 loaded;
        replay_archive<float> loader{saver.values};
        loaded.serialize(loader);

        THEN("Normalization works")
        {
            REQUIRE(value.length() == Catch::Approx(1.0f));
        }

        THEN("Serialization round-trips the values")
        {
            REQUIRE(loaded.get_x() == Catch::Approx(value.get_x()));
            REQUIRE(loaded.get_y() == Catch::Approx(value.get_y()));
        }
    }

    WHEN("Constructing matrix and quaternion types through the umbrella header")
    {
        mat3x3f rotation =
            mat3x3f::rotation(quatf::rotation_y(deg2rad(90.0f)));
        mat4x4f translation = mat4x4f::translation(float3(1.0f, 2.0f, 3.0f));
        quatf rotation_quat = quatf::identity();

        THEN("The umbrella header exposes the full public math surface")
        {
            REQUIRE(rotation.determinant() == Catch::Approx(1.0f));
            REQUIRE(translation == mat4x4f(1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           1, 2, 3, 1));
            REQUIRE(rotation_quat == quatf::identity());
        }
    }
}
