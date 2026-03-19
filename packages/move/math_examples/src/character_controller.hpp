#pragma once

#include <cstdint>
#include <type_traits>

#include "transform_component.hpp"

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CharacterSweepHit
    {
        using vector_type =
            move::math::vec3<T, move::math::Acceleration::Default>;

        T fraction = T(1);
        vector_type point = vector_type::zero();
        vector_type normal = vector_type::up();
        bool started_penetrating = false;
    };

    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CharacterPhysicsWorld
    {
        using vector_type =
            move::math::vec3<T, move::math::Acceleration::Default>;

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
        using vector_type =
            move::math::vec3<T, move::math::Acceleration::Default>;
        using transform_type = TransformComponent<T>;

        transform_type transform;
        vector_type velocity = vector_type::zero();

        T radius = T(0.35);
        T height = T(1.8);
        T skin_width = T(0.02);
        T ground_probe_distance = T(0.1);
        T max_ground_angle_radians = move::math::deg2rad(T(50));
        std::uint32_t max_slide_iterations = 4;

        bool grounded = false;
        vector_type ground_normal = vector_type::up();

        [[nodiscard]] T half_height() const
        {
            return move::math::max(height * T(0.5) - radius, T(0));
        }

        [[nodiscard]] vector_type position() const
        {
            return transform.world_position();
        }

        void set_position(const vector_type& value)
        {
            transform.set_world_position(value);
        }

        void move_and_slide(const CharacterPhysicsWorld<T>& world,
                            const vector_type& desired_displacement)
        {
            grounded = false;
            ground_normal = vector_type::up();

            resolve_penetration(world);

            vector_type remaining = desired_displacement;
            for (std::uint32_t iteration = 0;
                 iteration < max_slide_iterations &&
                 remaining.length_squared() > T(1.0e-8);
                 ++iteration)
            {
                CharacterSweepHit<T> hit;
                if (!world.sweep_capsule(
                        position(), radius, half_height(), remaining, hit))
                {
                    set_position(position() + remaining);
                    break;
                }

                if (hit.started_penetrating)
                {
                    resolve_penetration(world);
                    break;
                }

                const T travel_fraction = move::math::clamp(
                    hit.fraction - skin_fraction(remaining.length()), T(0), T(1));
                set_position(position() + remaining * travel_fraction);

                const vector_type leftover =
                    remaining * (T(1) - travel_fraction);
                remaining = project_onto_plane(leftover, hit.normal);

                if (is_ground(hit.normal))
                {
                    grounded = true;
                    ground_normal = hit.normal.normalized();
                }
            }

            vector_type probe_normal = vector_type::up();
            if (world.probe_ground(position(), radius, half_height(),
                                   ground_probe_distance, probe_normal) &&
                is_ground(probe_normal))
            {
                grounded = true;
                ground_normal = probe_normal.normalized();
            }

            if (grounded)
            {
                const T into_ground = vector_type::dot(velocity, ground_normal);
                if (into_ground < T(0))
                {
                    velocity -= ground_normal * into_ground;
                }
            }
        }

        void integrate(const CharacterPhysicsWorld<T>& world,
                       T delta_time,
                       const vector_type& gravity)
        {
            if (!grounded)
            {
                velocity += gravity * delta_time;
            }

            move_and_slide(world, velocity * delta_time);
        }

    private:
        [[nodiscard]] T skin_fraction(T displacement_length) const
        {
            if (displacement_length <= T(1.0e-6))
            {
                return T(0);
            }

            return move::math::clamp(
                skin_width / displacement_length, T(0), T(0.25));
        }

        [[nodiscard]] bool is_ground(const vector_type& normal) const
        {
            const T min_ground_dot = move::math::cos(max_ground_angle_radians);
            return vector_type::dot(normal.normalized(), vector_type::up()) >=
                   min_ground_dot;
        }

        [[nodiscard]] static vector_type project_onto_plane(
            const vector_type& value, const vector_type& plane_normal)
        {
            const vector_type normalized = plane_normal.normalized();
            return value - normalized * vector_type::dot(value, normalized);
        }

        void resolve_penetration(const CharacterPhysicsWorld<T>& world)
        {
            vector_type depenetration = vector_type::zero();
            if (world.depenetrate_capsule(
                    position(), radius, half_height(), depenetration))
            {
                set_position(position() + depenetration);
            }
        }
    };
}  // namespace examples
