#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>

#include <movemm/memory-allocator.h>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/mat4x4.hpp>
#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif
#include <move/string.hpp>

#include "mm_test_common.hpp"

template <typename mat4>
inline void test_mat4()
{
    using component_type = mat4::component_type;
    using vec3 = mat4::vec3_t;
    using vec4 = mat4::vec4_t;
    using quat = mat4::quat_t;
    static constexpr auto acceleration = mat4::acceleration;

    INFO("Testing mat4 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
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

    // Bulk ops crash testing
    {
        mat4 test1 = mat4::identity();
        mat4 test2 = mat4::translation(vec3(1, 2, 3));
        mat4 test3 = mat4::angle_axis(vec3::up(),
                                      move::math::deg2rad<component_type>(65));
        mat4 test4 = mat4::translation(vec3(6, 3, 4));
        mat4 test5 = mat4::translation(vec3(22, -19, 12));
        mat4 test6 = mat4::scale(2, 2, 2);
        mat4 test7 = mat4::identity();
        mat4 test8 = mat4::translation(vec3(32, 12, 4));
        mat4 test9 = mat4::translation(vec3(-18, 4, -89));
        mat4 test10 = mat4::angle_axis(vec3::right(),
                                       move::math::deg2rad<component_type>(23));
        mat4 test11 = mat4::translation(vec3(11, 333, 4));
        mat4 test12 = mat4::translation(vec3(1, 2, 3));
        mat4 test13 = mat4::angle_axis(vec3::up(),
                                       move::math::deg2rad<component_type>(65));
        mat4 test14 = mat4::translation(vec3(6, 3, 4));
        mat4 test15 = mat4::translation(vec3(22, -19, 12));
        mat4 test16 = mat4::scale(2, 2, 2);

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
            vec4 result = test * translation;
            REQUIRE(result == vec4(2, 4, 6, 1));
        }

        THEN(
            "Multiplying the vector by the inverse results in the correct "
            "transformation")
        {
            vec4 test = {2, 4, 6, 1};
            vec4 result = test * translation.inverse();
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
            vec4 result = test * rotation;
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
            vec4 result = test * opposite;
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
            vec4 result = test * rotation.inverse();
            vec4 expected = {0, 0, 1, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }
    }

    // Construct full transformation matrix
    {
        vec3 translation = {5, 0, 5};
        quat rotation = quat::angle_axis(
            vec3::up(), move::math::deg2rad<component_type>(90));
        vec3 scaleFac = {2, 2, 2};
        auto trs = mat4::trs(translation, rotation, scaleFac);

        vec3 pointToTransform = {1, 0, 0};

        THEN("The matrix is correct")
        {
            auto transformedPoint = vec4(pointToTransform, 1) * trs;
            auto expectedMatrix = mat4::scale(scaleFac) *
                                  mat4::rotation(rotation) *
                                  mat4::translation(translation);
            auto expectedTransformedPoint =
                vec4(pointToTransform, 1) * expectedMatrix;

            INFO(trs << " vs " << expectedMatrix);
            REQUIRE(move::math::approx_equal(trs, expectedMatrix,
                                             component_type(0.001)));
        }
    }

    // Construct a look_at matrix
    {
        auto defaultLookAt =
            mat4::look_at(vec3::zero(), vec3::forward(), vec3::up());

        THEN("The matrix is correct")
        {
            auto expected =
                mat4(1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1, 0, 0, 0, 0, 1);
            INFO(defaultLookAt << " vs " << expected);
            REQUIRE(move::math::approx_equal(defaultLookAt, expected,
                                             component_type(0.001)));
        }

        THEN(
            "Multiplying a vector by the matrix results in the correct "
            "transformation")
        {
            vec4 test = {1, 0, 0, 1};
            vec4 result = test * defaultLookAt;
            vec4 expected = {1, 0, 0, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }

        THEN(
            "Multiplying the vector by the inverse results in the correct "
            "transformation")
        {
            vec4 test = {1, 0, 0, 1};
            vec4 result = test * defaultLookAt.inverse();
            vec4 expected = {1, 0, 0, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }
    }

    // Construct a look_at matrix with a non - default eye position
    {
        auto defaultLookAt =
            mat4::look_at(vec3::left() * 2, vec3::right(), vec3::up());

        THEN("The matrix is correct")
        {
            auto expected =
                mat4(0, 0, -1, 0, 0, 1, 0, 0, 1, 0, 0, 0, 2, 0, 0, 1);
            INFO(defaultLookAt << " vs " << expected);
            REQUIRE(move::math::approx_equal(defaultLookAt, expected,
                                             component_type(0.001)));
        }

        THEN(
            "Multiplying a vector by the matrix results in the correct "
            "transformation")
        {
            vec4 test = {0, 1, 0, 1};
            vec4 result = test * defaultLookAt;
            vec4 expected = {0, 1, 0, 1};

            INFO(result << " vs " << expected);
            REQUIRE(move::math::approx_equal(result, expected));
        }
    }

    // Construct a broken look_at matrix
    {
        auto brokenLookAt = mat4::look_at({1, 2, 3}, {4, 5, 6}, {7, 8, 9});
        auto broken2 =
            mat4::filled(std::numeric_limits<component_type>::signaling_NaN());

        REQUIRE_FALSE(move::math::approx_equal(brokenLookAt, broken2,
                                               component_type(0.001)));
    }

    // Construct a perspective matrix
    {
        // TODO: Test this
    }

    // Construct a centered orthographic matrix
    {
        // TODO: Test this
    }

    // Construct an off-center orthographic matrix
    {
        // TODO: Test this
    }
}

template <typename mat4>
inline void benchmark_mat4()
{
    using component_type = mat4::component_type;
    using vec3 = mat4::vec3_t;
    using vec4 = mat4::vec4_t;
    using quat = mat4::quat_t;
    static constexpr auto acceleration = mat4::acceleration;
    move::string_view typeName = move::meta::type_name<mat4>();

    INFO("Benchmarking mat4 with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    INFO("\tquat: " << move::meta::type_name<quat>());
    INFO("\tmat4: " << move::meta::type_name<mat4>());

    mat4 identity;
    BENCHMARK(alloc_appended_name(typeName, ": Identity construction"))
    {
        return mat4();
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
        return mat4::trs(translation, rotation, scaleFac);
    };

    BENCHMARK(
        alloc_appended_name(typeName, ": Transform Construction (manual)"))
    {
        vec3 translation = {1, 2, 3};
        quat rotation = quat::angle_axis(vec3::up(), move::math::deg2rad(90));
        vec3 scaleFac = {1, 1, 1};

        auto scale = mat4::scale(scaleFac);
        auto rot = mat4::rotation(rotation);
        auto tra = mat4::translation(translation);

        auto result1 = scale * rot;
        auto result2 = result1 * tra;

        return result2;
    };

    BENCHMARK(alloc_appended_name(typeName,
                                  ": Transform Construction (manual, inline)"))
    {
        return
            //
            mat4::scale(scaleFac) *
            //
            mat4::rotation(rotation) *
            //
            mat4::translation(translation);
    };

    // Bulk ops crash testing
    BENCHMARK(alloc_appended_name(typeName, ": Bulk ops"))
    {
        mat4 test0 = mat4::scale(2, 2, 2);
        mat4 test1 = mat4::identity();
        mat4 test2 = mat4::translation(vec3(1, 2, 3));
        mat4 test3 = mat4::angle_axis(vec3::up(),
                                      move::math::deg2rad<component_type>(65));
        mat4 test4 = mat4::translation(vec3(6, 3, 4));
        mat4 test5 = mat4::translation(vec3(22, -19, 12));
        mat4 test6 = mat4::scale(2, 2, 2);
        mat4 test7 = mat4::identity();
        mat4 test8 = mat4::translation(vec3(32, 12, 4));
        mat4 test9 = mat4::translation(vec3(-18, 4, -89));
        mat4 test10 = mat4::angle_axis(vec3::right(),
                                       move::math::deg2rad<component_type>(23));
        mat4 test11 = mat4::translation(vec3(11, 333, 4));
        mat4 test12 = mat4::translation(vec3(1, 2, 3));
        mat4 test13 = mat4::angle_axis(vec3::up(),
                                       move::math::deg2rad<component_type>(65));
        mat4 test14 = mat4::translation(vec3(6, 3, 4));
        mat4 test15 = mat4::translation(vec3(22, -19, 12));

        mat4 tests[] = {test0,  test1,  test2,  test3, test4,  test5,
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