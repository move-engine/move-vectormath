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

        inline gpumat4(const move::vectormath::mat4& v)
        {
            for (int i = 0; i < 16; i++)
            {
                data[i] = v[i];
            }
        }

        inline gpumat4(const move::vectormath::fastmat4& v)
        {
        }

        inline mat4 as_mat4() const
        {
            mat4 res;
            for (int i = 0; i < 16; i++)
            {
                res[i] = data[i];
            }
            return res;
        }

        inline float* as_array()
        {
            return data;
        }

        inline operator mat4() const
        {
            return as_mat4();
        }

        inline operator fastmat4() const
        {
            return as_mat4().fast();
        }

        float data[16];
    };
}  // namespace move::vectormath