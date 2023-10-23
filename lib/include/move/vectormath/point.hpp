#pragma once
#include "vec3.hpp"

namespace move::vectormath
{
    using vector3_type = vec3f;
    // template <typename vec3_type>
    class point
    {
    public:
        using vec3_type = vector3_type;
        using component_type = typename vec3_type::component_type;

    public:
        inline point() noexcept = default;
        inline point(const point&) noexcept = default;
        inline point(point&&) noexcept = default;
        inline point& operator=(const point&) noexcept = default;
        inline point& operator=(point&&) noexcept = default;
        inline ~point() noexcept = default;

        inline point(const vec3_type& vec) noexcept : _vec(vec)
        {
        }

        inline point(
            component_type x, component_type y, component_type z) noexcept
            : _vec(x, y, z)
        {
        }

    public:
        inline component_type x() const noexcept
        {
            return _vec.x();
        }

        inline component_type y() const noexcept
        {
            return _vec.y();
        }

        inline component_type z() const noexcept
        {
            return _vec.z();
        }

        inline point& x(component_type value) noexcept
        {
            _vec.x(value);
            return *this;
        }

    public:
        inline vec3_type& vec() noexcept
        {
            return _vec;
        }

        inline const vec3_type& vec() const noexcept
        {
            return _vec;
        }

    public:
    private:
        vec3_type _vec;
    };
}  // namespace move::vectormath