#pragma once

#include <optional>
#include <type_traits>

#include <mv/math/Math.hpp>

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct TransformComponent
    {
        using scalar_type = T;
        using vector_type = mv::math::Vec3<T>;
        using point_type = mv::math::Point3<T>;
        using rotation_type = mv::math::Rotation3<T>;
        using matrix_type = mv::math::Mat4<T>;

        TransformComponent* parent = nullptr;
        vector_type local_position = vector_type::Zero();
        rotation_type local_rotation = rotation_type::Identity();
        vector_type local_scale = vector_type::One();

        [[nodiscard]] matrix_type local_matrix() const
        {
            return mv::math::ToMat4(mv::math::TrsTransform3<T>(
                local_position, local_rotation, local_scale));
        }

        [[nodiscard]] matrix_type world_matrix() const
        {
            const matrix_type local = local_matrix();
            return parent ? local * parent->world_matrix() : local;
        }

        [[nodiscard]] std::optional<matrix_type> inverse_world_matrix() const
        {
            return world_matrix().TryInverse();
        }

        [[nodiscard]] vector_type world_position() const
        {
            return parent ? parent->transform_point(local_position)
                          : local_position;
        }

        [[nodiscard]] rotation_type world_rotation() const
        {
            // Rotation multiplication composes its right operand first.
            return parent ? parent->world_rotation() * local_rotation
                          : local_rotation;
        }

        [[nodiscard]] vector_type world_scale() const
        {
            return parent ? local_scale * parent->world_scale() : local_scale;
        }

        [[nodiscard]] bool try_set_world_position(const vector_type& position)
        {
            if (!parent)
            {
                local_position = position;
                return true;
            }

            const auto local = parent->try_inverse_transform_point(position);
            if (!local)
            {
                return false;
            }
            local_position = *local;
            return true;
        }

        void set_world_rotation(const rotation_type& rotation)
        {
            local_rotation = parent
                                 ? parent->world_rotation().Inverse() * rotation
                                 : rotation;
        }

        [[nodiscard]] bool try_set_world_scale(const vector_type& scale)
        {
            if (!parent)
            {
                local_scale = scale;
                return true;
            }

            const vector_type parent_scale = parent->world_scale();
            if (parent_scale.X() == T(0) || parent_scale.Y() == T(0) ||
                parent_scale.Z() == T(0))
            {
                return false;
            }
            local_scale = scale / parent_scale;
            return true;
        }

        void translate_local(const vector_type& offset)
        {
            local_position += mv::math::Rotate(local_rotation, offset);
        }

        [[nodiscard]] bool translate_world(const vector_type& offset)
        {
            return try_set_world_position(world_position() + offset);
        }

        void rotate_local(const rotation_type& delta)
        {
            local_rotation = local_rotation * delta;
        }

        void rotate_world(const rotation_type& delta)
        {
            set_world_rotation(delta * world_rotation());
        }

        [[nodiscard]] vector_type rotate_vector(const vector_type& vector) const
        {
            return mv::math::Rotate(world_rotation(), vector);
        }

        [[nodiscard]] vector_type inverse_rotate_vector(
            const vector_type& vector) const
        {
            return mv::math::Rotate(world_rotation().Inverse(), vector);
        }

        [[nodiscard]] vector_type transform_point(
            const vector_type& point) const
        {
            const mv::math::Vec4<T> homogeneous(point, T(1));
            return (homogeneous * world_matrix()).XYZ();
        }

        [[nodiscard]] std::optional<vector_type> try_inverse_transform_point(
            const vector_type& point) const
        {
            const auto inverse = inverse_world_matrix();
            if (!inverse)
            {
                return std::nullopt;
            }
            return (mv::math::Vec4<T>(point, T(1)) * *inverse).XYZ();
        }

        [[nodiscard]] vector_type transform_vector(
            const vector_type& vector) const
        {
            return (mv::math::Vec4<T>(vector, T(0)) * world_matrix()).XYZ();
        }

        [[nodiscard]] std::optional<vector_type> try_inverse_transform_vector(
            const vector_type& vector) const
        {
            const auto inverse = inverse_world_matrix();
            if (!inverse)
            {
                return std::nullopt;
            }
            return (mv::math::Vec4<T>(vector, T(0)) * *inverse).XYZ();
        }

        [[nodiscard]] mv::math::Direction3<T> right() const
        {
            return mv::math::Rotate(world_rotation(),
                                    mv::math::Direction3<T>::AxisX());
        }

        [[nodiscard]] mv::math::Direction3<T> up() const
        {
            return mv::math::Rotate(world_rotation(),
                                    mv::math::Direction3<T>::AxisY());
        }

        [[nodiscard]] mv::math::Direction3<T> forward() const
        {
            return mv::math::Rotate(world_rotation(),
                                    mv::math::Direction3<T>::AxisZ());
        }
    };
}  // namespace examples
