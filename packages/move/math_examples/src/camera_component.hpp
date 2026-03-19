#pragma once

#include <type_traits>

#include "transform_component.hpp"

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CameraComponent
    {
        using scalar_type = T;
        using vector_type =
            move::math::vec3<T, move::math::Acceleration::Default>;
        using screen_point_type = move::math::vec2<T, move::math::Acceleration::Scalar>;
        using matrix_type = move::math::mat4x4<T>;
        using quaternion_type = move::math::quat<T>;
        using transform_type = TransformComponent<T>;

        struct Ray
        {
            vector_type origin = vector_type::zero();
            vector_type direction = vector_type::forward();
        };

        transform_type transform;
        T vertical_fov = move::math::deg2rad(T(60));
        T aspect_ratio = T(16) / T(9);
        T near_plane = T(0.1);
        T far_plane = T(1000);

        [[nodiscard]] matrix_type view_matrix() const
        {
            const vector_type eye = transform.world_position();
            const vector_type forward = transform.forward();
            const vector_type up = transform.up();
            return matrix_type::look_at(
                eye.fast(), (eye + forward).fast(), up.fast());
        }

        [[nodiscard]] matrix_type projection_matrix() const
        {
            return matrix_type::perspective(
                vertical_fov, aspect_ratio, near_plane, far_plane);
        }

        [[nodiscard]] matrix_type view_projection_matrix() const
        {
            return view_matrix() * projection_matrix();
        }

        void look_at(const vector_type& target,
                     const vector_type& world_up = vector_type::up())
        {
            (void)world_up;
            const vector_type forward =
                (target - transform.world_position()).normalized();
            const T planar_length = move::math::sqrt(
                forward.get_x() * forward.get_x() +
                forward.get_z() * forward.get_z());
            const T yaw = move::math::atan2(forward.get_x(), forward.get_z());
            const T pitch =
                move::math::atan2(-forward.get_y(), planar_length);
            transform.set_world_rotation(quaternion_type::euler(
                pitch, yaw, T(0)));
        }

        [[nodiscard]] Ray ndc_to_world_ray(
            const screen_point_type& ndc) const
        {
            const T tan_half_y = move::math::tan(vertical_fov / T(2));
            const T tan_half_x = tan_half_y * aspect_ratio;

            const vector_type local_direction(
                ndc.get_x() * tan_half_x, ndc.get_y() * tan_half_y, T(1));

            return {
                transform.world_position(),
                transform.rotate_direction(local_direction).normalized(),
            };
        }
    };
}  // namespace examples
