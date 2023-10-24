#pragma once
#include "normal.hpp"
#include "vec3.hpp"

namespace move::vectormath
{
    // template <typename vec3_type>
    using vector3_type = vec3f;
    class generic_point
    {
    public:
        using vec3_type = vector3_type;
        using component_type = typename vec3_type::component_type;

    public:
        RTM_FORCE_INLINE generic_point() noexcept = default;
        RTM_FORCE_INLINE generic_point(const generic_point&) noexcept = default;
        RTM_FORCE_INLINE generic_point(generic_point&&) noexcept = default;
        RTM_FORCE_INLINE generic_point& operator=(
            const generic_point&) noexcept = default;
        RTM_FORCE_INLINE generic_point& operator=(
            generic_point&&) noexcept = default;
        RTM_FORCE_INLINE ~generic_point() noexcept = default;

        RTM_FORCE_INLINE generic_point(const vec3_type& vec) noexcept
            : _vec(vec)
        {
        }

        RTM_FORCE_INLINE generic_point(component_type x, component_type y = 0,
            component_type z = 0) noexcept
            : _vec(rtm::vector_set(x, y, z, 0))
        {
        }

    public:
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

        RTM_FORCE_INLINE generic_point& x(component_type value) noexcept
        {
            _vec.x(value);
            return *this;
        }

        RTM_FORCE_INLINE generic_point& y(component_type value) noexcept
        {
            _vec.y(value);
            return *this;
        }

        RTM_FORCE_INLINE generic_point& z(component_type value) noexcept
        {
            _vec.z(value);
            return *this;
        }

        RTM_FORCE_INLINE generic_point& set(
            component_type x, component_type y, component_type z) noexcept
        {
            _vec.set(x, y, z);
            return *this;
        }

        RTM_FORCE_INLINE generic_point& set(const vec3_type& vec) noexcept
        {
            _vec = vec;
            return *this;
        }

        RTM_FORCE_INLINE generic_point& fill(component_type value) noexcept
        {
            _vec.fill(value);
            return *this;
        }

    public:
        RTM_FORCE_INLINE bool operator==(
            const generic_point& other) const noexcept
        {
            return _vec == other._vec;
        }

        RTM_FORCE_INLINE bool operator!=(
            const generic_point& other) const noexcept
        {
            return _vec != other._vec;
        }

        RTM_FORCE_INLINE generic_point operator-() const noexcept
        {
            return generic_point(-_vec);
        }

        RTM_FORCE_INLINE vec3_type operator+(
            const generic_point& other) const noexcept
        {
            return _vec + other._vec;
        }

        RTM_FORCE_INLINE vec3_type operator-(
            const generic_point& other) const noexcept
        {
            return _vec - other._vec;
        }

        RTM_FORCE_INLINE vec3_type operator*(
            component_type scalar) const noexcept
        {
            return _vec * scalar;
        }

        RTM_FORCE_INLINE generic_point operator/(
            component_type scalar) const noexcept
        {
            return generic_point(_vec / scalar);
        }

        RTM_FORCE_INLINE generic_point& operator+=(
            const generic_point& other) noexcept
        {
            _vec += other._vec;
            return *this;
        }

        RTM_FORCE_INLINE generic_point& operator-=(
            const generic_point& other) noexcept
        {
            _vec -= other._vec;
            return *this;
        }

        RTM_FORCE_INLINE generic_point& operator*=(
            component_type scalar) noexcept
        {
            _vec *= scalar;
            return *this;
        }

        RTM_FORCE_INLINE generic_point& operator/=(
            component_type scalar) noexcept
        {
            _vec /= scalar;
            return *this;
        }

    public:
        RTM_FORCE_INLINE component_type get_component(int index) const noexcept
        {
            return _vec.get_component(index);
        }

        RTM_FORCE_INLINE generic_point& set_component(
            int index, component_type value) noexcept
        {
            _vec.set_component(index, value);
            return *this;
        }

    public:
        RTM_FORCE_INLINE component_type length() const noexcept
        {
            return _vec.length();
        }

        RTM_FORCE_INLINE component_type squared_length() const noexcept
        {
            return _vec.squared_length();
        }

        RTM_FORCE_INLINE component_type reciprocal_length() const noexcept
        {
            return _vec.reciprocal_length();
        }

        RTM_FORCE_INLINE component_type distance(
            const generic_point& other) const noexcept
        {
            return _vec.distance_to_point(other._vec);
        }

        RTM_FORCE_INLINE component_type squared_distance(
            const generic_point& other) const noexcept
        {
            return _vec.squared_distance_to_point(other._vec);
        }

        RTM_FORCE_INLINE component_type distance_to_line(
            const generic_point& generic_point1,
            const generic_point& generic_point2) const noexcept
        {
            return _vec.distance_to_line(
                generic_point1._vec, generic_point2._vec);
        }

        RTM_FORCE_INLINE normal normalized() const noexcept
        {
            return normal(_vec.normalized());
        }

    public:
        RTM_FORCE_INLINE vec3_type& vec() noexcept
        {
            return _vec;
        }

        RTM_FORCE_INLINE const vec3_type& vec() const noexcept
        {
            return _vec;
        }

    public:
    private:
        vec3_type _vec;
    };

    using pointf = generic_point;
    using pointd = generic_point;
#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using point = pointd;
#else
    using point = pointf;
#endif
}  // namespace move::vectormath