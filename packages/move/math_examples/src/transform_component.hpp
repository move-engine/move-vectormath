#pragma once

#include <type_traits>

#include <move/vectormath.hpp>

namespace examples
{
    template <typename T = float>
        requires std::is_floating_point_v<T>
    struct TransformComponent
    {
        using scalar_type = T;
        using vector_type =
            move::math::vec3<T, move::math::Acceleration::Default>;
        using quaternion_type = move::math::quat<T>;
        using matrix_type = move::math::mat4x4<T>;

        TransformComponent* parent = nullptr;
        vector_type local_position = vector_type::zero();
        quaternion_type local_rotation = quaternion_type::identity();
        vector_type local_scale = vector_type::one();

        [[nodiscard]] matrix_type local_matrix() const
        {
            return matrix_type::trs(
                local_position.fast(), local_rotation, local_scale.fast());
        }

        [[nodiscard]] matrix_type world_matrix() const
        {
            const matrix_type local = local_matrix();
            return parent ? local * parent->world_matrix() : local;
        }

        [[nodiscard]] matrix_type inverse_world_matrix() const
        {
            return world_matrix().inverse();
        }

        [[nodiscard]] vector_type world_position() const
        {
            return parent ? parent->transform_point(local_position)
                          : local_position;
        }

        [[nodiscard]] quaternion_type world_rotation() const
        {
            // With row-vector composition, local rotation is applied before the
            // parent rotation.
            return parent ? local_rotation * parent->world_rotation()
                          : local_rotation;
        }

        [[nodiscard]] vector_type world_scale() const
        {
            return parent ? local_scale * parent->world_scale() : local_scale;
        }

        void set_world_position(const vector_type& position)
        {
            local_position = parent ? parent->inverse_transform_point(position)
                                    : position;
        }

        void set_world_rotation(const quaternion_type& rotation)
        {
            local_rotation = parent ? rotation * parent->world_rotation().inverse()
                                    : rotation;
        }

        void set_world_scale(const vector_type& scale)
        {
            if (parent)
            {
                const vector_type parent_scale = parent->world_scale();
                local_scale = vector_type(scale.get_x() / parent_scale.get_x(),
                                          scale.get_y() / parent_scale.get_y(),
                                          scale.get_z() / parent_scale.get_z());
            }
            else
            {
                local_scale = scale;
            }
        }

        void translate_local(const vector_type& offset)
        {
            local_position += offset * local_rotation;
        }

        void translate_world(const vector_type& offset)
        {
            set_world_position(world_position() + offset);
        }

        void rotate_local(const quaternion_type& delta)
        {
            local_rotation = local_rotation * delta;
        }

        void rotate_world(const quaternion_type& delta)
        {
            set_world_rotation(delta * world_rotation());
        }

        [[nodiscard]] vector_type rotate_direction(
            const vector_type& direction) const
        {
            return vector_type(direction * world_rotation()).normalized();
        }

        [[nodiscard]] vector_type inverse_rotate_direction(
            const vector_type& direction) const
        {
            return vector_type(direction * world_rotation().inverse())
                .normalized();
        }

        [[nodiscard]] vector_type transform_point(
            const vector_type& point) const
        {
            return vector_type(world_matrix().transform_point(point.fast()));
        }

        [[nodiscard]] vector_type inverse_transform_point(
            const vector_type& point) const
        {
            return vector_type(
                inverse_world_matrix().transform_point(point.fast()));
        }

        [[nodiscard]] vector_type transform_vector(
            const vector_type& value) const
        {
            return vector_type(world_matrix().transform_vector(value.fast()));
        }

        [[nodiscard]] vector_type inverse_transform_vector(
            const vector_type& value) const
        {
            return vector_type(
                inverse_world_matrix().transform_vector(value.fast()));
        }

        [[nodiscard]] vector_type right() const
        {
            return rotate_direction(vector_type::right());
        }

        [[nodiscard]] vector_type up() const
        {
            return rotate_direction(vector_type::up());
        }

        [[nodiscard]] vector_type forward() const
        {
            return rotate_direction(vector_type::forward());
        }
    };
}  // namespace examples
