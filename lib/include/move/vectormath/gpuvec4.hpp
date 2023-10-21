#pragma once

#include "vec4.hpp"

namespace move::vectormath
{
    /**
     * @brief A 4D vector that is guaranteed to be correctly laid out for GPU
     * usage.
     *
     * This is a 4D vector that is guaranteed to be correctly laid out for GPU
     * usage. It should be used whenever a 4D vector is needed for GPU usage.
     */
    struct gpuvec4
    {
        inline gpuvec4() = default;
        inline gpuvec4(const gpuvec4&) = default;
        inline gpuvec4(gpuvec4&&) = default;
        inline gpuvec4& operator=(const gpuvec4&) = default;
        inline gpuvec4& operator=(gpuvec4&&) = default;

        inline gpuvec4(const vec4f& v) : x(v.x()), y(v.y()), z(v.z()), w(v.w())
        {
        }

        inline gpuvec4(const move::vectormath::fastvec4& v)
            : x(v.x()), y(v.y()), z(v.z()), w(v.w())
        {
        }

        inline operator vec4f() const
        {
            return vec4f(x, y, z, w);
        }

        inline operator fastvec4() const
        {
            return fastvec4(x, y, z, w);
        }

        /**
         * @brief Constructs a gpuvec4 with the given x and y values.
         *
         * @param x The x value of the gpuvec4.
         * @param y The y value of the gpuvec4.
         */
        inline gpuvec4(float x, float y, float z, float w)
            : x(x), y(y), z(z), w(w)
        {
        }

        inline float* as_array()
        {
            return &x;
        }

        float x, y, z, w;
    };
}  // namespace move::vectormath