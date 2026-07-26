#include <move/vectormath.hpp>

int main()
{
    using namespace move::math;

    const float3 position(1.0F, 2.0F, 3.0F);
    const mat4x4f transform = mat4x4f::trs(float3(4.0F, 5.0F, 6.0F),
                                           quatf::identity(), float3::one());
    const float3 transformed = transform.transform_point(position);

    return approx_equal(transformed, float3(5.0F, 7.0F, 9.0F), 0.0001F) ? 0 : 1;
}
