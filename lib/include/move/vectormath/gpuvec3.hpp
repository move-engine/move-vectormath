#pragma once

#include "vec3.hpp"

namespace move::vectormath
{
    /**
     * @brief A 3D vector that is guaranteed to be correctly laid out for GPU
     * usage.
     *
     * This is a 3D vector that is guaranteed to be correctly laid out for GPU
     * usage. It should be used whenever a 3D vector is needed on the GPU.
     */
    struct gpuvec3
    {
        inline gpuvec3() = default;
        inline gpuvec3(const gpuvec3&) = default;
        inline gpuvec3(gpuvec3&&) = default;
        inline gpuvec3& operator=(const gpuvec3&) = default;
        inline gpuvec3& operator=(gpuvec3&&) = default;

        inline gpuvec3(const vec3& v) : x(v.x()), y(v.y()), z(v.z())
        {
        }

        inline gpuvec3(const move::vectormath::fastvec3& v)
            : x(v.x()), y(v.y()), z(v.z())
        {
        }

        inline operator vec3() const
        {
            return vec3(x, y, z);
        }

        inline operator fastvec3() const
        {
            return fastvec3(x, y, z);
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

        inline float* as_array()
        {
            return &x;
        }

        float x, y, z;
    };
}  // namespace move::vectormath