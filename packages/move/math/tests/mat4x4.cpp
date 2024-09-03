#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>

#include <movemm/memory-allocator.h>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/mat4x4.hpp>
#include <move/meta/type_utils.hpp>
#include <move/string.hpp>

#include "mm_test_common.hpp"

template <typename mat4>
inline void test_mat4()
{
    using component_type = mat4::component_type;
    using vec2 = mat4::vec2_t;
    using vec3 = mat4::vec3_t;
    using vec4 = mat4::vec4_t;
    static constexpr auto acceleration = mat4::acceleration;

    INFO("Testing mat4 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec2: " << move::meta::type_name<vec2>());
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    INFO("\tmat4: " << move::meta::type_name<mat4>());

    REQUIRE(mat4::zero() ==
            mat4(0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0));
    REQUIRE(mat4::one() ==
            mat4(1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1));

    REQUIRE(mat4::identity() ==
            mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1));

    REQUIRE(mat4::filled(2) ==
            mat4(2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2, 2));

    REQUIRE(mat4::infinity() ==
            mat4::filled(std::numeric_limits<component_type>::infinity()));

    REQUIRE(mat4::negative_infinity() ==
            mat4::filled(-std::numeric_limits<component_type>::infinity()));

    // Identity testing
    {
        mat4 identity;
        WHEN("The inverse of an identity matrix is calculated")
        {
            mat4 result = identity.inverse();
            THEN("The result is correct")
            {
                REQUIRE(result == mat4::identity());
            }
        }

        WHEN("The transpose of an identity matrix is calculated")
        {
            mat4 result = identity.transposed();
            THEN("The result is correct")
            {
                REQUIRE(result == mat4::identity());
            }
        }

        WHEN("The determinant of an identity matrix is calculated")
        {
            component_type result = identity.determinant();
            THEN("The result is correct")
            {
                REQUIRE(result == 1);
            }
        }
    }

    // Transform matrix testing
    {
        mat4 translation = mat4::translation(vec3(1, 2, 3));

        THEN("The matrix is correct")
        {
            REQUIRE(translation ==
                    mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 1, 2, 3, 1));
        }

        THEN("The inverse is correct")
        {
            REQUIRE(translation.inverse() ==
                    mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, -1, -2, -3, 1));
        }

        THEN("The transpose is correct")
        {
            REQUIRE(translation.transposed() ==
                    mat4(1, 0, 0, 1, 0, 1, 0, 2, 0, 0, 1, 3, 0, 0, 0, 1));
        }

        THEN("The determinant is correct")
        {
            REQUIRE(translation.determinant() == 1);
        }

        THEN(
            "Multiplying a vector by the matrix results in the correct "
            "transformation")
        {
            vec4 test = {1, 2, 3, 1};
            vec4 result = translation * test;
            REQUIRE(result == vec4(2, 4, 6, 1));
        }

        THEN(
            "Multiplying the vector by the inverse results in the correct "
            "transformation")
        {
            vec4 test = {2, 4, 6, 1};
            vec4 result = translation.inverse() * test;
            REQUIRE(result == vec4(1, 2, 3, 1));
        }
    }

    // Rotation matrix testing
    {
        mat4 rotation = mat4::angle_axis(
            vec3::up(), move::math::deg2rad<component_type>(90));

        THEN("The matrix is correct")
        {
            auto expected =
                mat4(0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 0, 0, 0, 1);
            INFO(rotation << "Expected value" << expected);

            REQUIRE(move::math::approx_equal(rotation, expected,
                                             component_type(0.001)));
        }

        THEN("The inverse is correct")
        {
            auto inv = rotation.inverse();
            auto expected = mat4::angle_axis(
                vec3::up(), move::math::deg2rad<component_type>(-90));
            INFO(inv << " vs " << expected);
            REQUIRE(
                move::math::approx_equal(inv, expected, component_type(0.001)));
        }

        THEN("The determinant is correct")
        {
            REQUIRE(rotation.determinant() == Catch::Approx(1));
        }

        THEN(
            "Multiplying a vector by the matrix results in the correct "
            "transformation")
        {
            vec4 test = {1, 0, 0, 1};
            vec4 result = rotation * test;
            vec4 expected = {0, 0, -1, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }

        THEN(
            "Multiplying the vector by the opposite transform results in the"
            "correct transformation")
        {
            auto opposite = mat4::angle_axis(
                vec3::up(), move::math::deg2rad<component_type>(-90));

            vec4 test = {1, 0, 0, 1};
            vec4 result = opposite * test;
            vec4 expected = {0, 0, 1, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
            REQUIRE(move::math::approx_equal(rotation.inverse(), opposite,
                                             component_type(0.001)));
        }

        THEN(
            "Multiplying the vector by the inverse results in the correct "
            "transformation")
        {
            vec4 test = {1, 0, 0, 1};
            vec4 result = rotation.inverse() * test;
            vec4 expected = {0, 0, 1, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }
    }
}

template <typename mat4>
inline void benchmark_mat4()
{
    using component_type = mat4::component_type;
    using vec2 = mat4::vec2_t;
    using vec3 = mat4::vec3_t;
    using vec4 = mat4::vec4_t;
    static constexpr auto acceleration = mat4::acceleration;
    move::string_view typeName = move::meta::type_name<mat4>();

    mat4 identity;
    BENCHMARK(alloc_appended_name(typeName, ": Identity construction"))
    {
        return mat4();
    };

    BENCHMARK(alloc_appended_name(typeName, ": Inverse"))
    {
        return identity.inverse();
    };
}

REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(test_mat4, move::math::mat4x4);
REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(benchmark_mat4, move::math::mat4x4);

SCENARIO("Mat4 full tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    test_mat4_multi<float, double>();
}

SCENARIO("Mat4 benchmarks")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    benchmark_mat4_multi<float, double>();
}