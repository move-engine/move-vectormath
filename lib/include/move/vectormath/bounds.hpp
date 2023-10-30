#pragma once

#include "vec2.hpp"
#include "vec3.hpp"
#include "vec4.hpp"

namespace move::vectormath
{
    template <typename vector_type>
    struct generic_bounds
    {
    public:
        using component_type = typename vector_type::component_type;
        using vec_type = vector_type;

    public:
        /**
         * @brief Creates a new bounding box with the minimum and maximum set to
         * zero.
         */
        generic_bounds() = default;

        /**
         * @brief Creates a new bounding box with the given minimum and maximum
         * values.
         */
        MVM_INLINE generic_bounds(const vec_type& min, const vec_type& max)
            : _min(min), _max(max)
        {
        }

        /**
         * @brief Creates a new bounding box with the minimum and maximum
         * bounded exactly to a single point.
         */
        MVM_INLINE generic_bounds(const vec_type& point)
            : _min(point), _max(point)
        {
        }

    public:
        /**
         * @brief Returns the minimum value of the bounding box.
         *
         * @return The minimum value of the bounding box.
         */
        MVM_INLINE_NODISCARD vec_type min() const
        {
            return _min;
        }

        /**
         * @brief Returns the maximum value of the bounding box.
         *
         * @return The maximum value of the bounding box.
         */
        MVM_INLINE_NODISCARD vec_type max() const
        {
            return _max;
        }

        /**
         * @brief Sets the minimum value of the bounding box.
         *
         * @param min The new minimum value of the bounding box.
         * @return A reference to the bounding box.
         */
        MVM_INLINE generic_bounds& min(const vec_type& min)
        {
            _min = min;
            return *this;
        }

        /**
         * @brief Sets the maximum value of the bounding box.
         *
         * @param max The new maximum value of the bounding box.
         * @return A reference to the bounding box.
         */
        MVM_INLINE generic_bounds& max(const vec_type& max)
        {
            _max = max;
            return *this;
        }

        /**
         * @brief Returns the center of the bounding box.
         *
         * @return The center of the bounding box.
         */
        MVM_INLINE_NODISCARD vec_type center() const
        {
            return (_min + _max) / 2;
        }

        /**
         * @brief Returns the size of the bounding box.
         *
         * @return The size of the bounding box.
         */
        MVM_INLINE_NODISCARD vec_type size() const
        {
            return _max - _min;
        }

        /**
         * @brief Returns the extents of the bounding box.
         *
         * @return The extents of the bounding box.
         */
        MVM_INLINE_NODISCARD vec_type extents() const
        {
            return size() / 2;
        }

        /**
         * @brief Checks if the bounding box contains the given point.
         *
         * @param point The point to check.
         * @return True if the bounding box contains the point, false otherwise.
         */
        MVM_INLINE_NODISCARD bool contains(const vec_type& point) const
        {
            return point >= _min && point <= _max;
        }

        /**
         * @brief Returns the closest point on the bounding box to the given
         * point.
         *
         * @param point The point to check.
         * @return The closest point on the bounding box to the given point.
         */
        MVM_INLINE vec_type closest_point(const vec_type& point) const
        {
            return vec_type::clamp(point, _min, _max);
        }

        /**
         * @brief Checks if the bounding box intersects with the given bounding
         * box.
         *
         * @param bounds The bounding box to check.
         * @return true if the bounding box intersects with the given bounding
         * box
         * @return false if the bounding box does not intersect with the given
         * bounding box.
         */
        MVM_INLINE_NODISCARD bool intersects(const generic_bounds& bounds) const
        {
            return _min <= bounds._max && _max >= bounds._min;
        }

        /**
         * @brief Checks if the bounding box intersects with a ray.
         *
         * @param origin The origin of the ray.
         * @param direction The direction of the ray.
         * @param tmin The minimum distance of the ray.
         * @param tmax The maximum distance of the ray.
         * @return true if the bounding box intersects with the ray.
         * @return false if the bounding box does not intersect with the ray.
         */
        MVM_INLINE_NODISCARD bool intersects(const vec_type& origin,
            const vec_type& direction, const component_type tmin,
            const component_type tmax) const
        {
            auto tmin2 = tmin;
            auto tmax2 = tmax;

            for (int i = 0; i < vec_type::num_components; ++i)
            {
                auto invD = component_type(1) / direction[i];
                auto origini = origin[i];

                auto t0 = (_min[i] - origini) * invD;
                auto t1 = (_max[i] - origini) * invD;

                if (invD < component_type(0))
                {
                    std::swap(t0, t1);
                }

                tmin2 = t0 > tmin2 ? t0 : tmin2;
                tmax2 = t1 < tmax2 ? t1 : tmax2;

                if (tmax2 <= tmin2)
                {
                    return false;
                }
            }

            return true;
        }

    public:
        /**
         * @brief Grows the bounding box to include the given point.
         *
         * @param point The point to include.
         */
        MVM_INLINE generic_bounds& encapsulate(const vec_type& point)
        {
            _min = vec_type::min(_min, point);
            _max = vec_type::max(_max, point);
            return *this;
        }

        /**
         * @brief Grows the bounding box to include the given bounding box.
         *
         * @param bounds The bounding box to include.
         */
        MVM_INLINE generic_bounds& encapsulate(const generic_bounds& bounds)
        {
            _min = vec_type::min(_min, bounds._min);
            _max = vec_type::max(_max, bounds._max);
            return *this;
        }

        /**
         * @brief Grows the bounding box by the given amount.
         *
         * @param amount The amount to grow the bounding box by.
         */
        MVM_INLINE generic_bounds& expand(const vec_type& amount)
        {
            _min -= amount;
            _max += amount;
            return *this;
        }

    private:
        vec_type _min;
        vec_type _max;
    };

    using bounds2f32 = generic_bounds<vec2f32>;
    using bounds2f64 = generic_bounds<vec2f64>;
    using bounds2i8 = generic_bounds<vec2i8>;
    using bounds2i16 = generic_bounds<vec2i16>;
    using bounds2i32 = generic_bounds<vec2i32>;
    using bounds2i64 = generic_bounds<vec2i64>;
    using bounds2u8 = generic_bounds<vec2u8>;
    using bounds2u16 = generic_bounds<vec2u16>;
    using bounds2u32 = generic_bounds<vec2u32>;
    using bounds2u64 = generic_bounds<vec2u64>;

    using bounds2f = generic_bounds<vec2f>;
    using bounds2d = generic_bounds<vec2d>;
    using bounds2i = generic_bounds<vec2i>;
    using bounds2u = generic_bounds<vec2u>;

    using rectf = bounds2f;
    using rect2 = bounds2d;

    using bounds3f = generic_bounds<vec3f>;
    using bounds3d = generic_bounds<vec3d>;
    using aabbf = bounds3f;
    using aabbd = bounds3d;

    using bounds4f = generic_bounds<vec4f>;
    using bounds4d = generic_bounds<vec4d>;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using bounds2 = bounds2d;
    using bounds3 = bounds3d;
    using bounds4 = bounds4d;
#else
    using bounds2 = bounds2f;
    using bounds3 = bounds3f;
    using bounds4 = bounds4f;
#endif

    using rect = bounds2;
    using aabb = bounds3;
}  // namespace move::vectormath