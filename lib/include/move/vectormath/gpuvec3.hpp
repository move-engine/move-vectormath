#pragma once

#include "vec3.hpp"

namespace move::vectormath
{
    /**
     * @brief A 3D vector that is guaranteed to be correctly laid out for GPU
     * usage.
     *
     * This is a 3D vector that is guaranteed to be correctly laid out for GPU
     * usage. It should be used whenever a 3D vector is needed for GPU usage.
     */
    struct gpuvec3
    {
        inline gpuvec3() = default;
        inline gpuvec3(const gpuvec3&) = default;
        inline gpuvec3(gpuvec3&&) = default;
        inline gpuvec3& operator=(const gpuvec3&) = default;
        inline gpuvec3& operator=(gpuvec3&&) = default;

        inline gpuvec3(const vec3f& v) : x(v.x()), y(v.y()), z(v.z())
        {
        }

        inline gpuvec3(const vec3d& v) : x(v.x()), y(v.y()), z(v.z())
        {
        }

        /**
         * @brief Constructs a gpuvec3 with the given x and y values.
         *
         * @param x The x value of the gpuvec3.
         * @param y The y value of the gpuvec3.
         */
        inline gpuvec3(float x, float y, float z) : x(x), y(y), z(z)
        {
        }

        inline operator vec3f() const
        {
            return vec3f(x, y, z);
        }

        inline operator vec3d() const
        {
            return vec3d(x, y, z);
        }

        inline float* as_array()
        {
            return &x;
        }

        float x, y, z;
    };
}  // namespace move::vectormath