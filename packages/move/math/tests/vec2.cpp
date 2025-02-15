#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <move/math/common.hpp>
#include <move/math/vec2.hpp>

template <typename vec2>
inline void test_vec2()
{
    REQUIRE(vec2::element_count == 2);
    REQUIRE(move::math::traits::is_vector_type_v<vec2>);
    REQUIRE(!move::math::traits::is_matrix_type_v<vec2>);
    REQUIRE(move::math::traits::component_count_v<vec2> == 2);
    REQUIRE(std::is_same_v<move::math::traits::component_type_t<vec2>,
                           typename vec2::component_type>);

    WHEN("A default vec2 is created and a vec2 is created with values")
    {
        vec2 test;

        THEN("The values are correct")
        {
            REQUIRE(test.x == 0);
            REQUIRE(test.y == 0);
        }
    }

    WHEN("A vec2 is created with values")
    {
        vec2 test(1, 2);

        THEN("The values are correct")
        {
            REQUIRE(test.x == 1);
            REQUIRE(test.y == 2);
        }
    }

    WHEN("A vec2 is created with values in {} notation")
    {
        vec2 test = {1, 2};

        THEN("The values are correct")
        {
            REQUIRE(test.x == 1);
            REQUIRE(test.y == 2);
        }
    }

    WHEN("Two vec2s are added")
    {
        vec2 added = vec2(1, 2) + vec2(3, 4);

        THEN("The result is correct")
        {
            REQUIRE(added.x == 4);
            REQUIRE(added.y == 6);
        }
    }

    WHEN("Two vec2s are subtracted")
    {
        vec2 subtracted = vec2(2, 4) - vec2(1, 2);

        THEN("The result is correct")
        {
            REQUIRE(subtracted.x == 1);
            REQUIRE(subtracted.y == 2);
        }
    }

    WHEN("Two vec2s are multiplied")
    {
        vec2 multiplied = vec2(2, 4) * vec2(3, 5);

        THEN("The result is correct")
        {
            REQUIRE(multiplied.x == 6);
            REQUIRE(multiplied.y == 20);
        }
    }

    WHEN("Two vec2s are divided")
    {
        vec2 divided = vec2(6, 20) / vec2(2, 4);

        THEN("The result is correct")
        {
            REQUIRE(divided.x == 3);
            REQUIRE(divided.y == 5);
        }
    }

    WHEN("A vec2 is assigned to another vec2")
    {
        vec2 test = {1, 2};
        vec2 test2 = test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 1);
            REQUIRE(test2.y == 2);
        }
    }

    WHEN("A vec2 is assigned to another vec2")
    {
        vec2 test = {1, 2};
        vec2 test2;
        test2 = test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 1);
            REQUIRE(test2.y == 2);
        }
    }

    WHEN("A vec2 is add-assigned to another vec2")
    {
        vec2 test = {1, 2};
        vec2 test2 = {3, 4};
        test2 += test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 4);
            REQUIRE(test2.y == 6);
        }
    }

    WHEN("A vec2 is subtract-assigned to another vec2")
    {
        vec2 test = {1, 2};
        vec2 test2 = {3, 4};
        test2 -= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 2);
            REQUIRE(test2.y == 2);
        }
    }

    WHEN("A vec2 is multiply-assigned to another vec2")
    {
        vec2 test = {2, 4};
        vec2 test2 = {3, 5};
        test2 *= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 6);
            REQUIRE(test2.y == 20);
        }
    }

    WHEN("A vec2 is divide-assigned to another vec2")
    {
        vec2 test = {2, 4};
        vec2 test2 = {6, 20};
        test2 /= test;

        THEN("The values are correct")
        {
            REQUIRE(test2.x == 3);
            REQUIRE(test2.y == 5);
        }
    }

    WHEN("A vec2's length is computed")
    {
        vec2 test = {3, 4};

        THEN("The values are correct")
        {
            REQUIRE(test.length() == 5);
            REQUIRE(test.length_squared() == 25);
        }
    }
}

SCENARIO("Vec2 tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    // Scalar tests
    test_vec2<vec2<float, Accel::Scalar>>();
    test_vec2<vec2<double, Accel::Scalar>>();

    test_vec2<vec2<int8_t, Accel::Scalar>>();
    test_vec2<vec2<int16_t, Accel::Scalar>>();
    test_vec2<vec2<int32_t, Accel::Scalar>>();
    test_vec2<vec2<int64_t, Accel::Scalar>>();

    test_vec2<vec2<uint8_t, Accel::Scalar>>();
    test_vec2<vec2<uint16_t, Accel::Scalar>>();
    test_vec2<vec2<uint32_t, Accel::Scalar>>();
    test_vec2<vec2<uint64_t, Accel::Scalar>>();

    // Note: For Vec2, these are currently identical
    // SIMD tests
    test_vec2<vec2<float, Accel::RTM>>();
    test_vec2<vec2<double, Accel::RTM>>();

    test_vec2<vec2<int8_t, Accel::RTM>>();
    test_vec2<vec2<int16_t, Accel::RTM>>();
    test_vec2<vec2<int32_t, Accel::RTM>>();
    test_vec2<vec2<int64_t, Accel::RTM>>();

    test_vec2<vec2<uint8_t, Accel::RTM>>();
    test_vec2<vec2<uint16_t, Accel::RTM>>();
    test_vec2<vec2<uint32_t, Accel::RTM>>();
    test_vec2<vec2<uint64_t, Accel::RTM>>();
}