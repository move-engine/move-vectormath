#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/vec4.hpp>
#include <move/meta/type_utils.hpp>
#include "catch2/catch_approx.hpp"

template <typename vec4>
inline void test_vec4()
{
    using component_type = vec4::component_type;
    using vec2 = vec4::vec2_t;
    using vec3 = vec4::vec3_t;
    static constexpr auto acceleration = vec4::acceleration;

    INFO("Testing vec4 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec2: " << move::meta::type_name<vec2>());
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    REQUIRE(vec4::zero() == vec4(0, 0, 0, 0));
    REQUIRE(vec4::one() == vec4(1, 1, 1, 1));

    REQUIRE(vec4(1, 1, 1, 1) == vec4(1, 1, 1, 1));
    REQUIRE(vec4(0, 0, 0, 0) == vec4(0, 0, 0, 0));
    REQUIRE(vec4::filled(1) == vec4(1, 1, 1, 1));

    REQUIRE(vec4::infinity() ==
            vec4(std::numeric_limits<component_type>::infinity(),
                 std::numeric_limits<component_type>::infinity(),
                 std::numeric_limits<component_type>::infinity(),
                 std::numeric_limits<component_type>::infinity()));

    REQUIRE(vec4::negative_infinity() ==
            vec4(-std::numeric_limits<component_type>::infinity(),
                 -std::numeric_limits<component_type>::infinity(),
                 -std::numeric_limits<component_type>::infinity(),
                 -std::numeric_limits<component_type>::infinity()));

    REQUIRE(vec4(vec2(1, 2)) == vec4(1, 2, 0, 0));

    WHEN("A default vec4 is created")
    {
        vec4 test;

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 0);
            REQUIRE(test.get_y() == 0);
            REQUIRE(test.get_z() == 0);
            REQUIRE(test.get_w() == 0);
        }
    }

    WHEN("A vec4 is created with 3 values")
    {
        vec4 test(1, 2, 3);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
            REQUIRE(test.get_w() == 0);
        }
    }

    WHEN("A vec4 is created with values in {} notation")
    {
        vec4 test = {1, 2, 3, 4};

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
            REQUIRE(test.get_w() == 4);
        }
    }

    WHEN("A vec4 is created with 4 values and swizzled")
    {
        vec4 test(1, 2, 3, 4);

        // vec2
        REQUIRE(test.xy() == vec2(1, 2));
        REQUIRE(test.xz() == vec2(1, 3));
        REQUIRE(test.xw() == vec2(1, 4));

        REQUIRE(test.yx() == vec2(2, 1));
        REQUIRE(test.yz() == vec2(2, 3));
        REQUIRE(test.yw() == vec2(2, 4));

        REQUIRE(test.zx() == vec2(3, 1));
        REQUIRE(test.zy() == vec2(3, 2));
        REQUIRE(test.zw() == vec2(3, 4));

        REQUIRE(test.wx() == vec2(4, 1));
        REQUIRE(test.wy() == vec2(4, 2));
        REQUIRE(test.wz() == vec2(4, 3));

        // vec3
        REQUIRE(test.xyz() == vec3(1, 2, 3));
        REQUIRE(test.xyw() == vec3(1, 2, 4));

        REQUIRE(test.xzy() == vec3(1, 3, 2));
        REQUIRE(test.xzw() == vec3(1, 3, 4));

        REQUIRE(test.xwy() == vec3(1, 4, 2));
        REQUIRE(test.xwz() == vec3(1, 4, 3));

        REQUIRE(test.yxz() == vec3(2, 1, 3));
        REQUIRE(test.yxw() == vec3(2, 1, 4));

        REQUIRE(test.yzx() == vec3(2, 3, 1));
        REQUIRE(test.yzw() == vec3(2, 3, 4));

        REQUIRE(test.ywx() == vec3(2, 4, 1));
        REQUIRE(test.ywz() == vec3(2, 4, 3));

        REQUIRE(test.zxy() == vec3(3, 1, 2));
        REQUIRE(test.zxw() == vec3(3, 1, 4));

        REQUIRE(test.zyx() == vec3(3, 2, 1));
        REQUIRE(test.zyw() == vec3(3, 2, 4));

        REQUIRE(test.zwx() == vec3(3, 4, 1));
        REQUIRE(test.zwy() == vec3(3, 4, 2));

        REQUIRE(test.wxy() == vec3(4, 1, 2));
        REQUIRE(test.wxz() == vec3(4, 1, 3));

        REQUIRE(test.wyx() == vec3(4, 2, 1));
        REQUIRE(test.wyz() == vec3(4, 2, 3));

        REQUIRE(test.wzx() == vec3(4, 3, 1));
        REQUIRE(test.wzy() == vec3(4, 3, 2));
    }

    WHEN("Two vec4s are added")
    {
        vec4 added = vec4(1, 2, 3, 4) + vec4(3, 4, 5, 6);

        THEN("The result is correct")
        {
            REQUIRE(added.get_x() == 4);
            REQUIRE(added.get_y() == 6);
            REQUIRE(added.get_z() == 8);
            REQUIRE(added.get_w() == 10);
        }
    }

    WHEN("Two vec4s are subtracted")
    {
        vec4 subtracted = vec4(2, 4, 6, 8) - vec4(1, 2, 3, 4);

        THEN("The result is correct")
        {
            REQUIRE(subtracted.get_x() == 1);
            REQUIRE(subtracted.get_y() == 2);
            REQUIRE(subtracted.get_z() == 3);
            REQUIRE(subtracted.get_w() == 4);
        }
    }

    WHEN("Two vec4s are multiplied")
    {
        vec4 multiplied = vec4(2, 4, 6, 8) * vec4(3, 5, 7, 9);

        THEN("The result is correct")
        {
            REQUIRE(multiplied.get_x() == 6);
            REQUIRE(multiplied.get_y() == 20);
            REQUIRE(multiplied.get_z() == 42);
            REQUIRE(multiplied.get_w() == 72);
        }
    }

    WHEN("Two vec4s are divided")
    {
        vec4 divided = vec4(6, 20, 42, 72) / vec4(2, 4, 6, 8);

        THEN("The result is correct")
        {
            REQUIRE(divided.get_x() == 3);
            REQUIRE(divided.get_y() == 5);
            REQUIRE(divided.get_z() == 7);
            REQUIRE(divided.get_w() == 9);
        }
    }

    WHEN("A vec4 is assigned to another vec4")
    {
        vec4 test = {1, 2, 3, 4};
        vec4 test2;
        test2 = test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 1);
            REQUIRE(test2.get_y() == 2);
            REQUIRE(test2.get_z() == 3);
            REQUIRE(test2.get_w() == 4);
        }
    }

    WHEN("A vec4 is add-assigned to another vec4")
    {
        vec4 test = {1, 2, 3, 4};
        vec4 test2 = {3, 4, 5, 6};
        test2 += test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 4);
            REQUIRE(test2.get_y() == 6);
            REQUIRE(test2.get_z() == 8);
            REQUIRE(test2.get_w() == 10);
        }
    }

    WHEN("A vec4 is subtract-assigned to another vec4")
    {
        vec4 test = {1, 2, 3, 4};
        vec4 test2 = {3, 4, 5, 6};
        test2 -= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 2);
            REQUIRE(test2.get_y() == 2);
            REQUIRE(test2.get_z() == 2);
            REQUIRE(test2.get_w() == 2);
        }
    }

    WHEN("A vec4 is multiply-assigned to another vec4")
    {
        vec4 test = {1, 2, 3, 4};
        vec4 test2 = {3, 4, 5, 6};
        test2 *= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 3);
            REQUIRE(test2.get_y() == 8);
            REQUIRE(test2.get_z() == 15);
            REQUIRE(test2.get_w() == 24);
        }
    }

    WHEN("A vec4 is divide-assigned to another vec4")
    {
        vec4 test1 = {1, 2, 3, 4};
        vec4 test2 = {3, 8, 15, 24};
        test2 /= test1;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 3);
            REQUIRE(test2.get_y() == 4);
            REQUIRE(test2.get_z() == 5);
            REQUIRE(test2.get_w() == 6);
        }
    }

    WHEN("A vec4's lengths are computed")
    {
        vec4 test = {3, 4, 5, 6};

        REQUIRE(test.length_squared() == component_type(86));
        REQUIRE(move::math::approx_equal(test.length(),
                                         component_type(std::sqrt(86))));
        REQUIRE(move::math::approx_equal(
            test.reciprocal_length(),
            component_type(1) / component_type(std::sqrt(86))));
    }

    WHEN("A vec4 is normalized")
    {
        vec4 test = {3, 4, 5, 6};
        test.normalize();

        THEN("The length is correct")
        {
            REQUIRE(move::math::safe_equal(test.length(), 1));
        }
    }

    WHEN("A vec4 is zeroed")
    {
        vec4 test = {3, 4, 5, 6};
        test.set_zero();

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 0);
            REQUIRE(test.get_y() == 0);
            REQUIRE(test.get_z() == 0);
            REQUIRE(test.get_w() == 0);
        }
    }

    WHEN("A vec4 is converted to a pointer")
    {
        vec4 test = {3, 4, 5, 6};

        THEN("The pointer is correct")
        {
            component_type* ptr = test.to_array();
            REQUIRE(ptr[0] == 3);
            REQUIRE(ptr[1] == 4);
            REQUIRE(ptr[2] == 5);
            REQUIRE(ptr[3] == 6);
        }
    }

    WHEN("A vec4 is converted to a const pointer")
    {
        const vec4 test = {3, 4, 5, 6};
        THEN("The pointer is correct")
        {
            const component_type* ptr = test.to_array();
            REQUIRE(ptr[0] == 3);
            REQUIRE(ptr[1] == 4);
            REQUIRE(ptr[2] == 5);
            REQUIRE(ptr[3] == 6);
        }
    }

    WHEN("A vec4 is compared to another vec4")
    {
        vec4 test1 = {3, 4, 5, 6};
        vec4 test2 = {3, 4, 5, 6};
        vec4 test3 = {4, 5, 6, 7};

        THEN("The comparison is correct")
        {
            REQUIRE(test1 == test2);
            REQUIRE(test1 != test3);
        }
    }

    WHEN("A vec4 is filled with a value")
    {
        vec4 test = {3, 4, 5, 6};
        test.fill(7);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 7);
            REQUIRE(test.get_y() == 7);
            REQUIRE(test.get_z() == 7);
            REQUIRE(test.get_w() == 7);
        }
    }

    WHEN("A vec4 is set with values")
    {
        vec4 test = {3, 4, 5, 6};
        test.set(7, 8, 9, 10);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 7);
            REQUIRE(test.get_y() == 8);
            REQUIRE(test.get_z() == 9);
            REQUIRE(test.get_w() == 10);
        }
    }

    WHEN("A vec4 has individual components set")
    {
        vec4 test = vec4::zero();

        REQUIRE(test.get_x() == 0);
        REQUIRE(test.get_y() == 0);
        REQUIRE(test.get_z() == 0);
        REQUIRE(test.get_w() == 0);

        test.set_x(1);
        test.set_y(2);
        test.set_z(3);
        test.set_w(4);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
            REQUIRE(test.get_w() == 4);
        }
    }

    WHEN("Computing the distance and squared distance between two points")
    {
        vec4 test1 = {1, 2, 3, 4};
        vec4 test2 = {4, 3, 2, 1};

        THEN("The distances are correct")
        {
            REQUIRE(test1.distance_squared(test2) == Catch::Approx(20));
            REQUIRE(test1.distance(test2) ==
                    Catch::Approx(component_type(std::sqrt(20))));
        }
    }

    WHEN("Computing the dot product between two vectors")
    {
        vec4 test1 = {1, 2, 3, 4};
        vec4 test2 = {4, 3, 2, 1};

        THEN("The dot product is correct")
        {
            REQUIRE(vec4::dot(test1, test2) == 20);
        }
    }

    if constexpr (std::is_floating_point_v<component_type>)
    {
        WHEN("Computing the cross product between two vectors")
        {
            vec4 test1 = {1, 2, 3, 4};
            vec4 test2 = {4, 3, 2, 1};
            vec4 test3 = {1, 2, 3, 4};
            vec4 cross = vec4::cross(test1, test2, test3);

            THEN("The cross product is correct")
            {
                // TODO: Check cross product
            }
        }
    }

    WHEN("Computing the angle between two vectors")
    {
        REQUIRE(vec4::angle_between_vectors({1, 0, 0, 0}, {0, 1, 0, 0}) ==
                Catch::Approx(component_type(1.5707964)));

        REQUIRE(vec4::angle_between_vectors({1, 0, 1, 0}, {0, 1, 0, 0}) ==
                Catch::Approx(component_type(1.5707964)));
    }

    // TODO: Test reflect
    // TODO: Test refract

    WHEN("Lerping between vectors")
    {
        vec4 test1 = {0, 0, 0, 0};
        vec4 test2 = {5, 5, 5, 5};

        // lerp unclamped (scalar factor)
        REQUIRE(vec4::lerp_unclamped(test1, test2, component_type(0.5)) ==
                vec4::filled(component_type(2.5)));
        REQUIRE(vec4::lerp_unclamped(test1, test2, 2) == vec4(10, 10, 10, 10));

        // lerp unclamped (vector factor)
        REQUIRE(vec4::lerp_unclamped(test1, test2, {0, 1, 0, 1}) ==
                vec4(0, 5, 0, 5));
        REQUIRE(vec4::lerp_unclamped(test1, test2, {1, 2, 1, 0}) ==
                vec4(5, 10, 5, 0));

        // lerp (scalar factor)
        REQUIRE(vec4::lerp(test1, test2, component_type(0.5)) ==
                vec4::filled(component_type(2.5)));
        REQUIRE(vec4::lerp(test1, test2, 2) == vec4(5, 5, 5, 5));

        // lerp (vector factor)
        REQUIRE(vec4::lerp(test1, test2, {0, 1, 0, 1}) == vec4(0, 5, 0, 5));
        REQUIRE(vec4::lerp(test1, test2, {1, 2, 1, 0}) == vec4(5, 5, 5, 0));
    }

    WHEN("Computing the minimum of two vectors")
    {
        vec4 test1 = {1, 2, 3, 4};
        vec4 test2 = {4, 3, 2, 1};

        THEN("The minimum is correct")
        {
            REQUIRE(vec4::min(test1, test2) == vec4(1, 2, 2, 1));
        }
    }

    WHEN("Computing the maximum of two vectors")
    {
        vec4 test1 = {1, 2, 3, 4};
        vec4 test2 = {4, 3, 2, 1};

        THEN("The maximum is correct")
        {
            REQUIRE(vec4::max(test1, test2) == vec4(4, 3, 3, 4));
        }
    }

    WHEN("Clamping a vector with a scalar range")
    {
        vec4 test = {1, 2, 3, 4};

        THEN("The clamped vector is correct")
        {
            REQUIRE(vec4::clamp(test, 2, 3) == vec4(2, 2, 3, 3));
        }
    }

    WHEN("Clamping the vector with a vector range")
    {
        vec4 test = {1, 2, 3, 4};

        THEN("The clamped vector is correct")
        {
            REQUIRE(vec4::clamp(test, {2, 3, 4, 5}, {3, 4, 5, 6}) ==
                    vec4(2, 3, 4, 5));
        }
    }
}

SCENARIO("Vec4 tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    // Scalar tests
    test_vec4<vec4<float, Accel::Scalar>>();
    test_vec4<vec4<double, Accel::Scalar>>();

    test_vec4<vec4<int8_t, Accel::Scalar>>();
    test_vec4<vec4<int16_t, Accel::Scalar>>();
    test_vec4<vec4<int32_t, Accel::Scalar>>();
    test_vec4<vec4<int64_t, Accel::Scalar>>();

    test_vec4<vec4<uint8_t, Accel::Scalar>>();
    test_vec4<vec4<uint16_t, Accel::Scalar>>();
    test_vec4<vec4<uint32_t, Accel::Scalar>>();
    test_vec4<vec4<uint64_t, Accel::Scalar>>();

    // SIMD tests
    test_vec4<vec4<float, Accel::SIMD>>();
    test_vec4<vec4<double, Accel::SIMD>>();

    test_vec4<vec4<int8_t, Accel::SIMD>>();
    test_vec4<vec4<int16_t, Accel::SIMD>>();
    test_vec4<vec4<int32_t, Accel::SIMD>>();
    test_vec4<vec4<int64_t, Accel::SIMD>>();

    test_vec4<vec4<uint8_t, Accel::SIMD>>();
    test_vec4<vec4<uint16_t, Accel::SIMD>>();
    test_vec4<vec4<uint32_t, Accel::SIMD>>();
    test_vec4<vec4<uint64_t, Accel::SIMD>>();
}