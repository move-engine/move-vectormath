#include <catch2/catch_all.hpp>
#include <catch2/catch_approx.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>

#include <movemm/memory-allocator.h>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/quat.hpp>
#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif
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

        THEN("Logarithm and exponential are inverse operations")
        {
            // For a unit quaternion, exp(ln(q)) should equal q
            quat log_result = rotation.ln();
            quat exp_result = log_result.exp();
            REQUIRE(move::math::approx_equal(rotation, exp_result, component_type(0.001)));
            
            // Also test static versions
            quat static_log = quat::ln(rotation);
            quat static_exp = quat::exp(static_log);
            REQUIRE(move::math::approx_equal(rotation, static_exp, component_type(0.001)));
            REQUIRE(move::math::approx_equal(log_result, static_log, component_type(0.001)));
        }

        THEN("Identity quaternion logarithm is zero")
        {
            quat identity = quat::identity();
            quat log_identity = identity.ln();
            quat zero_quat = quat::zero();
            
            REQUIRE(move::math::approx_equal(log_identity, zero_quat, component_type(0.001)));
        }

        THEN("Zero quaternion exponential is identity")
        {
            quat zero_quat = quat::zero();
            quat exp_zero = zero_quat.exp();
            quat identity = quat::identity();
            
            REQUIRE(move::math::approx_equal(exp_zero, identity, component_type(0.001)));
        }

        THEN("Logarithm of quaternion multiplication is addition of logarithms")
        {
            // This is a property of quaternion logarithms for commuting quaternions
            // For rotations around the same axis: ln(q1 * q2) ≈ ln(q1) + ln(q2)
            quat rotation1 = quat::angle_axis(vec3(1, 0, 0), move::math::deg2rad(component_type(30)));
            quat rotation2 = quat::angle_axis(vec3(1, 0, 0), move::math::deg2rad(component_type(60)));
            
            quat combined = rotation1 * rotation2;
            quat log_combined = combined.ln();
            
            quat log1 = rotation1.ln();
            quat log2 = rotation2.ln();
            
            // For pure quaternions (w=0), addition is component-wise
            component_type log_combined_x = log_combined.get_x();
            component_type log_combined_y = log_combined.get_y();
            component_type log_combined_z = log_combined.get_z();
            component_type log_combined_w = log_combined.get_w();
            
            component_type log_sum_x = log1.get_x() + log2.get_x();
            component_type log_sum_y = log1.get_y() + log2.get_y();
            component_type log_sum_z = log1.get_z() + log2.get_z();
            component_type log_sum_w = log1.get_w() + log2.get_w();
            
            REQUIRE(move::math::abs(log_combined_x - log_sum_x) < component_type(0.001));
            REQUIRE(move::math::abs(log_combined_y - log_sum_y) < component_type(0.001));
            REQUIRE(move::math::abs(log_combined_z - log_sum_z) < component_type(0.001));
            REQUIRE(move::math::abs(log_combined_w - log_sum_w) < component_type(0.001));
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

// SCENARIO("Quat benchmarks")
// {
//     using namespace move::math;
//     using Accel = move::math::Acceleration;

//     benchmark_quat_multi<float, double>();
// }