#pragma once

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <type_traits>

#include "transform_component.hpp"

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CharacterSweepHit
    {
        using vector_type = mv::math::Vec3<T>;

        T fraction = T(1);
        vector_type point = vector_type::Zero();
        vector_type normal = vector_type::AxisY();
        bool started_penetrating = false;
    };

    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CharacterPhysicsWorld
    {
        using vector_type = mv::math::Vec3<T>;

        virtual ~CharacterPhysicsWorld() = default;

        virtual bool sweep_capsule(const vector_type& center,
                                   T radius,
                                   T half_height,
                                   const vector_type& displacement,
                                   CharacterSweepHit<T>& out_hit) const = 0;

        virtual bool depenetrate_capsule(const vector_type& center,
                                         T radius,
                                         T half_height,
                                         vector_type& out_offset) const = 0;

        virtual bool probe_ground(const vector_type& center,
                                  T radius,
                                  T half_height,
                                  T probe_distance,
                                  vector_type& out_normal) const = 0;
    };

    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CollideAndSlideCharacterController
    {
        using scalar_type = T;
        using vector_type = mv::math::Vec3<T>;
        using transform_type = TransformComponent<T>;

        transform_type transform;
        vector_type velocity = vector_type::Zero();

        T radius = T(0.35);
        T height = T(1.8);
        T skin_width = T(0.02);
        T ground_probe_distance = T(0.1);
        mv::math::Radians<T> max_ground_angle =
            mv::math::ToRadians(mv::math::Degrees<T>(T(50)));
        std::uint32_t max_slide_iterations = 4;

        bool grounded = false;
        vector_type ground_normal = vector_type::AxisY();

        [[nodiscard]] T half_height() const
        {
            return std::max(height * T(0.5) - radius, T(0));
        }

        [[nodiscard]] vector_type position() const
        {
            return transform.world_position();
        }

        [[nodiscard]] bool set_position(const vector_type& value)
        {
            return transform.try_set_world_position(value);
        }

        [[nodiscard]] bool move_and_slide(
            const CharacterPhysicsWorld<T>& world,
            const vector_type& desired_displacement)
        {
            grounded = false;
            ground_normal = vector_type::AxisY();

            resolve_penetration(world);

            vector_type remaining = desired_displacement;
            for (std::uint32_t iteration = 0;
                 iteration < max_slide_iterations &&
                 mv::math::LengthSquared(remaining) > T(1.0e-8);
                 ++iteration)
            {
                CharacterSweepHit<T> hit;
                if (!world.sweep_capsule(position(), radius, half_height(),
                                         remaining, hit))
                {
                    if (!set_position(position() + remaining))
                    {
                        return false;
                    }
                    break;
                }

                if (hit.started_penetrating)
                {
                    resolve_penetration(world);
                    break;
                }

                const T travel_fraction = std::clamp(
                    hit.fraction - skin_fraction(mv::math::Length(remaining)),
                    T(0), T(1));
                if (!set_position(position() + remaining * travel_fraction))
                {
                    return false;
                }

                const vector_type leftover =
                    remaining * (T(1) - travel_fraction);
                remaining = project_onto_plane(leftover, hit.normal);

                if (is_ground(hit.normal))
                {
                    grounded = true;
                    ground_normal = normalized_or_up(hit.normal);
                }
            }

            vector_type probe_normal = vector_type::AxisY();
            if (world.probe_ground(position(), radius, half_height(),
                                   ground_probe_distance, probe_normal) &&
                is_ground(probe_normal))
            {
                grounded = true;
                ground_normal = normalized_or_up(probe_normal);
            }

            if (grounded)
            {
                const T into_ground = mv::math::Dot(velocity, ground_normal);
                if (into_ground < T(0))
                {
                    velocity -= ground_normal * into_ground;
                }
            }
            return true;
        }

        [[nodiscard]] bool integrate(const CharacterPhysicsWorld<T>& world,
                                     T delta_time,
                                     const vector_type& gravity)
        {
            if (!grounded)
            {
                velocity += gravity * delta_time;
            }

            return move_and_slide(world, velocity * delta_time);
        }

    private:
        [[nodiscard]] T skin_fraction(T displacement_length) const
        {
            if (displacement_length <= T(1.0e-6))
            {
                return T(0);
            }

            return std::clamp(skin_width / displacement_length, T(0), T(0.25));
        }

        [[nodiscard]] bool is_ground(const vector_type& normal) const
        {
            const auto direction = mv::math::Direction3<T>::TryFrom(normal);
            return direction &&
                   mv::math::Dot(direction->Vector(), vector_type::AxisY()) >=
                       std::cos(max_ground_angle.Value());
        }

        [[nodiscard]] static vector_type project_onto_plane(
            const vector_type& value, const vector_type& plane_normal)
        {
            const auto normal = mv::math::Normal3<T>::TryFrom(plane_normal);
            return normal ? mv::math::ProjectOntoPlane(value, *normal) : value;
        }

        [[nodiscard]] static vector_type normalized_or_up(
            const vector_type& value)
        {
            const auto direction = mv::math::Direction3<T>::TryFrom(value);
            return direction ? direction->Vector() : vector_type::AxisY();
        }

        void resolve_penetration(const CharacterPhysicsWorld<T>& world)
        {
            vector_type depenetration = vector_type::Zero();
            if (world.depenetrate_capsule(position(), radius, half_height(),
                                          depenetration))
            {
                (void)set_position(position() + depenetration);
            }
        }
    };
}  // namespace examples
