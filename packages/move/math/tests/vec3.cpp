#include <catch2/catch_all.hpp>
#include <catch2/catch_test_macros.hpp>

#include <magic_enum.hpp>
#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/scalar/base_vec3.hpp>
#include <move/math/vec3.hpp>
#if __has_include(<move/meta/type_utils.hpp>)
#define MVM_HAS_MOVE_CORE
#include <move/meta/type_utils.hpp>
#endif
#include "catch2/catch_approx.hpp"
#include "mm_test_common.hpp"

template <typename vec3>
inline void test_vec3()
{
    REQUIRE(vec3::element_count == 3);
    REQUIRE(move::math::traits::is_vector_type_v<vec3>);
    REQUIRE(!move::math::traits::is_matrix_type_v<vec3>);
    REQUIRE(move::math::traits::component_count_v<vec3> == 3);
    REQUIRE(std::is_same_v<move::math::traits::component_type_t<vec3>,
                           typename vec3::component_type>);
    ;

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
            if constexpr (vec3::has_pointer_semantics)
            {
                component_type* ptr = test.to_array();
                REQUIRE(ptr[0] == 3);
                REQUIRE(ptr[1] == 4);
                REQUIRE(ptr[2] == 5);
            }
            else
            {
                component_type ptr[3];
                test.store_array(ptr);
                REQUIRE(ptr[0] == 3);
                REQUIRE(ptr[1] == 4);
                REQUIRE(ptr[2] == 5);
            }
        }
    }

    WHEN("A vec3 is converted to a const pointer")
    {
        const vec3 test = {3, 4, 5};
        THEN("The pointer is correct")
        {
            if constexpr (vec3::has_pointer_semantics)
            {
                const component_type* ptr = test.to_array();
                REQUIRE(ptr[0] == 3);
                REQUIRE(ptr[1] == 4);
                REQUIRE(ptr[2] == 5);
            }
            else
            {
                component_type ptr[3];
                test.store_array(ptr);
                REQUIRE(ptr[0] == 3);
                REQUIRE(ptr[1] == 4);
                REQUIRE(ptr[2] == 5);
            }
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

    if constexpr (std::is_floating_point_v<component_type>)
    {
        WHEN("Testing reflect function")
        {
            vec3 incident = {1, -1, 0};
            vec3 normal = {0, 1, 0};  // Upward normal
            vec3 reflected = vec3::reflect(incident, normal);
            vec3 expected = {1, 1, 0};  // Reflected upward

            THEN("The reflection is correct")
            {
                REQUIRE(move::math::approx_equal(reflected, expected));
            }
        }

        WHEN("Testing refract function")
        {
            vec3 incident = {1, -1, 0};
            incident = incident.normalized();
            vec3 normal = {0, 1, 0};  // Upward normal
            component_type ior = component_type(1.2);  // More realistic IOR for testing
            vec3 refracted = vec3::refract(incident, normal, ior);

            THEN("The refracted vector is valid")
            {
                // Basic sanity check - refracted vector should not be NaN
                REQUIRE(!std::isnan(refracted.get_x()));
                REQUIRE(!std::isnan(refracted.get_y()));
                REQUIRE(!std::isnan(refracted.get_z()));

                // For this configuration, we shouldn't get total internal reflection
                REQUIRE(refracted.length() > component_type(0.01));
                // Entering a denser material bends the ray toward the normal.
                REQUIRE(refracted.get_y() < incident.get_y());
            }
        }

        WHEN("Testing refraction from air into a denser material")
        {
            vec3 incident = {1, -1, 0};
            incident = incident.normalized();
            vec3 normal = {0, 1, 0};
            vec3 refracted =
                vec3::refract(incident, normal, component_type(1.5));

            THEN("The refracted vector is finite, non-zero, and bends toward the normal")
            {
                REQUIRE(!std::isnan(refracted.get_x()));
                REQUIRE(!std::isnan(refracted.get_y()));
                REQUIRE(!std::isnan(refracted.get_z()));
                REQUIRE(refracted.length() ==
                        Catch::Approx(component_type(1)).epsilon(
                            component_type(0.001)));
                REQUIRE(refracted.get_y() < incident.get_y());
            }
        }

        WHEN("Testing total internal reflection when leaving a denser material")
        {
            vec3 incident = {1, component_type(0.2), 0};
            incident = incident.normalized();
            vec3 normal = {0, 1, 0};
            vec3 refracted =
                vec3::refract(incident, normal, component_type(1.5));

            THEN("The refracted vector collapses to zero")
            {
                REQUIRE(move::math::approx_equal(refracted, vec3(vec3::zero()),
                                                 component_type(0.001)));
            }
        }
    }

    WHEN("Testing abs function")
    {
        if constexpr (std::is_signed_v<component_type>)
        {
            vec3 test = {component_type(-1), component_type(2), component_type(-3)};
            vec3 result = vec3::abs(test);
            vec3 expected = {component_type(1), component_type(2), component_type(3)};

            THEN("The absolute values are correct")
            {
                REQUIRE(result == expected);
            }
        }
        else
        {
            // For unsigned types, abs should return the same value
            vec3 test = {component_type(1), component_type(2), component_type(3)};
            vec3 result = vec3::abs(test);

            THEN("The absolute values are correct (unsigned)")
            {
                REQUIRE(result == test);
            }
        }
    }

    WHEN("Testing sign function")
    {
        if constexpr (std::is_signed_v<component_type>)
        {
            vec3 test = {component_type(-2), component_type(0), component_type(3)};
            vec3 result = vec3::sign(test);
            vec3 expected = {component_type(-1), component_type(1), component_type(1)};  // Note: sign(0) = 1 by definition in common.hpp

            THEN("The signs are correct")
            {
                REQUIRE(result == expected);
            }
        }
        else
        {
            // For unsigned types, sign should always return 1
            vec3 test = {component_type(2), component_type(0), component_type(3)};
            vec3 result = vec3::sign(test);
            vec3 expected = {component_type(1), component_type(1), component_type(1)};

            THEN("The signs are correct (unsigned)")
            {
                REQUIRE(result == expected);
            }
        }
    }

    if constexpr (std::is_floating_point_v<component_type>)
    {
        WHEN("Testing project_onto_plane function")
        {
            vec3 v = {1, 2, 3};
            vec3 plane_normal = {0, 1, 0};  // XZ plane (normal is Y-axis)
            vec3 projected = vec3::project_onto_plane(v, plane_normal);
            vec3 expected = {1, 0, 3};  // Y component should be removed

            THEN("The projection is correct")
            {
                REQUIRE(move::math::approx_equal(projected, expected));
            }
        }

        WHEN("Testing project_onto_plane with diagonal plane")
        {
            vec3 v = {1, 1, 1};
            vec3 plane_normal = {1, 1, 1};
            plane_normal = plane_normal.normalized();  // Normalize the normal
            vec3 projected = vec3::project_onto_plane(v, plane_normal);

            THEN("The projected vector lies on the plane")
            {
                // The projected vector should be perpendicular to the normal
                component_type dot_with_normal = vec3::dot(projected, plane_normal);
                REQUIRE(move::math::approx_equal(dot_with_normal, component_type(0), component_type(0.001)));
            }
        }

        WHEN("Testing project_onto_plane with vector parallel to plane")
        {
            vec3 plane_normal = {0, 0, 1};  // XY plane
            vec3 v = {1, 1, 0};             // Already on XY plane
            vec3 projected = vec3::project_onto_plane(v, plane_normal);

            THEN("Vector parallel to plane remains unchanged")
            {
                REQUIRE(move::math::approx_equal(projected, v));
            }
        }

        WHEN("Testing project_onto_plane with vector perpendicular to plane")
        {
            vec3 plane_normal = {0, 0, 1};  // XY plane
            vec3 v = {0, 0, 5};             // Perpendicular to XY plane
            vec3 projected = vec3::project_onto_plane(v, plane_normal);

            THEN("Vector perpendicular to plane becomes zero")
            {
                vec3 expected = {0, 0, 0};
                REQUIRE(move::math::approx_equal(projected, expected));
            }
        }
    }

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

    if constexpr (std::is_same_v<component_type, float>)
    {
        WHEN("Serializing and deserializing a vec3 through an archive")
        {
            vec3 original = {component_type(1.25), component_type(-2.5),
                             component_type(3.75)};
            capture_archive<component_type> saver;
            original.serialize(saver);

            vec3 loaded;
            replay_archive<component_type> loader{saver.values};
            loaded.serialize(loader);

            THEN("The serialized values round-trip correctly")
            {
                REQUIRE(move::math::approx_equal(loaded, original,
                                                 component_type(0.001)));
            }
        }
    }
}
REPEAT_FOR_EACH_TYPE_WRAPPER(test_vec3, move::math::vec3);

TEST_CASE("scalar::base_vec3 operator-= returns self")
{
    move::math::scalar::base_vec3<float> lhs(3.0f, 4.0f, 5.0f);
    move::math::scalar::base_vec3<float> rhs(1.0f, 1.0f, 1.0f);

    auto* result = &(lhs -= rhs);

    REQUIRE(result == &lhs);
    REQUIRE(lhs.x == Catch::Approx(2.0f));
    REQUIRE(lhs.y == Catch::Approx(3.0f));
    REQUIRE(lhs.z == Catch::Approx(4.0f));
}

SCENARIO("Vec3 tests")
{
    using namespace move::math;
    using Accel = move::math::Acceleration;

    // Scalar tests
    test_vec3_multi<Accel::Scalar, float, double, int8_t, int16_t, int32_t,
                    int64_t, uint8_t, uint16_t, uint32_t, uint64_t>();
    // SIMD tests
    test_vec3_multi<Accel::RTM, float, double>();
}
