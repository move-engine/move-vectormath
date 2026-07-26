#include <cmath>
#include <cstdint>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include <move/vectormath.hpp>

using namespace move::math;

template <typename Vector>
concept normalizable_vector = requires(Vector value) { value.normalize(); };

static_assert(std::is_same_v<traits::component_type_t<int>, int>);
static_assert(std::is_same_v<decltype(float3::zero()), float3>);
static_assert(std::is_same_v<decltype(float3{} + float3{}), float3>);
static_assert(
    std::is_same_v<decltype(float3::cross(float3{}, float3{})), float3>);
static_assert(std::is_same_v<decltype(float3{}.normalized()), float3>);
static_assert(std::is_same_v<decltype(float4::x_axis()), float4>);
static_assert(std::is_same_v<decltype(float4{} + float4{}), float4>);
static_assert(
    std::is_same_v<decltype(float4::lerp(float4{}, float4{}, 0.5f)), float4>);
static_assert(std::is_same_v<decltype(float4{}.xy()), float2>);
static_assert(traits::is_matrix_type_v<mat3x3f>);
static_assert(traits::component_count_v<mat3x3f> == 9);
static_assert(traits::is_matrix_type_v<storage_mat4x4<float>>);
static_assert(
    std::is_same_v<traits::component_type_t<storage_mat4x4<float>>, float>);
static_assert(normalizable_vector<float3>);
static_assert(!normalizable_vector<int3>);

void require(bool condition)
{
    if (!condition)
    {
        std::abort();
    }
}

int main()
{
    require(saturate(-2) == 0);
    require(saturate(0) == 0);
    require(saturate(2) == 1);

    const float infinity = std::numeric_limits<float>::infinity();
    const float nan = std::numeric_limits<float>::quiet_NaN();
    require(approx_equal(infinity, infinity));
    require(!approx_equal(nan, nan));
    require(!approx_equal(std::numeric_limits<std::int64_t>::min(),
                          std::numeric_limits<std::int64_t>::max(),
                          std::int64_t(1)));

    const float3 rotated =
        float3::forward() * quatf::rotation_y(deg2rad(90.0f));
    require(approx_equal(rotated, float3::right(), 1.0e-4f));

    const storage_float4 scalar_tiny(1.0e-4f, 0.0f, 0.0f, 0.0f);
    const fast_float4 rtm_tiny(1.0e-4f, 0.0f, 0.0f, 0.0f);
    require(approx_equal(scalar_tiny.normalized(), storage_float4::x_axis(),
                         1.0e-5f));
    require(
        approx_equal(rtm_tiny.normalized(), fast_float4::x_axis(), 1.0e-5f));
    require(storage_float4::zero().normalized() == storage_float4::zero());
    require(fast_float4::zero().normalized() == fast_float4::zero());
    require(storage_float4::angle_between_vectors(
                storage_float4::zero(), storage_float4::x_axis()) == 0.0f);
    require(fast_float4::angle_between_vectors(fast_float4::zero(),
                                               fast_float4::x_axis()) == 0.0f);

    const quatf negative_w =
        quatf::angle_axis(float3::x_axis(), deg2rad(270.0f));
    require(approx_equal(negative_w.ln().exp(), negative_w, 1.0e-4f));

    const quatf non_unit(1.0f, 2.0f, 3.0f, 4.0f);
    require(approx_equal(non_unit.ln().exp(), non_unit, 1.0e-4f));

    require(same_rotation(quatf::look_rotation(float3::forward(), float3::up()),
                          quatf::identity(), 1.0e-4f));
    const quatf look_right =
        quatf::look_rotation(float3::right(), float3::up());
    require(
        approx_equal(float3::forward() * look_right, float3::right(), 1.0e-4f));
    require(quatf::look_rotation(float3::zero(), float3::up()) ==
            quatf::identity());
    require(
        approx_equal(quatf::look_rotation(float3::up(), float3::up()).length(),
                     1.0f, 1.0e-4f));

    const quatf identity = quatf::identity();
    const quatf close(0.0f, 0.0f, 0.0f, 1.0f - 1.0e-6f);
    const quatf negative_identity(-identity.get_x(), -identity.get_y(),
                                  -identity.get_z(), -identity.get_w());
    require(identity != close);
    require(approx_equal(identity, close, 1.0e-5f));
    require(same_rotation(identity, negative_identity, 1.0e-5f));

    storage_mat3x3f stored;
    const mat3x3f restored = stored;
    require(restored == mat3x3f::identity());
    require(approx_equal(float2(1.0f, 2.0f), float2(1.0f, 2.0f)));
}
