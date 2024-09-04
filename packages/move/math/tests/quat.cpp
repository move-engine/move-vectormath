#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>

#include <movemm/memory-allocator.h>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/quat.hpp>
#include <move/meta/type_utils.hpp>
#include <move/string.hpp>

#include "mm_test_common.hpp"

template <typename quat>
inline void test_quat()
{
    using component_type = quat::component_type;
    using vec3 = quat::vec3_t;
    using vec4 = quat::vec4_t;
    static constexpr auto acceleration = quat::acceleration;

    INFO("Testing quat with following config:");
    INFO("\tcomponent_type: " << move::meta::type_name<component_type>());
    INFO("\tacceleration: " << magic_enum::enum_name(acceleration));
    INFO("\tvec3: " << move::meta::type_name<vec3>());
    INFO("\tvec4: " << move::meta::type_name<vec4>());
    INFO("\tquat: " << move::meta::type_name<quat>());

    REQUIRE(quat::zero() == quat(0, 0, 0, 0));
    REQUIRE(quat::identity() == quat(0, 0, 0, 1));

    // Identity testing
    {
        quat identity;
        WHEN("The inverse of an identity quaternion is calculated")
        {
            quat result = identity.inverse();
            THEN("The result is correct")
            {
                REQUIRE(result == quat::identity());
            }
        }

        WHEN("The conjugate of an identity quaternion is calculated")
        {
            quat result = identity.conjugate();
            THEN("The result is correct")
            {
                REQUIRE(result == quat::identity());
            }
        }

        WHEN("The length of an identity quaternion is calculated")
        {
            component_type result = identity.length();
            THEN("The result is correct")
            {
                REQUIRE(result == 1);
            }
        }
    }

    // Rotation testing
    {
        using Approx = Catch::Approx;
        quat rotation =
            quat::angle_axis(vec3(1, 0, 0), move::math::deg2rad(90));

        THEN("The inverse is correct")
        {
            quat inverse = rotation.inverse();
            quat expected =
                quat::angle_axis(vec3(1, 0, 0), move::math::deg2rad(-90));
            REQUIRE(move::math::approx_equal(inverse, expected));
        }

        THEN("The length is correct")
        {
            component_type result = rotation.length();
            REQUIRE(result == Approx(1));
        }

        THEN("The length squared is correct")
        {
            component_type result = rotation.length_squared();
            REQUIRE(result == Approx(1));
        }

        THEN("The dot product is correct")
        {
            component_type result = rotation.dot(rotation);
            REQUIRE(result == Approx(1));
        }

        THEN("Rotating a point is correct")
        {
            vec3 point(1, 0, 0);
            vec3 result = rotation.rotate_point(point);
            REQUIRE(result == vec3(1, 0, 0));
        }

        THEN("Rotating a point is correct")
        {
            vec3 point(1, 0, 0);
            vec3 result = point * rotation;
            REQUIRE(result == vec3(1, 0, 0));
        }
    }
}

template <typename quat>
inline void benchmark_quat()
{
    using component_type = quat::component_type;
    using vec3 = quat::vec3_t;
    using vec4 = quat::vec4_t;
    static constexpr auto acceleration = quat::acceleration;
    move::string_view typeName = move::meta::type_name<quat>();

    quat identity;
    BENCHMARK(alloc_appended_name(typeName, ": Identity construction"))
    {
        return quat();
    };

    BENCHMARK(alloc_appended_name(typeName, ": Inverse"))
    {
        return identity.inverse();
    };
}

REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(test_quat, move::math::quat);
REPEAT_FOR_EACH_TYPE_WRAPPER_NOACCEL(benchmark_quat, move::math::quat);

SCENARIO("Quat full tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    test_quat_multi<float, double>();
}

SCENARIO("Quat benchmarks")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    benchmark_quat_multi<float, double>();
}