#pragma once
#include <ostream>

#include "rtm/impl/matrix_affine_common.h"
#include "rtm/impl/matrix_cast.h"
#include "rtm/impl/matrix_common.h"
#include "rtm/quatd.h"
#include "underlying_types.hpp"

#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <rtm/scalard.h>
#include <rtm/types.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include "vec3.hpp"
#include "vec4.hpp"

#include "rtm_ext.hpp"

#include <fmt/format.h>

#if !defined(MOVE_generic_mat4_HPP)
#define MOVE_generic_mat4_HPP
#endif

namespace move::vectormath
{
    using value_type = float;
    using vector3_type_raw = wrappers::v4f;
    using vector4_type_raw = wrappers::v4f;
    using quat_type_raw = wrappers::qf;
    using matrix_type_raw = wrappers::m4x4f;
    using matrix3x4_type_raw = wrappers::m3x4f;
    struct generic_mat4
    {
        using component_type = value_type;
        using underlying_vector3_type = typename vector3_type_raw::type;
        using underlying_vector4_type = typename vector4_type_raw::type;
        using underlying_quat_type = typename quat_type_raw::type;
        using underlying_matrix3x4_type = typename matrix3x4_type_raw::type;
        using underlying_matrix4x4_type = typename matrix_type_raw::type;
        using vec3_type = generic_vec3<value_type, vector3_type_raw>;
        using vec4_type = generic_vec4<value_type, vector4_type_raw>;

        inline generic_mat4() noexcept : _value()
        {
        }

        inline generic_mat4(float x00, float x01, float x02, float x03,
            float x10, float x11, float x12, float x13, float x20, float x21,
            float x22, float x23, float x30, float x31, float x32,
            float x33) noexcept
            : _value(rtm::matrix_set(rtm::vector_set(x00, x01, x02, x03),
                  rtm::vector_set(x10, x11, x12, x13),
                  rtm::vector_set(x20, x21, x22, x23),
                  rtm::vector_set(x30, x31, x32, x33)))
        {
        }

        inline generic_mat4(const underlying_vector4_type& row0,
            const underlying_vector4_type& row1,
            const underlying_vector4_type& row2,
            const underlying_vector4_type& row3) noexcept
            : _value(rtm::matrix_set(row0, row1, row2, row3))
        {
        }

        inline generic_mat4(const underlying_matrix4x4_type& rhs) noexcept
            : _value(rhs)
        {
        }

    public:
        inline operator underlying_matrix4x4_type&() noexcept
        {
            return _value;
        }

        inline generic_mat4 operator*(const generic_mat4& m) const noexcept
        {
            using namespace ::rtm;
            return generic_mat4(matrix_mul(_value, m._value));
        }

        inline generic_mat4& operator*=(const generic_mat4& m) noexcept
        {
            using namespace ::rtm;
            _value = matrix_mul(_value, m._value);
            return *this;
        }

    public:
        inline generic_mat4 inverse()
        {
            return generic_mat4(rtm::matrix_inverse(_value));
        }

    public:
        static inline generic_mat4 create_identity() noexcept
        {
            return generic_mat4(rtm::matrix_identity());
        }

        static inline generic_mat4 create_rotation_x(value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(1.0), value_type(0.0f), value_type(0.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        static inline generic_mat4 create_rotation_y(value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(0.0), value_type(1.0f), value_type(0.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        static inline generic_mat4 create_rotation_z(value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(0.0), value_type(0.0f), value_type(1.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        static inline generic_mat4 create_translation(
            value_type x, value_type y, value_type z) noexcept
        {
            using rtm::vector_set;
            return {
                vector_set(
                    value_type(1), value_type(0), value_type(0), value_type(0)),
                vector_set(
                    value_type(0), value_type(1), value_type(0), value_type(0)),
                vector_set(
                    value_type(0), value_type(0), value_type(1), value_type(0)),
                vector_set(x, y, z, value_type(1)),
            };
        }

        static inline generic_mat4 create_scale(
            value_type x, value_type y, value_type z) noexcept
        {
            using rtm::vector_set;
            return {
                vector_set(x, value_type(0), value_type(0), value_type(0)),
                vector_set(value_type(0), y, value_type(0), value_type(0)),
                vector_set(value_type(0), value_type(0), z, value_type(0)),
                vector_set(
                    value_type(0), value_type(0), value_type(0), value_type(1)),
            };
        }

        static inline generic_mat4 create_perspective_rh(value_type fov,
            value_type aspect, value_type near, value_type far) noexcept
        {
            return rtm::ext::perspective_fov_rh(fov, aspect, near, far);
        }

        static inline generic_mat4 create_perspective_lh(value_type fov,
            value_type aspect, value_type near, value_type far) noexcept
        {
            return rtm::ext::perspective_fov_lh(fov, aspect, near, far);
        }

        static inline generic_mat4 create_perspective(value_type fov,
            value_type aspect, value_type near, value_type far) noexcept
        {
            return create_perspective_rh(fov, aspect, near, far);
        }

        static inline generic_mat4 create_ortho_rh(value_type width,
            value_type height, value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_rh(width, height, near, far);
        }

        static inline generic_mat4 create_ortho_lh(value_type width,
            value_type height, value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_lh(width, height, near, far);
        }

        static inline generic_mat4 create_ortho(value_type width,
            value_type height, value_type near, value_type far) noexcept
        {
            return create_ortho_rh(width, height, near, far);
        }

        static inline generic_mat4 create_ortho_off_center_rh(value_type left,
            value_type right, value_type bottom, value_type top,
            value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_off_center_rh(
                left, right, bottom, top, near, far);
        }

        static inline generic_mat4 create_ortho_off_center_lh(value_type left,
            value_type right, value_type bottom, value_type top,
            value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_off_center_lh(
                left, right, bottom, top, near, far);
        }

    public:
        // Externally defined
        // vector3_type operator*(const vector3_type& v) const noexcept;
        // vector4_type operator*(const vector4_type& v) const noexcept;

        inline value_type operator[](size_t index) const noexcept
        {
            return get(index / 4, index % 4);
        }

        inline value_type get(size_t row, size_t col) const noexcept
        {
            using namespace rtm;
            underlying_vector4_type axis =
                rtm::matrix_get_axis(_value, rtm::axis4(row));
            return vector_get_component(axis, mix4(col));
        }

    public:
        inline static generic_mat4 create_look_at_rh(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return rtm::ext::look_at_rh(eye, target, up);
        }

        inline static generic_mat4 create_look_at_lh(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return rtm::ext::look_at_lh(eye, target, up);
        }

        inline static generic_mat4 create_look_at(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return create_look_at_rh(eye, target, up);
        }

    public:
        inline static generic_mat4 create_transformation(
            const underlying_vector3_type& translation,
            const underlying_quat_type& rotation,
            const underlying_vector3_type& scale) noexcept
        {
            return rtm::ext::transform_4x4(translation, rotation, scale);
        }

        // static generic_mat4 create_transformation_matrix(
        //     const vector3_type& translation, const quat& rotation,
        //     const vector3_type& scale) noexcept;

    private:
        underlying_matrix4x4_type _value;
    };

    using mat4f = generic_mat4;
    using mat4d = generic_mat4;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using mat4 = mat4d;
#else
    using mat4 = mat4f;
#endif
}  // namespace move::vectormath