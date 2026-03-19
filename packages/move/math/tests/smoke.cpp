#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <type_traits>

#include <move/vectormath.hpp>

#include "mm_test_common.hpp"

static_assert(move::math::fast_float2::acceleration ==
              move::math::Acceleration::Scalar);
static_assert(
    std::is_same_v<move::math::byte2::component_type, std::uint8_t>);
static_assert(
    std::is_same_v<move::math::byte3::component_type, std::uint8_t>);
static_assert(
    std::is_same_v<move::math::byte4::component_type, std::uint8_t>);
static_assert(std::is_same_v<move::math::sbyte2::component_type, std::int8_t>);
static_assert(std::is_same_v<move::math::sbyte3::component_type, std::int8_t>);
static_assert(std::is_same_v<move::math::sbyte4::component_type, std::int8_t>);

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
        const quatf const_value = value;
        quatf mutable_value = value;
        quatf normalized_copy = const_value.normalized();
        quatf& normalized_in_place = mutable_value.normalize();

        THEN("The components are loaded correctly")
        {
            REQUIRE(value.get_x() == Catch::Approx(1.0f));
            REQUIRE(value.get_y() == Catch::Approx(2.0f));
            REQUIRE(value.get_z() == Catch::Approx(3.0f));
            REQUIRE(value.get_w() == Catch::Approx(4.0f));
            REQUIRE(normalized_copy.length() == Catch::Approx(1.0f));
            REQUIRE(&normalized_in_place == &mutable_value);
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
        mat3x3f rotation_copy = rotation;
        mat4x4f translation = mat4x4f::translation(float3(1.0f, 2.0f, 3.0f));
        quatf rotation_quat = quatf::identity();
        float3 near_vec3_a(1.0f, 2.0f, 3.0f);
        float3 near_vec3_b(1.0005f, 2.0f, 3.0f);
        float4 near_vec4_a(1.0f, 2.0f, 3.0f, 4.0f);
        float4 near_vec4_b(1.0005f, 2.0f, 3.0f, 4.0f);

        THEN("The umbrella header exposes the full public math surface")
        {
            REQUIRE(rotation.determinant() == Catch::Approx(1.0f));
            REQUIRE(translation == mat4x4f(1, 0, 0, 0,
                                           0, 1, 0, 0,
                                           0, 0, 1, 0,
                                           1, 2, 3, 1));
            REQUIRE(rotation_quat == quatf::identity());
            REQUIRE(sign(0.0f) == Catch::Approx(0.0f));
            REQUIRE(sign(-5.0f) == Catch::Approx(-1.0f));
            REQUIRE(sign(5.0f) == Catch::Approx(1.0f));
            REQUIRE(approx_equal(near_vec3_a, near_vec3_b, 0.001f));
            REQUIRE_FALSE(approx_equal(near_vec3_a, near_vec3_b, 0.0001f));
            REQUIRE(approx_equal(near_vec4_a, near_vec4_b, 0.001f));
            REQUIRE_FALSE(approx_equal(near_vec4_a, near_vec4_b, 0.0001f));
            REQUIRE(approx_equal(rotation_quat, quatf::identity()));
            REQUIRE(approx_equal(rotation, rotation_copy));
            REQUIRE(approx_equal(translation,
                                 mat4x4f(1, 0, 0, 0,
                                         0, 1, 0, 0,
                                         0, 0, 1, 0,
                                         1, 2, 3, 1)));
        }
    }
}
