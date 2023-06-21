#pragma once

#include "vec2.hpp"

namespace move::vectormath
{
    /**
     * @brief A 2D vector that is guaranteed to be correctly laid out for GPU
     * usage.
     *
     * This is a 2D vector that is guaranteed to be correctly laid out for GPU
     * usage. It should be used whenever a 2D vector is needed for GPU usage.
     */
    struct gpuvec2
    {
        inline gpuvec2() = default;
        inline gpuvec2(const gpuvec2&) = default;
        inline gpuvec2(gpuvec2&&) = default;
        inline gpuvec2& operator=(const gpuvec2&) = default;
        inline gpuvec2& operator=(gpuvec2&&) = default;

        inline gpuvec2(const vec2& v) : x(v.x()), y(v.y())
        {
        }

        inline gpuvec2(const move::vectormath::fastvec2& v) : x(v.x()), y(v.y())
        {
        }

        inline operator vec2() const
        {
            return vec2(x, y);
        }

        inline operator fastvec2() const
        {
            return fastvec2(x, y);
        }

        /**
         * @brief Constructs a gpuvec2 with the given x and y values.
         *
         * @param x The x value of the gpuvec2.
         * @param y The y value of the gpuvec2.
         */
        inline gpuvec2(float x, float y) : x(x), y(y)
        {
        }

        float x, y;
    };
}  // namespace move::vectormath