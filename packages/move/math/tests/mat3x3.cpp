#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>

#include <movemm/memory-allocator.h>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/mat3x3.hpp>
#include <move/math/mat3x4.hpp>

#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif
#include <move/string.hpp>

#include "mm_test_common.hpp"

template <typename mat3>
inline void test_mat3()
{
    using component_type = mat3::component_type;
    using vec3 = mat3::vec3_t;
    using vec4 = mat3::vec4_t;
    using quat = mat3::quat_t;
    static constexpr auto acceleration = mat3::acceleration;

    INFO("Testing mat3 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    INFO("\tmat3: " << move::meta::type_name<mat3>());

    REQUIRE(mat3::zero() == mat3(0, 0, 0, 0, 0, 0, 0, 0, 0));
    REQUIRE(mat3::one() == mat3(1, 1, 1, 1, 1, 1, 1, 1, 1));

    REQUIRE(mat3::identity() == mat3(1, 0, 0, 0, 1, 0, 0, 0, 1));

    REQUIRE(mat3::filled(2) == mat3(2, 2, 2, 2, 2, 2, 2, 2, 2));

    REQUIRE(mat3::infinity() ==
            mat3::filled(std::numeric_limits<component_type>::infinity()));

    REQUIRE(mat3::negative_infinity() ==
            mat3::filled(-std::numeric_limits<component_type>::infinity()));

    // Identity testing
    {
        mat3 identity;
        WHEN("The inverse of an identity matrix is calculated")
        {
            mat3 result = identity.inverse();
            THEN("The result is correct")
            {
                REQUIRE(move::math::approx_equal(result, mat3::identity(),
                                                 component_type(0.001)));
            }
        }

        WHEN("The transpose of an identity matrix is calculated")
        {
            mat3 result = identity.transposed();
            THEN("The result is correct")
            {
                REQUIRE(move::math::approx_equal(result, mat3::identity(),
                                                 component_type(0.001)));
            }
        }

        WHEN("The determinant of an identity matrix is calculated")
        {
            component_type result = identity.determinant();
            THEN("The result is correct")
            {
                REQUIRE(move::math::approx_equal(result, component_type(1),
                                                 component_type(0.001)));
            }
        }
    }

    // Bulk ops crash testing
    {
        mat3 test1 = mat3::identity();
        mat3 test2 = mat3::angle_axis(vec3(1, 2, 3).normalized(), 90);
        mat3 test3 = mat3::angle_axis(vec3::up(),
                                      move::math::deg2rad<component_type>(65));
        mat3 test4 = mat3::angle_axis(vec3(6, 3, 4).normalized(), 90);
        mat3 test5 = mat3::angle_axis(vec3(22, -19, 12).normalized(), 90);
        mat3 test6 = mat3::scale(2, 2, 2);
        mat3 test7 = mat3::identity();
        mat3 test8 = mat3::angle_axis(vec3(32, 12, 4).normalized(), 90);
        mat3 test9 = mat3::angle_axis(vec3(-18, 4, -89).normalized(), 90);
        mat3 test10 = mat3::angle_axis(vec3::right(),
                                       move::math::deg2rad<component_type>(23));
        mat3 test11 = mat3::angle_axis(vec3(11, 333, 4).normalized(), 90);
        mat3 test12 = mat3::angle_axis(vec3(1, 2, 3).normalized(), 90);
        mat3 test13 = mat3::angle_axis(vec3::up(),
                                       move::math::deg2rad<component_type>(65));
        mat3 test14 = mat3::angle_axis(vec3(6, 3, 4).normalized(), 90);
        mat3 test15 = mat3::angle_axis(vec3(22, -19, 12).normalized(), 90);
        mat3 test16 = mat3::scale(2, 2, 2);

        REQUIRE_NOTHROW(test1 * test2);
        REQUIRE_NOTHROW(test1 * test2 * test3);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11 * test12);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11 * test12 * test13);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11 * test12 * test13 *
                        test14);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11 * test12 * test13 *
                        test14 * test15);
        REQUIRE_NOTHROW(test1 * test2 * test3 * test4 * test5 * test6 * test7 *
                        test8 * test9 * test10 * test11 * test12 * test13 *
                        test14 * test15 * test16);
    }

    // Rotation matrix testing
    {
        mat3 rotation = mat3::angle_axis(
            vec3::up(), move::math::deg2rad<component_type>(90));

        THEN("The matrix is correct")
        {
            auto expected = mat3(0, 0, -1, 0, 1, 0, 1, 0, 0);
            INFO(rotation << "Expected value" << expected);

            REQUIRE(move::math::approx_equal(rotation, expected,
                                             component_type(0.001)));
        }

        THEN("The inverse is correct")
        {
            auto inv = rotation.inverse();
            auto expected = mat3::angle_axis(
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
            vec3 test = {1, 0, 0};
            vec3 result = test * rotation;
            vec3 expected = {0, 0, -1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }

        THEN(
            "Multiplying the vector by the opposite transform results in the"
            "correct transformation")
        {
            auto opposite = mat3::angle_axis(
                vec3::up(), move::math::deg2rad<component_type>(-90));

            vec3 test = {1, 0, 0};
            vec3 result = test * opposite;
            vec3 expected = {0, 0, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
            REQUIRE(move::math::approx_equal(rotation.inverse(), opposite,
                                             component_type(0.001)));
        }

        THEN(
            "Multiplying the vector by the inverse results in the correct "
            "transformation")
        {
            vec3 test = {1, 0, 0};
            vec3 result = test * rotation.inverse();
            vec3 expected = {0, 0, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }
    }
}

template <typename mat3>
inline void benchmark_mat3()
{
    using component_type = mat3::component_type;
    using vec3 = mat3::vec3_t;
    using vec4 = mat3::vec4_t;
    using quat = mat3::quat_t;
    static constexpr auto acceleration = mat3::acceleration;
    move::string_view typeName = move::meta::type_name<mat3>();

    INFO("Benchmarking mat3 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    INFO("\tquat: " << move::meta::type_name<quat>());
    INFO("\tmat3: " << move::meta::type_name<mat3>());

    mat3 identity;
    BENCHMARK(alloc_appended_name(typeName, ": Identity construction"))
    {
        return mat3();
    };

    BENCHMARK(alloc_appended_name(typeName, ": Inverse"))
    {
        return identity.inverse();
    };

    vec3 translation = {1, 2, 3};
    quat rotation = quat::angle_axis(vec3::up(), move::math::deg2rad(90));
    vec3 scaleFac = {1, 1, 1};
    BENCHMARK(
        alloc_appended_name(typeName, ": Transform Construction (builtin)"))
    {
        return mat3::trs(translation, rotation, scaleFac);
    };

    BENCHMARK(
        alloc_appended_name(typeName, ": Transform Construction (manual)"))
    {
        vec3 translation = {1, 2, 3};
        quat rotation = quat::angle_axis(vec3::up(), move::math::deg2rad(90));
        vec3 scaleFac = {1, 1, 1};

        auto scale = mat3::scale(scaleFac);
        auto rot = mat3::rotation(rotation);
        auto tra = mat3::translation(translation);

        auto result1 = scale * rot;
        auto result2 = result1 * tra;

        return result2;
    };

    BENCHMARK(alloc_appended_name(typeName,
                                  ": Transform Construction (manual, inline)"))
    {
        return
            //
            mat3::scale(scaleFac) *
            //
            mat3::rotation(rotation) *
            //
            mat3::translation(translation);
    };

    // Bulk ops crash testing
    BENCHMARK(alloc_appended_name(typeName, ": Bulk ops"))
    {
        mat3 test0 = mat3::scale(2, 2, 2);
        mat3 test1 = mat3::identity();
        mat3 test2 = mat3::translation(vec3(1, 2, 3));
        mat3 test3 = mat3::angle_axis(vec3::up(),
                                      move::math::deg2rad<component_type>(65));
        mat3 test4 = mat3::translation(vec3(6, 3, 4));
        mat3 test5 = mat3::translation(vec3(22, -19, 12));
        mat3 test6 = mat3::scale(2, 2, 2);
        mat3 test7 = mat3::identity();
        mat3 test8 = mat3::translation(vec3(32, 12, 4));
        mat3 test9 = mat3::translation(vec3(-18, 4, -89));
        mat3 test10 = mat3::angle_axis(vec3::right(),
                                       move::math::deg2rad<component_type>(23));
        mat3 test11 = mat3::translation(vec3(11, 333, 4));
        mat3 test12 = mat3::translation(vec3(1, 2, 3));
        mat3 test13 = mat3::angle_axis(vec3::up(),
                                       move::math::deg2rad<component_type>(65));
        mat3 test14 = mat3::translation(vec3(6, 3, 4));
        mat3 test15 = mat3::translation(vec3(22, -19, 12));

        mat3 tests[] = {test0,  test1,  test2,  test3, test4,  test5,
                        test6,  test7,  test8,  test9, test10, test11,
                        test12, test13, test14, test15};

        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10 * test11;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10 * test11 * test12;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10 * test11 * test12 *
                       test13;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10 * test11 * test12 *
                       test13 * test14;
            }());
        REQUIRE_NOTHROW(
            [&]
            {
                return test0 * test1 * test2 * test3 * test4 * test5 * test6 *
                       test7 * test8 * test9 * test10 * test11 * test12 *
                       test13 * test14 * test15;
            }());
    };
}

REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(test_mat3, move::math::mat3x3);
REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(benchmark_mat3, move::math::mat3x3);

SCENARIO("Mat3 full tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    test_mat3_multi<float, double>();
}

// SCENARIO("Mat3 benchmarks")
// {
//     using namespace move::math;
//     using Accel = move::math::Acceleration;

//     benchmark_mat3_multi<float, double>();
// }