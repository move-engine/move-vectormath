#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/vec3.hpp>
#include <move/meta/type_utils.hpp>
#include "catch2/catch_approx.hpp"

template <typename vec3>
inline void test_vec3()
{
    using component_type = vec3::component_type;
    using vec2 = vec3::vec2_t;
    static constexpr auto acceleration = vec3::acceleration;

    INFO("Testing vec3 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec2: " << move::meta::type_name<vec2>());
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    REQUIRE(vec3::zero() == vec3(0, 0, 0));
    REQUIRE(vec3::one() == vec3(1, 1, 1));

    REQUIRE(vec3(1, 1, 1) == vec3(1, 1, 1));
    REQUIRE(vec3(0, 0, 0) == vec3(0, 0, 0));
    REQUIRE(vec3::filled(1) == vec3(1, 1, 1));

    REQUIRE(vec3::infinity() ==
            vec3(std::numeric_limits<component_type>::infinity(),
                 std::numeric_limits<component_type>::infinity(),
                 std::numeric_limits<component_type>::infinity()));

    REQUIRE(vec3::negative_infinity() ==
            vec3(-std::numeric_limits<component_type>::infinity(),
                 -std::numeric_limits<component_type>::infinity(),
                 -std::numeric_limits<component_type>::infinity()));

    REQUIRE(vec3(vec2(1, 2)) == vec3(1, 2, 0));

    WHEN("A default vec3 is created")
    {
        vec3 test;

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 0);
            REQUIRE(test.get_y() == 0);
            REQUIRE(test.get_z() == 0);
        }
    }

    WHEN("A vec3 is created with 3 values")
    {
        vec3 test(1, 2, 3);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
        }
    }

    WHEN("A vec3 is created with values in {} notation")
    {
        vec3 test = {1, 2, 3};

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
        }
    }

    WHEN("A vec3 is created with 4 values and swizzled")
    {
        vec3 test(1, 2, 3);

        REQUIRE(test.xy() == vec2(1, 2));
        REQUIRE(test.xz() == vec2(1, 3));

        REQUIRE(test.yx() == vec2(2, 1));
        REQUIRE(test.yz() == vec2(2, 3));

        REQUIRE(test.zx() == vec2(3, 1));
        REQUIRE(test.zy() == vec2(3, 2));
    }

    WHEN("Two vec3s are added")
    {
        vec3 added = vec3(1, 2, 3) + vec3(3, 4, 5);

        THEN("The result is correct")
        {
            REQUIRE(added.get_x() == 4);
            REQUIRE(added.get_y() == 6);
            REQUIRE(added.get_z() == 8);
        }
    }

    WHEN("Two vec3s are subtracted")
    {
        vec3 subtracted = vec3(2, 4, 6) - vec3(1, 2, 3);

        THEN("The result is correct")
        {
            REQUIRE(subtracted.get_x() == 1);
            REQUIRE(subtracted.get_y() == 2);
            REQUIRE(subtracted.get_z() == 3);
        }
    }

    WHEN("Two vec3s are multiplied")
    {
        vec3 multiplied = vec3(2, 4, 6) * vec3(3, 5, 7);

        THEN("The result is correct")
        {
            REQUIRE(multiplied.get_x() == 6);
            REQUIRE(multiplied.get_y() == 20);
            REQUIRE(multiplied.get_z() == 42);
        }
    }

    WHEN("Two vec3s are divided")
    {
        vec3 divided = vec3(6, 20, 42) / vec3(2, 4, 6);

        THEN("The result is correct")
        {
            REQUIRE(divided.get_x() == 3);
            REQUIRE(divided.get_y() == 5);
            REQUIRE(divided.get_z() == 7);
        }
    }

    WHEN("A vec3 is assigned to another vec3")
    {
        vec3 test = {1, 2, 3};
        vec3 test2;
        test2 = test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 1);
            REQUIRE(test2.get_y() == 2);
            REQUIRE(test2.get_z() == 3);
        }
    }

    WHEN("A vec3 is add-assigned to another vec3")
    {
        vec3 test = {1, 2, 3};
        vec3 test2 = {3, 4, 5};
        test2 += test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 4);
            REQUIRE(test2.get_y() == 6);
            REQUIRE(test2.get_z() == 8);
        }
    }

    WHEN("A vec3 is subtract-assigned to another vec3")
    {
        vec3 test = {1, 2, 3};
        vec3 test2 = {3, 4, 5};
        test2 -= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 2);
            REQUIRE(test2.get_y() == 2);
            REQUIRE(test2.get_z() == 2);
        }
    }

    WHEN("A vec3 is multiply-assigned to another vec3")
    {
        vec3 test = {1, 2, 3};
        vec3 test2 = {3, 4, 5};
        test2 *= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 3);
            REQUIRE(test2.get_y() == 8);
            REQUIRE(test2.get_z() == 15);
        }
    }

    WHEN("A vec3 is divide-assigned to another vec3")
    {
        vec3 test1 = {1, 2, 3};
        vec3 test2 = {3, 8, 15};
        test2 /= test1;

        THEN("The values are correct")
        {
            REQUIRE(test2.get_x() == 3);
            REQUIRE(test2.get_y() == 4);
            REQUIRE(test2.get_z() == 5);
        }
    }

    WHEN("A vec3's lengths are computed")
    {
        vec3 test = {3, 4, 5};

        REQUIRE(test.length_squared() == component_type(50));
        REQUIRE(move::math::approx_equal(test.length(),
                                         component_type(std::sqrt(50))));
        REQUIRE(move::math::approx_equal(
            test.reciprocal_length(),
            component_type(1) / component_type(std::sqrt(50))));
    }

    WHEN("A vec3 is normalized")
    {
        vec3 test = {3, 4, 5};
        test.normalize();

        THEN("The length is correct")
        {
            REQUIRE(move::math::safe_equal(test.length(), 1));
        }
    }

    WHEN("A vec3 is zeroed")
    {
        vec3 test = {3, 4, 5};
        test.set_zero();

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 0);
            REQUIRE(test.get_y() == 0);
            REQUIRE(test.get_z() == 0);
        }
    }

    WHEN("A vec3 is converted to a pointer")
    {
        vec3 test = {3, 4, 5};

        THEN("The pointer is correct")
        {
            component_type* ptr = test.to_array();
            REQUIRE(ptr[0] == 3);
            REQUIRE(ptr[1] == 4);
            REQUIRE(ptr[2] == 5);
        }
    }

    WHEN("A vec3 is converted to a const pointer")
    {
        const vec3 test = {3, 4, 5};
        THEN("The pointer is correct")
        {
            const component_type* ptr = test.to_array();
            REQUIRE(ptr[0] == 3);
            REQUIRE(ptr[1] == 4);
            REQUIRE(ptr[2] == 5);
        }
    }

    WHEN("A vec3 is compared to another vec3")
    {
        vec3 test1 = {3, 4, 5};
        vec3 test2 = {3, 4, 5};
        vec3 test3 = {4, 5, 6};

        THEN("The comparison is correct")
        {
            REQUIRE(test1 == test2);
            REQUIRE(test1 != test3);
        }
    }

    WHEN("A vec3 is filled with a value")
    {
        vec3 test = {3, 4, 5};
        test.fill(7);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 7);
            REQUIRE(test.get_y() == 7);
            REQUIRE(test.get_z() == 7);
        }
    }

    WHEN("A vec3 is set with values")
    {
        vec3 test = {3, 4, 5};
        test.set(7, 8, 9);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 7);
            REQUIRE(test.get_y() == 8);
            REQUIRE(test.get_z() == 9);
        }
    }

    WHEN("A vec3 has individual components set")
    {
        vec3 test = vec3::zero();

        REQUIRE(test.get_x() == 0);
        REQUIRE(test.get_y() == 0);
        REQUIRE(test.get_z() == 0);

        test.set_x(1);
        test.set_y(2);
        test.set_z(3);

        THEN("The values are correct")
        {
            REQUIRE(test.get_x() == 1);
            REQUIRE(test.get_y() == 2);
            REQUIRE(test.get_z() == 3);
        }
    }

    WHEN("Computing the distance and squared distance between two points")
    {
        vec3 test1 = {1, 2, 3};
        vec3 test2 = {4, 3, 2};

        THEN("The distances are correct")
        {
            REQUIRE(test1.distance_squared(test2) == Catch::Approx(11));
            REQUIRE(test1.distance(test2) ==
                    Catch::Approx(component_type(std::sqrt(11))));
        }
    }

    WHEN("Computing the dot product between two vectors")
    {
        vec3 test1 = {1, 2, 3};
        vec3 test2 = {4, 3, 2};

        THEN("The dot product is correct")
        {
            REQUIRE(vec3::dot(test1, test2) == 16);
        }
    }

    if constexpr (std::is_floating_point_v<component_type>)
    {
        WHEN("Computing the cross product between two vectors")
        {
            vec3 test1 = {1, 2, 3};
            vec3 test2 = {4, 3, 2};
            vec3 test3 = {1, 2, 3};
            vec3 cross = vec3::cross(test1, test2);

            THEN("The cross product is correct")
            {
                // TODO: Check cross product
            }
        }
    }

    WHEN("Computing the angle between two vectors")
    {
        REQUIRE(vec3::angle_between_vectors({1, 0, 0}, {0, 1, 0}) ==
                Catch::Approx(component_type(1.5707964)));

        REQUIRE(vec3::angle_between_vectors({1, 0, 1}, {0, 1, 0}) ==
                Catch::Approx(component_type(1.5707964)));
    }

    // TODO: Test reflect
    // TODO: Test refract

    WHEN("Lerping between vectors")
    {
        vec3 test1 = {0, 0, 0};
        vec3 test2 = {5, 5, 5};

        // lerp unclamped (scalar factor)
        REQUIRE(vec3::lerp_unclamped(test1, test2, component_type(0.5)) ==
                vec3::filled(component_type(2.5)));
        REQUIRE(vec3::lerp_unclamped(test1, test2, 2) == vec3(10, 10, 10));

        // lerp unclamped (vector factor)
        REQUIRE(vec3::lerp_unclamped(test1, test2, {0, 1, 0}) == vec3(0, 5, 0));
        REQUIRE(vec3::lerp_unclamped(test1, test2, {1, 2, 1}) ==
                vec3(5, 10, 5));

        // lerp (scalar factor)
        REQUIRE(vec3::lerp(test1, test2, component_type(0.5)) ==
                vec3::filled(component_type(2.5)));
        REQUIRE(vec3::lerp(test1, test2, 2) == vec3(5, 5, 5));

        // lerp (vector factor)
        REQUIRE(vec3::lerp(test1, test2, {0, 1, 0}) == vec3(0, 5, 0));
        REQUIRE(vec3::lerp(test1, test2, {1, 2, 1}) == vec3(5, 5, 5));
    }

    WHEN("Computing the minimum of two vectors")
    {
        vec3 test1 = {1, 2, 3};
        vec3 test2 = {4, 3, 2};

        THEN("The minimum is correct")
        {
            REQUIRE(vec3::min(test1, test2) == vec3(1, 2, 2));
        }
    }

    WHEN("Computing the maximum of two vectors")
    {
        vec3 test1 = {1, 2, 3};
        vec3 test2 = {4, 3, 2};

        THEN("The maximum is correct")
        {
            REQUIRE(vec3::max(test1, test2) == vec3(4, 3, 3));
        }
    }

    WHEN("Clamping a vector with a scalar range")
    {
        vec3 test = {1, 2, 3};

        THEN("The clamped vector is correct")
        {
            REQUIRE(vec3::clamp(test, 2, 3) == vec3(2, 2, 3));
        }
    }

    WHEN("Clamping the vector with a vector range")
    {
        vec3 test = {1, 2, 3};

        THEN("The clamped vector is correct")
        {
            REQUIRE(vec3::clamp(test, {2, 3, 4}, {3, 4, 5}) == vec3(2, 3, 4));
        }
    }
}

SCENARIO("Vec3 tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    // Scalar tests
    test_vec3<vec3<float, Accel::Scalar>>();
    test_vec3<vec3<double, Accel::Scalar>>();

    test_vec3<vec3<int8_t, Accel::Scalar>>();
    test_vec3<vec3<int16_t, Accel::Scalar>>();
    test_vec3<vec3<int32_t, Accel::Scalar>>();
    test_vec3<vec3<int64_t, Accel::Scalar>>();

    test_vec3<vec3<uint8_t, Accel::Scalar>>();
    test_vec3<vec3<uint16_t, Accel::Scalar>>();
    test_vec3<vec3<uint32_t, Accel::Scalar>>();
    test_vec3<vec3<uint64_t, Accel::Scalar>>();

    // SIMD tests
    test_vec3<vec3<float, Accel::SIMD>>();
    test_vec3<vec3<double, Accel::SIMD>>();

    test_vec3<vec3<int8_t, Accel::SIMD>>();
    test_vec3<vec3<int16_t, Accel::SIMD>>();
    test_vec3<vec3<int32_t, Accel::SIMD>>();
    test_vec3<vec3<int64_t, Accel::SIMD>>();

    test_vec3<vec3<uint8_t, Accel::SIMD>>();
    test_vec3<vec3<uint16_t, Accel::SIMD>>();
    test_vec3<vec3<uint32_t, Accel::SIMD>>();
    test_vec3<vec3<uint64_t, Accel::SIMD>>();
}