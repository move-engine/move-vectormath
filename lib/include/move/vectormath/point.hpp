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
            : _vec(rtm::vector_set(x, y, z, 0))
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

        inline point& y(component_type value) noexcept
        {
            _vec.y(value);
            return *this;
        }

        inline point& z(component_type value) noexcept
        {
            _vec.z(value);
            return *this;
        }

        inline point& set(
            component_type x, component_type y, component_type z) noexcept
        {
            _vec.set(x, y, z);
            return *this;
        }

        inline point& set(const vec3_type& vec) noexcept
        {
            _vec = vec;
            return *this;
        }

        inline point& fill(component_type value) noexcept
        {
            _vec.fill(value);
            return *this;
        }

    public:
        inline bool operator==(const point& other) const noexcept
        {
            return _vec == other._vec;
        }

        inline bool operator!=(const point& other) const noexcept
        {
            return _vec != other._vec;
        }

        inline point operator-() const noexcept
        {
            return point(-_vec);
        }

        inline point operator+(const point& other) const noexcept
        {
            return point(_vec + other._vec);
        }

        inline point operator-(const point& other) const noexcept
        {
            return point(_vec - other._vec);
        }

        inline point operator*(component_type scalar) const noexcept
        {
            return point(_vec * scalar);
        }

        inline point operator/(component_type scalar) const noexcept
        {
            return point(_vec / scalar);
        }

        inline point& operator+=(const point& other) noexcept
        {
            _vec += other._vec;
            return *this;
        }

        inline point& operator-=(const point& other) noexcept
        {
            _vec -= other._vec;
            return *this;
        }

        inline point& operator*=(component_type scalar) noexcept
        {
            _vec *= scalar;
            return *this;
        }

        inline point& operator/=(component_type scalar) noexcept
        {
            _vec /= scalar;
            return *this;
        }

    public:
        inline component_type get_component(int index) const noexcept
        {
            return _vec.get_component(index);
        }

        inline point& set_component(int index, component_type value) noexcept
        {
            _vec.set_component(index, value);
            return *this;
        }

    public:
        inline component_type length() const noexcept
        {
            return _vec.length();
        }

        inline component_type squared_length() const noexcept
        {
            return _vec.squared_length();
        }

        inline component_type reciprocal_length() const noexcept
        {
            return _vec.reciprocal_length();
        }

        inline component_type distance(const point& other) const noexcept
        {
            return _vec.distance_to_point(other._vec);
        }

        inline component_type squared_distance(
            const point& other) const noexcept
        {
            return _vec.squared_distance_to_point(other._vec);
        }

        inline component_type distance_to_line(
            const point& point1, const point& point2) const noexcept
        {
            return _vec.distance_to_line(point1._vec, point2._vec);
        }

        inline point& normalize() noexcept
        {
            _vec.normalize();
            return *this;
        }

        inline point normalized() const noexcept
        {
            return point(_vec.normalized());
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