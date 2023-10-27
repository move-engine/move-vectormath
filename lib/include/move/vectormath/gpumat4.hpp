#pragma once

#include "mat4.hpp"

namespace move::vectormath
{
    /**
     * @brief A 4D vector that is guaranteed to be correctly laid out for GPU
     * usage.
     *
     * This is a 4D vector that is guaranteed to be correctly laid out for GPU
     * usage. It should be used whenever a 4D vector is needed for GPU usage.
     */
    struct gpumat4
    {
        inline gpumat4() = default;
        inline gpumat4(const gpumat4&) = default;
        inline gpumat4(gpumat4&&) = default;
        inline gpumat4& operator=(const gpumat4&) = default;
        inline gpumat4& operator=(gpumat4&&) = default;

        inline gpumat4(const move::vectormath::mat4f& v)
        {
            for (int i = 0; i < 16; i++)
            {
                data[i] = v[i];
            }
        }

        inline gpumat4(const move::vectormath::mat4d& v)
        {
            for (int i = 0; i < 16; i++)
            {
                data[i] = v[i];
            }
        }

        inline float* as_array()
        {
            return data;
        }

        inline operator mat4f() const
        {
            return mat4f::create_from_array(data);
        }

        float data[16];
    };
}  // namespace move::vectormath