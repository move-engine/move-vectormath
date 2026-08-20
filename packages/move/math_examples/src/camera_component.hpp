#pragma once

#include <cmath>
#include <optional>
#include <type_traits>

#include "transform_component.hpp"

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct CameraComponent
    {
        using scalar_type = T;
        using vector_type = mv::math::Vec3<T>;
        using screen_point_type = mv::math::Vec2<T>;
        using matrix_type = mv::math::Mat4<T>;
        using ray_type = mv::math::Ray3<T>;
        using transform_type = TransformComponent<T>;

        transform_type transform;
        mv::math::Radians<T> vertical_fov =
            mv::math::ToRadians(mv::math::Degrees<T>(T(60)));
        T aspect_ratio = T(16) / T(9);
        T near_plane = T(0.1);
        T far_plane = T(1000);

        [[nodiscard]] std::optional<matrix_type> view_matrix() const
        {
            const mv::math::Point3<T> eye =
                mv::math::Point3<T>::FromVector(transform.world_position());
            return mv::math::TryLookAtLH(
                eye, eye + transform.forward().Vector(), transform.up());
        }

        [[nodiscard]] std::optional<matrix_type> projection_matrix() const
        {
            return mv::math::TryPerspectiveFov(
                vertical_fov, aspect_ratio, near_plane, far_plane,
                mv::math::Handedness::Left, mv::math::ClipDepth::ZeroToOne,
                mv::math::DepthDirection::Forward);
        }

        [[nodiscard]] std::optional<matrix_type> view_projection_matrix() const
        {
            const auto view = view_matrix();
            const auto projection = projection_matrix();
            return view && projection
                       ? std::optional<matrix_type>(*view * *projection)
                       : std::nullopt;
        }

        [[nodiscard]] bool look_at(const vector_type& target,
                                   const mv::math::Direction3<T>& world_up =
                                       mv::math::Direction3<T>::AxisY())
        {
            const auto forward = mv::math::Direction3<T>::TryFrom(
                target - transform.world_position());
            if (!forward)
            {
                return false;
            }
            const auto rotation =
                mv::math::Rotation3<T>::TryLookTowards(*forward, world_up);
            if (!rotation)
            {
                return false;
            }
            transform.set_world_rotation(*rotation);
            return true;
        }

        [[nodiscard]] std::optional<ray_type> ndc_to_world_ray(
            const screen_point_type& ndc) const
        {
            const T tan_half_y = std::tan(vertical_fov.Value() / T(2));
            const T tan_half_x = tan_half_y * aspect_ratio;
            const auto local_direction = mv::math::Direction3<T>::TryFrom(
                vector_type(ndc.X() * tan_half_x, ndc.Y() * tan_half_y, T(1)));
            if (!local_direction)
            {
                return std::nullopt;
            }

            const mv::math::Direction3<T> world_direction =
                mv::math::Rotate(transform.world_rotation(), *local_direction);
            return ray_type::TryFromOriginDirection(
                mv::math::Point3<T>::FromVector(transform.world_position()),
                world_direction);
        }
    };
}  // namespace examples
