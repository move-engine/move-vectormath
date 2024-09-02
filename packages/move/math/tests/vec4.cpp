#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <move/math/common.hpp>
#include <move/math/vec4.hpp>

template <typename Vec4T>
inline void test_vec4()
{
    using component_type = typename Vec4T::component_type;
    WHEN("A default vec4 is created and a vec4 is created with values")
    {
        Vec4T test;

        THEN("The values are correct")
        {
            REQUIRE(test.x == 0);
            REQUIRE(test.y == 0);
            REQUIRE(test.z == 0);
            REQUIRE(test.w == 0);
        }
    }

    WHEN("A vec4 is created with 3 values")
    {
        Vec4T test(1, 2, 3);

        THEN("The values are correct")
        {
            REQUIRE(test.x == 1);
            REQUIRE(test.y == 2);
            REQUIRE(test.z == 3);
            REQUIRE(test.w == 0);
        }
    }

    WHEN("A vec4 is created with 4 values")
    {
        Vec4T test(1, 2, 3, 4);

        THEN("The values are correct")
        {
            REQUIRE(test.x == 1);
            REQUIRE(test.y == 2);
            REQUIRE(test.z == 3);
            REQUIRE(test.w == 4);
        }
    }

    WHEN("A vec4 is created with values in {} notation")
    {
        Vec4T test = {1, 2, 3, 4};

        THEN("The values are correct")
        {
            REQUIRE(test.x == 1);
            REQUIRE(test.y == 2);
            REQUIRE(test.z == 3);
            REQUIRE(test.w == 4);
        }
    }

    WHEN("Two vec4s are added")
    {
        Vec4T added = Vec4T(1, 2, 3, 4) + Vec4T(3, 4, 5, 6);

        THEN("The result is correct")
        {
            REQUIRE(added.x == 4);
            REQUIRE(added.y == 6);
            REQUIRE(added.z == 8);
            REQUIRE(added.w == 10);
        }
    }

    WHEN("Two vec4s are subtracted")
    {
        Vec4T subtracted = Vec4T(2, 4, 6, 8) - Vec4T(1, 2, 3, 4);

        THEN("The result is correct")
        {
            REQUIRE(subtracted.x == 1);
            REQUIRE(subtracted.y == 2);
            REQUIRE(subtracted.z == 3);
            REQUIRE(subtracted.w == 4);
        }
    }

    WHEN("Two vec4s are multiplied")
    {
        Vec4T multiplied = Vec4T(2, 4, 6, 8) * Vec4T(3, 5, 7, 9);

        THEN("The result is correct")
        {
            REQUIRE(multiplied.x == 6);
            REQUIRE(multiplied.y == 20);
            REQUIRE(multiplied.z == 42);
            REQUIRE(multiplied.w == 72);
        }
    }

    WHEN("Two vec4s are divided")
    {
        Vec4T divided = Vec4T(6, 20, 42, 72) / Vec4T(2, 4, 6, 8);

        THEN("The result is correct")
        {
            REQUIRE(divided.x == 3);
            REQUIRE(divided.y == 5);
            REQUIRE(divided.z == 7);
            REQUIRE(divided.w == 9);
        }
    }

    WHEN("A vec4 is assigned to another vec4")
    {
        Vec4T test = {1, 2, 3, 4};
        Vec4T test2;
        test2 = test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 1);
            REQUIRE(test2.y == 2);
            REQUIRE(test2.z == 3);
            REQUIRE(test2.w == 4);
        }
    }

    WHEN("A vec4 is add-assigned to another vec4")
    {
        Vec4T test = {1, 2, 3, 4};
        Vec4T test2 = {3, 4, 5, 6};
        test2 += test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 4);
            REQUIRE(test2.y == 6);
            REQUIRE(test2.z == 8);
            REQUIRE(test2.w == 10);
        }
    }

    WHEN("A vec4 is subtract-assigned to another vec4")
    {
        Vec4T test = {1, 2, 3, 4};
        Vec4T test2 = {3, 4, 5, 6};
        test2 -= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 2);
            REQUIRE(test2.y == 2);
            REQUIRE(test2.z == 2);
            REQUIRE(test2.w == 2);
        }
    }

    WHEN("A vec4 is multiply-assigned to another vec4")
    {
        Vec4T test = {1, 2, 3, 4};
        Vec4T test2 = {3, 4, 5, 6};
        test2 *= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 3);
            REQUIRE(test2.y == 8);
            REQUIRE(test2.z == 15);
            REQUIRE(test2.w == 24);
        }
    }

    WHEN("A vec4 is divide-assigned to another vec4")
    {
        Vec4T test1 = {1, 2, 3, 4};
        Vec4T test2 = {3, 8, 15, 24};
        test2 /= test1;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 3);
            REQUIRE(test2.y == 4);
            REQUIRE(test2.z == 5);
            REQUIRE(test2.w == 6);
        }
    }

    WHEN("A vec4's length is computed")
    {
        Vec4T test = {3, 4, 5, 6};

        THEN("The length is correct")
        {
            REQUIRE(test.length() == component_type(9.273618495495704));
        }
    }

    WHEN("A vec4 is normalized")
    {
        Vec4T test = {3, 4, 5, 6};
        test.normalize();

        THEN("The length is correct")
        {
            REQUIRE(move::math::approx_equal(test.length(), 1));
        }
    }

    WHEN("A vec4 is zeroed")
    {
        Vec4T test = {3, 4, 5, 6};
        test.zero();

        THEN("The values are correct")
        {
            REQUIRE(test.x == 0);
            REQUIRE(test.y == 0);
            REQUIRE(test.z == 0);
            REQUIRE(test.w == 0);
        }
    }

    WHEN("A vec4 is converted to a pointer")
    {
        Vec4T test = {3, 4, 5, 6};

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
        const Vec4T test = {3, 4, 5, 6};

        THEN("The pointer is correct")
        {
            const component_type* ptr = test.to_array();
            REQUIRE(ptr[0] == 3);
            REQUIRE(ptr[1] == 4);
            REQUIRE(ptr[2] == 5);
            REQUIRE(ptr[3] == 6);
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

    // Note: For Vec4, these are currently identical
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