#pragma once
#include <ostream>

#include "macros.hpp"
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

namespace move::vectormath
{
    template <typename value_type, typename vector3_type_raw,
        typename vector4_type_raw, typename quat_type_raw,
        typename matrix3x4_type_raw, typename matrix4x4_type_raw>
    struct generic_mat4_rtm
    {
        using component_type = value_type;
        using underlying_vector3_type = typename vector3_type_raw::type;
        using underlying_vector4_type = typename vector4_type_raw::type;
        using underlying_quat_type = typename quat_type_raw::type;
        using underlying_matrix3x4_type = typename matrix3x4_type_raw::type;
        using underlying_matrix4x4_type = typename matrix4x4_type_raw::type;
        using vec3_type = generic_vec3_rtm<value_type, vector3_type_raw>;
        using vec4_type = generic_vec4_rtm<value_type, vector4_type_raw>;

        MVM_INLINE generic_mat4_rtm() noexcept : _value()
        {
        }

        MVM_INLINE generic_mat4_rtm(value_type x00, value_type x01,
            value_type x02, value_type x03, value_type x10, value_type x11,
            value_type x12, value_type x13, value_type x20, value_type x21,
            value_type x22, value_type x23, value_type x30, value_type x31,
            value_type x32, float x33) noexcept
            : _value(rtm::matrix_set(rtm::vector_set(x00, x01, x02, x03),
                  rtm::vector_set(x10, x11, x12, x13),
                  rtm::vector_set(x20, x21, x22, x23),
                  rtm::vector_set(x30, x31, x32, x33)))
        {
        }

        MVM_INLINE generic_mat4_rtm(const underlying_vector4_type& row0,
            const underlying_vector4_type& row1,
            const underlying_vector4_type& row2,
            const underlying_vector4_type& row3) noexcept
            : _value(rtm::matrix_set(row0, row1, row2, row3))
        {
        }

        MVM_INLINE generic_mat4_rtm(
            const underlying_matrix4x4_type& rhs) noexcept
            : _value(rhs)
        {
        }

    public:
        MVM_INLINE_NODISCARD const underlying_matrix4x4_type& get_internal()
            const noexcept
        {
            return _value;
        }

        MVM_INLINE_NODISCARD underlying_matrix4x4_type& get_internal() noexcept
        {
            return _value;
        }

    public:
        /* @brief Serializes the matrix to/from an archive.  Intended for use
         * with Cereal and Cereal-like APIs.
         *
         * @tparam Archive The archive type
         * @param ar The archive
         */
        template <typename Archive>
        void serialize(Archive& archive)
        {
            // if this is a write archive
            if constexpr (Archive::is_saving::value)
            {
                for (size_t i = 0; i < 4; ++i)
                {
                    for (size_t j = 0; j < 4; ++j)
                    {
                        archive(get_component(i, j));
                    }
                }
            }
            else
            {
                value_type values[16];
                for (size_t i = 0; i < 16; ++i)
                {
                    archive(values[i]);
                }
                set(values);
            }
        }

    public:
        MVM_INLINE_NODISCARD operator underlying_matrix4x4_type&() noexcept
        {
            return _value;
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm operator*(
            const generic_mat4_rtm& m) const noexcept
        {
            using namespace ::rtm;
            return generic_mat4_rtm(matrix_mul(_value, m._value));
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& operator*=(
            const generic_mat4_rtm& m) noexcept
        {
            using namespace ::rtm;
            _value = matrix_mul(_value, m._value);
            return *this;
        }

        MVM_INLINE void to_data_array(value_type* value)
        {
            using namespace rtm;
            vector_store(matrix_get_axis(_value, axis4::x), value);
            vector_store(matrix_get_axis(_value, axis4::y), value + 4);
            vector_store(matrix_get_axis(_value, axis4::z), value + 8);
            vector_store(matrix_get_axis(_value, axis4::w), value + 12);
        }

        MVM_INLINE_NODISCARD value_type operator[](size_t index) const noexcept
        {
            return get_component(index / 4, index % 4);
        }

        MVM_INLINE_NODISCARD value_type get_component(
            size_t index) const noexcept
        {
            return get_component(index / 4, index % 4);
        }

        MVM_INLINE_NODISCARD value_type get_component(
            size_t row, size_t col) const noexcept
        {
            using namespace rtm;
            underlying_vector4_type axis =
                rtm::matrix_get_axis(_value, rtm::axis4(row));
            return vector_get_component(axis, mix4(col));
        }

        MVM_INLINE_NODISCARD vec4_type get_axis(size_t row) noexcept
        {
            return vec4_type(get_axis_internal(row));
        }

        MVM_INLINE_NODISCARD underlying_vector4_type get_axis_internal(
            size_t row) noexcept
        {
            using namespace rtm;
            return rtm::matrix_get_axis(_value, rtm::axis4(row));
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& set_axis(
            size_t row, const vec4_type& vec) noexcept
        {
            return set_axis(row, vec.get_internal());
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& set_axis(
            size_t row, const underlying_vector4_type& axis) noexcept
        {
            // TODO: Make this faster?  Seems like there should just be a
            // matrix_set_axis call, but maybe I'm missing some performance
            // implication there.
            using namespace rtm;
            underlying_vector4_type axes[] = {
                rtm::matrix_get_axis(_value, rtm::axis4(0)),
                rtm::matrix_get_axis(_value, rtm::axis4(1)),
                rtm::matrix_get_axis(_value, rtm::axis4(2)),
                rtm::matrix_get_axis(_value, rtm::axis4(3)),
            };
            axes[row] = axis;
            _value = matrix_set(axes[0], axes[1], axes[2], axes[3]);
            return *this;
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& set_component(
            size_t row, size_t col, value_type value) noexcept
        {
            using namespace rtm;
            return set_axis(row,
                vector_set_component(get_axis_internal(row), mix4(col), value));
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& set(value_type x00,
            value_type x01, value_type x02, value_type x03, value_type x10,
            value_type x11, value_type x12, value_type x13, value_type x20,
            value_type x21, value_type x22, value_type x23, value_type x30,
            value_type x31, value_type x32, value_type x33) noexcept
        {
            using namespace rtm;
            _value = matrix_set(vector_set(x00, x01, x02, x03),
                vector_set(x10, x11, x12, x13), vector_set(x20, x21, x22, x23),
                vector_set(x30, x31, x32, x33));
            return *this;
        }

        MVM_INLINE_NODISCARD generic_mat4_rtm& set(const value_type values[16])
        {
            using namespace rtm;
            _value = matrix_set(
                vector_set(values[0], values[1], values[2], values[3]),
                vector_set(values[4], values[5], values[6], values[7]),
                vector_set(values[8], values[9], values[10], values[11]),
                vector_set(values[12], values[13], values[14], values[15]));
            return *this;
        }

    public:
        MVM_INLINE_NODISCARD generic_mat4_rtm inverse()
        {
            return generic_mat4_rtm(rtm::matrix_inverse(_value));
        }

    public:
        MVM_INLINE_NODISCARD static generic_mat4_rtm create_from_array(
            const value_type values[16])
        {
            return generic_mat4_rtm(rtm::matrix_set(
                rtm::vector_set(values[0], values[1], values[2], values[3]),
                rtm::vector_set(values[4], values[5], values[6], values[7]),
                rtm::vector_set(values[8], values[9], values[10], values[11]),
                rtm::vector_set(
                    values[12], values[13], values[14], values[15])));
            {
            }
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_identity() noexcept
        {
            return generic_mat4_rtm(rtm::matrix_identity());
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_rotation_x(
            value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(1.0), value_type(0.0f), value_type(0.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_rotation_y(
            value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(0.0), value_type(1.0f), value_type(0.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_rotation_z(
            value_type angle) noexcept
        {
            using namespace rtm;
            auto quat = quat_from_axis_angle(
                vector_set(value_type(0.0), value_type(0.0f), value_type(1.0f),
                    value_type(0.0f)),
                angle);

            return underlying_matrix4x4_type(
                matrix_cast<underlying_matrix3x4_type>(matrix_from_quat(quat)));
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_translation(
            const underlying_vector4_type& translation) noexcept
        {
            using rtm::vector_set;
            return {vector_set(value_type(1), value_type(0), value_type(0),
                        value_type(0)),
                vector_set(
                    value_type(0), value_type(1), value_type(0), value_type(0)),
                vector_set(
                    value_type(0), value_type(0), value_type(1), value_type(0)),
                translation};
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_translation(
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

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_scale(
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

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_scale(
            const vec3_type& scale) noexcept
        {
            using rtm::vector_set;
            return {
                vector_set(
                    scale.x(), value_type(0), value_type(0), value_type(0)),
                vector_set(
                    value_type(0), scale.y(), value_type(0), value_type(0)),
                vector_set(
                    value_type(0), value_type(0), scale.z(), value_type(0)),
                vector_set(
                    value_type(0), value_type(0), value_type(0), value_type(1)),
            };
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_perspective_rh(
            value_type fov, value_type aspect, value_type near,
            value_type far) noexcept
        {
            return rtm::ext::perspective_fov_rh(fov, aspect, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_perspective_lh(
            value_type fov, value_type aspect, value_type near,
            value_type far) noexcept
        {
            return rtm::ext::perspective_fov_lh(fov, aspect, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_perspective(
            value_type fov, value_type aspect, value_type near,
            value_type far) noexcept
        {
            return create_perspective_rh(fov, aspect, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_ortho_rh(
            value_type width, value_type height, value_type near,
            value_type far) noexcept
        {
            return rtm::ext::ortho_rh(width, height, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_ortho_lh(
            value_type width, value_type height, value_type near,
            value_type far) noexcept
        {
            return rtm::ext::ortho_lh(width, height, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_ortho(
            value_type width, value_type height, value_type near,
            value_type far) noexcept
        {
            return create_ortho_rh(width, height, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_ortho_off_center_rh(
            value_type left, value_type right, value_type bottom,
            value_type top, value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_off_center_rh(
                left, right, bottom, top, near, far);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_ortho_off_center_lh(
            value_type left, value_type right, value_type bottom,
            value_type top, value_type near, value_type far) noexcept
        {
            return rtm::ext::ortho_off_center_lh(
                left, right, bottom, top, near, far);
        }

    public:
        MVM_INLINE_NODISCARD static generic_mat4_rtm create_look_at_rh(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return rtm::ext::look_at_rh<underlying_matrix4x4_type>(
                eye, target, up);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_look_at_lh(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return rtm::ext::look_at_lh<underlying_matrix4x4_type>(
                eye, target, up);
        }

        MVM_INLINE_NODISCARD static generic_mat4_rtm create_look_at(
            const underlying_vector3_type& eye,
            const underlying_vector3_type& target,
            const underlying_vector3_type& up) noexcept
        {
            return create_look_at_rh(eye, target, up);
        }

    public:
        MVM_INLINE_NODISCARD static generic_mat4_rtm create_transformation(
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

    template <typename matrix_type>
    MVM_INLINE_NODISCARD static typename matrix_type::vec4_type operator*(
        const typename matrix_type::vec4_type& v, const matrix_type& m) noexcept
    {
        using namespace rtm;
        return matrix_mul_vector(v.get_internal(), m.get_internal());
    }

    template <typename matrix_type>
    MVM_INLINE_NODISCARD static typename matrix_type::vec4_type operator*(
        const matrix_type& m, const typename matrix_type::vec4_type& v) noexcept
    {
        (void)m;
        (void)v;
        static_assert(sizeof(matrix_type) == 0, "Vector must be on the left");
    }

    using mat4f = generic_mat4_rtm<float, wrappers::v4f, wrappers::v4f,
        wrappers::qf, wrappers::m3x4f, wrappers::m4x4f>;
    using mat4d = generic_mat4_rtm<double, wrappers::v4d, wrappers::v4d,
        wrappers::qd, wrappers::m3x4d, wrappers::m4x4d>;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using mat4 = mat4d;
#else
    using mat4 = mat4f;
#endif
}  // namespace move::vectormath