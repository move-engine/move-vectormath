#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <cstdint>
#include <limits>
#include <type_traits>

#include <move/vectormath.hpp>

#include "mm_test_common.hpp"

static_assert(move::math::fast_float2::acceleration ==
              move::math::Acceleration::Scalar);
static_assert(std::is_same_v<move::math::byte2::component_type, std::uint8_t>);
static_assert(std::is_same_v<move::math::byte3::component_type, std::uint8_t>);
static_assert(std::is_same_v<move::math::byte4::component_type, std::uint8_t>);
static_assert(std::is_same_v<move::math::sbyte2::component_type, std::int8_t>);
static_assert(std::is_same_v<move::math::sbyte3::component_type, std::int8_t>);
static_assert(std::is_same_v<move::math::sbyte4::component_type, std::int8_t>);
static_assert(std::is_same_v<move::math::traits::component_type_t<int>, int>);
static_assert(
    std::is_same_v<decltype(move::math::float3::zero()), move::math::float3>);
static_assert(
    std::is_same_v<decltype(move::math::float3{} + move::math::float3{}),
                   move::math::float3>);
static_assert(
    std::is_same_v<decltype(move::math::float4::x_axis()), move::math::float4>);
static_assert(
    std::is_same_v<decltype(move::math::float4{} + move::math::float4{}),
                   move::math::float4>);
static_assert(
    std::is_same_v<decltype(move::math::float4{}.xy()), move::math::float2>);
static_assert(move::math::traits::is_matrix_type_v<move::math::mat3x3f>);
static_assert(move::math::traits::component_count_v<move::math::mat3x3f> == 9);

template <typename Vector>
concept normalizable_vector = requires(Vector value) { value.normalize(); };

static_assert(normalizable_vector<move::math::float3>);
static_assert(!normalizable_vector<move::math::int3>);

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
                float4(1.0f, 0.0f, 0.0f, 0.0f), float4(0.0f, 1.0f, 0.0f, 0.0f));
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
        mat3x3f rotation = mat3x3f::rotation(quatf::rotation_y(deg2rad(90.0f)));
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
            REQUIRE(translation ==
                    mat4x4f(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1));
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
            REQUIRE(approx_equal(translation, mat4x4f(1, 0, 0, 0, 0, 1, 0, 0, 0,
                                                      0, 1, 0, 1, 2, 3, 1)));
        }
    }
}

TEST_CASE("Audited numerical and API contracts")
{
    using namespace move::math;

    SECTION("Signed saturation clamps to the unit interval")
    {
        REQUIRE(saturate(-2) == 0);
        REQUIRE(saturate(0) == 0);
        REQUIRE(saturate(2) == 1);
    }

    SECTION("Approximate equality handles special and extreme values")
    {
        const float infinity = std::numeric_limits<float>::infinity();
        const float nan = std::numeric_limits<float>::quiet_NaN();
        REQUIRE(approx_equal(infinity, infinity));
        REQUIRE_FALSE(approx_equal(nan, nan));
        REQUIRE_FALSE(approx_equal(std::numeric_limits<std::int64_t>::min(),
                                   std::numeric_limits<std::int64_t>::max(),
                                   std::int64_t(1)));
    }

    SECTION("Scalar and RTM normalization share zero and tiny-vector behavior")
    {
        const storage_float4 scalar_tiny(1.0e-4f, 0.0f, 0.0f, 0.0f);
        const fast_float4 rtm_tiny(1.0e-4f, 0.0f, 0.0f, 0.0f);
        REQUIRE(approx_equal(scalar_tiny.normalized(), storage_float4::x_axis(),
                             1.0e-5f));
        REQUIRE(approx_equal(rtm_tiny.normalized(), fast_float4::x_axis(),
                             1.0e-5f));
        REQUIRE(storage_float4::zero().normalized() == storage_float4::zero());
        REQUIRE(fast_float4::zero().normalized() == fast_float4::zero());
        REQUIRE(storage_float4::angle_between_vectors(
                    storage_float4::zero(), storage_float4::x_axis()) == 0.0f);
        REQUIRE(fast_float4::angle_between_vectors(
                    fast_float4::zero(), fast_float4::x_axis()) == 0.0f);
    }

    SECTION("Documented vector quaternion rotation compiles and works")
    {
        quatf rotation = quatf::rotation_y(deg2rad(90.0f));
        float3 rotated = float3::forward() * rotation;
        REQUIRE(approx_equal(rotated, float3::right(), 1.0e-4f));
    }

    SECTION("Quaternion logarithm and exponential round-trip")
    {
        const quatf negative_w =
            quatf::angle_axis(float3::x_axis(), deg2rad(270.0f));
        REQUIRE(approx_equal(negative_w.ln().exp(), negative_w, 1.0e-4f));

        const quatf non_unit(1.0f, 2.0f, 3.0f, 4.0f);
        REQUIRE(approx_equal(non_unit.ln().exp(), non_unit, 1.0e-4f));
    }

    SECTION("Look rotation handles identity and degenerate inputs")
    {
        REQUIRE(
            same_rotation(quatf::look_rotation(float3::forward(), float3::up()),
                          quatf::identity(), 1.0e-4f));
        const quatf look_right =
            quatf::look_rotation(float3::right(), float3::up());
        REQUIRE(approx_equal(float3::forward() * look_right, float3::right(),
                             1.0e-4f));
        REQUIRE(quatf::look_rotation(float3::zero(), float3::up()) ==
                quatf::identity());

        const quatf collinear =
            quatf::look_rotation(float3::up(), float3::up());
        REQUIRE(collinear.length() == Catch::Approx(1.0f));
    }

    SECTION("Quaternion equality is exact and rotation equivalence is explicit")
    {
        const quatf identity = quatf::identity();
        const quatf close(0.0f, 0.0f, 0.0f, 1.0f - 1.0e-6f);
        const quatf negative_identity(-identity.get_x(), -identity.get_y(),
                                      -identity.get_z(), -identity.get_w());
        REQUIRE_FALSE(identity == close);
        REQUIRE(approx_equal(identity, close, 1.0e-5f));
        REQUIRE(same_rotation(identity, negative_identity, 1.0e-5f));
    }

    SECTION("Storage and vec2 parity APIs are available")
    {
        storage_mat3x3f stored;
        mat3x3f restored = stored;
        REQUIRE(restored == mat3x3f::identity());
        REQUIRE(approx_equal(float2(1.0f, 2.0f), float2(1.0f, 2.0f)));
    }
}
