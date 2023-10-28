#pragma once

#include "rtm/impl/compiler_utils.h"
#include "vec3.hpp"

namespace move::vectormath
{
    using vector3_type = vec3f;
    // template <typename vec3_type>
    class generic_normal
    {
    public:
        using vec3_type = vector3_type;
        using component_type = typename vec3_type::component_type;

    public:
        MVM_INLINE_NODISCARD generic_normal() noexcept = default;
        MVM_INLINE_NODISCARD generic_normal(
            const generic_normal&) noexcept = default;
        MVM_INLINE_NODISCARD generic_normal(
            generic_normal&&) noexcept = default;
        MVM_INLINE_NODISCARD generic_normal& operator=(
            const generic_normal&) noexcept = default;
        MVM_INLINE_NODISCARD generic_normal& operator=(
            generic_normal&&) noexcept = default;
        MVM_INLINE ~generic_normal() noexcept = default;

        MVM_INLINE_NODISCARD generic_normal(const vec3_type& vec) noexcept
            : _vec(vec)
        {
            normalize();
        }

        MVM_INLINE_NODISCARD generic_normal(
            component_type x, component_type y, component_type z) noexcept
            : _vec(rtm::vector_normalize3(rtm::vector_set(x, y, z, 0)))
        {
        }

    public:
        MVM_INLINE_NODISCARD bool operator==(
            const generic_normal& other) const noexcept
        {
            return _vec == other._vec;
        }

        MVM_INLINE_NODISCARD bool operator!=(
            const generic_normal& other) const noexcept
        {
            return _vec != other._vec;
        }

        MVM_INLINE_NODISCARD generic_normal operator-() const noexcept
        {
            return generic_normal(-_vec);
        }

        MVM_INLINE_NODISCARD generic_normal operator+(
            const generic_normal& other) const noexcept
        {
            return generic_normal(_vec + other._vec);
        }

        MVM_INLINE_NODISCARD generic_normal operator-(
            const generic_normal& other) const noexcept
        {
            return generic_normal(_vec - other._vec);
        }

    private:
        MVM_INLINE void normalize() noexcept
        {
            _vec = rtm::vector_normalize3(_vec);
        }

    public:
        MVM_INLINE_NODISCARD component_type get_component(
            int index) const noexcept
        {
            return _vec.get_component(index);
        }

        MVM_INLINE_NODISCARD generic_normal& set_component(
            int index, component_type value) noexcept
        {
            _vec.set_component(index, value);
            normalize();
            return *this;
        }

        MVM_INLINE_NODISCARD component_type x() const noexcept
        {
            return _vec.x();
        }

        MVM_INLINE_NODISCARD component_type y() const noexcept
        {
            return _vec.y();
        }

        MVM_INLINE_NODISCARD component_type z() const noexcept
        {
            return _vec.z();
        }

        MVM_INLINE_NODISCARD generic_normal& set(
            component_type x, component_type y, component_type z) noexcept
        {
            _vec.set(x, y, z);
            normalize();
            return *this;
        }

        MVM_INLINE_NODISCARD generic_normal& set(const vec3_type& vec) noexcept
        {
            _vec = vec;
            normalize();
            return *this;
        }

        MVM_INLINE_NODISCARD generic_normal& set(
            const generic_normal& vec) noexcept
        {
            _vec = vec._vec;
            return *this;
        }

    public:
        MVM_INLINE_NODISCARD component_type dot(
            const generic_normal& n1, const generic_normal& n2) const noexcept
        {
            return _vec.dot(n1._vec, n2._vec);
        }

        MVM_INLINE_NODISCARD component_type angle_between(
            const generic_normal& n1, const generic_normal& n2) const noexcept
        {
            return _vec.angle_between_normalized_vectors(n1._vec, n2._vec);
        }

        MVM_INLINE_NODISCARD component_type length() const noexcept
        {
            return 1;
        }

        MVM_INLINE_NODISCARD component_type squared_length() const noexcept
        {
            return 1;
        }

        MVM_INLINE_NODISCARD component_type reciprocal_length() const noexcept
        {
            return 1;
        }

    public:
        MVM_INLINE_NODISCARD static generic_normal cross(
            const generic_normal& n1, const generic_normal& other) noexcept
        {
            return generic_normal(vec3_type::cross(n1._vec, other._vec));
        }

        MVM_INLINE_NODISCARD static generic_normal cross(
            const generic_normal& n1, const vec3_type& other) noexcept
        {
            return generic_normal(vec3_type::cross(n1._vec, other));
        }

        MVM_INLINE_NODISCARD static generic_normal reflect(
            const generic_normal& n1, const generic_normal& normalv) noexcept
        {
            return generic_normal(vec3_type::reflect(n1._vec, normalv._vec));
        }

        MVM_INLINE_NODISCARD static generic_normal reflect(
            const vec3_type& incident, const vec3_type& normalv) noexcept
        {
            return generic_normal(vec3_type::reflect(incident, normalv));
        }

        MVM_INLINE_NODISCARD static generic_normal refract(
            const vec3_type& incident, const generic_normal& normalv,
            component_type ior) noexcept
        {
            return generic_normal(
                vec3_type::refract(incident, normalv._vec, ior));
        }

        MVM_INLINE_NODISCARD static generic_normal refract(
            const vec3_type& incident, const vec3_type& normalv,
            component_type ior) noexcept
        {
            return generic_normal(vec3_type::refract(incident, normalv, ior));
        }

    public:
        MVM_INLINE_NODISCARD const vec3_type& vec() noexcept
        {
            return _vec;
        }

    public:
        MVM_INLINE_NODISCARD static generic_normal x_axis() noexcept
        {
            return generic_normal(vec3_type::x_axis());
        }

        MVM_INLINE_NODISCARD static generic_normal y_axis() noexcept
        {
            return generic_normal(vec3_type::y_axis());
        }

        MVM_INLINE_NODISCARD static generic_normal z_axis() noexcept
        {
            return generic_normal(vec3_type::z_axis());
        }

        MVM_INLINE_NODISCARD static generic_normal right() noexcept
        {
            return generic_normal(vec3_type::right());
        }

        MVM_INLINE_NODISCARD static generic_normal left() noexcept
        {
            return generic_normal(vec3_type::left());
        }

        MVM_INLINE_NODISCARD static generic_normal up() noexcept
        {
            return generic_normal(vec3_type::up());
        }

        MVM_INLINE_NODISCARD static generic_normal down() noexcept
        {
            return generic_normal(vec3_type::down());
        }

        MVM_INLINE_NODISCARD static generic_normal forward() noexcept
        {
            return generic_normal(vec3_type::forward());
        }

        MVM_INLINE_NODISCARD static generic_normal backward() noexcept
        {
            return generic_normal(vec3_type::backward());
        }

    private:
        vec3_type _vec;
    };

    using normalf = generic_normal;
    using normald = generic_normal;
#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using normal = normald;
#else
    using normal = generic_normal;
#endif
}  // namespace move::vectormath