#pragma once

#include "rtm/impl/compiler_utils.h"
#include "vec3.hpp"

namespace move::vectormath
{
    using vector3_type = vec3f;
    // template <typename vec3_type>
    class normal
    {
    public:
        using vec3_type = vector3_type;
        using component_type = typename vec3_type::component_type;

    public:
        RTM_FORCE_INLINE normal() noexcept = default;
        RTM_FORCE_INLINE normal(const normal&) noexcept = default;
        RTM_FORCE_INLINE normal(normal&&) noexcept = default;
        RTM_FORCE_INLINE normal& operator=(const normal&) noexcept = default;
        RTM_FORCE_INLINE normal& operator=(normal&&) noexcept = default;
        RTM_FORCE_INLINE ~normal() noexcept = default;

        RTM_FORCE_INLINE normal(const vec3_type& vec) noexcept : _vec(vec)
        {
            normalize();
        }

        RTM_FORCE_INLINE normal(
            component_type x, component_type y, component_type z) noexcept
            : _vec(rtm::vector_normalize3(rtm::vector_set(x, y, z, 0)))
        {
        }

    public:
        RTM_FORCE_INLINE bool operator==(const normal& other) const noexcept
        {
            return _vec == other._vec;
        }

        RTM_FORCE_INLINE bool operator!=(const normal& other) const noexcept
        {
            return _vec != other._vec;
        }

        RTM_FORCE_INLINE normal operator-() const noexcept
        {
            return normal(-_vec);
        }

        RTM_FORCE_INLINE normal operator+(const normal& other) const noexcept
        {
            return normal(_vec + other._vec);
        }

        RTM_FORCE_INLINE normal operator-(const normal& other) const noexcept
        {
            return normal(_vec - other._vec);
        }

    private:
        RTM_FORCE_INLINE void normalize() noexcept
        {
            _vec = rtm::vector_normalize3(_vec);
        }

    public:
        RTM_FORCE_INLINE component_type get_component(int index) const noexcept
        {
            return _vec.get_component(index);
        }

        RTM_FORCE_INLINE normal& set_component(
            int index, component_type value) noexcept
        {
            _vec.set_component(index, value);
            normalize();
        }

        RTM_FORCE_INLINE component_type x() const noexcept
        {
            return _vec.x();
        }

        RTM_FORCE_INLINE component_type y() const noexcept
        {
            return _vec.y();
        }

        RTM_FORCE_INLINE component_type z() const noexcept
        {
            return _vec.z();
        }

        RTM_FORCE_INLINE normal& set(
            component_type x, component_type y, component_type z) noexcept
        {
            _vec.set(x, y, z);
            normalize();
            return *this;
        }

        RTM_FORCE_INLINE normal& set(const vec3_type& vec) noexcept
        {
            _vec = vec;
            normalize();
            return *this;
        }

        RTM_FORCE_INLINE normal& set(const normal& vec) noexcept
        {
            _vec = vec._vec;
            return *this;
        }

    public:
        RTM_FORCE_INLINE component_type dot(const normal& other) const noexcept
        {
            return _vec.dot(other._vec);
        }

        RTM_FORCE_INLINE component_type angle_between(
            const normal& other) const noexcept
        {
            return _vec.angle_between_normalized_vectors(other._vec);
        }

        RTM_FORCE_INLINE component_type length() const noexcept
        {
            return 1;
        }

        RTM_FORCE_INLINE component_type squared_length() const noexcept
        {
            return 1;
        }

        RTM_FORCE_INLINE component_type reciprocal_length() const noexcept
        {
            return 1;
        }

    public:
        RTM_FORCE_INLINE normal cross(const normal& other) const noexcept
        {
            return normal(_vec.cross(other._vec));
        }

        RTM_FORCE_INLINE normal cross(const vec3_type& other) const noexcept
        {
            return normal(_vec.cross(other));
        }

        RTM_FORCE_INLINE normal reflect(const normal& normalv) const noexcept
        {
            return normal(_vec.reflect(normalv._vec));
        }

        RTM_FORCE_INLINE normal reflect(const vec3_type& normalv) const noexcept
        {
            return normal(_vec.reflect(normalv));
        }

        RTM_FORCE_INLINE normal refract(
            const normal& normalv, component_type ior) const noexcept
        {
            return normal(_vec.refract(normalv._vec, ior));
        }

        RTM_FORCE_INLINE normal refract(
            const vec3_type& normalv, component_type ior) const noexcept
        {
            return normal(_vec.refract(normalv, ior));
        }

    public:
        RTM_FORCE_INLINE const vec3_type& vec() const noexcept
        {
            return _vec;
        }

    public:
        static RTM_FORCE_INLINE normal x_axis() noexcept
        {
            return normal(vec3_type::x_axis());
        }

        static RTM_FORCE_INLINE normal y_axis() noexcept
        {
            return normal(vec3_type::y_axis());
        }

        static RTM_FORCE_INLINE normal z_axis() noexcept
        {
            return normal(vec3_type::z_axis());
        }

        static RTM_FORCE_INLINE normal right() noexcept
        {
            return normal(vec3_type::right());
        }

        static RTM_FORCE_INLINE normal left() noexcept
        {
            return normal(vec3_type::left());
        }

        static RTM_FORCE_INLINE normal up() noexcept
        {
            return normal(vec3_type::up());
        }

        static RTM_FORCE_INLINE normal down() noexcept
        {
            return normal(vec3_type::down());
        }

        static RTM_FORCE_INLINE normal forward() noexcept
        {
            return normal(vec3_type::forward());
        }

        static RTM_FORCE_INLINE normal backward() noexcept
        {
            return normal(vec3_type::backward());
        }

    private:
        vec3_type _vec;
    };
}  // namespace move::vectormath