#pragma once
#include <ostream>

#include "move/vectormath/mat4.hpp"
#include "rtm/quatf.h"
#include "underlying_types.hpp"

#include <rtm/mask4d.h>
#include <rtm/mask4f.h>
#include <rtm/scalard.h>
#include <rtm/types.h>
#include <rtm/vector4d.h>
#include <rtm/vector4f.h>

#include <fmt/format.h>

#include <move/vectormath/vec3.hpp>

namespace move::vectormath
{
    // using value_type = float;
    // using quat_type_raw = wrappers::qf;
    // using underlying_matrix3x4_type_raw = wrappers::m3x4f;
    // using underlying_matrix4x4_type_raw = wrappers::m4x4f;
    // using vec3_type = vec3f;
    // using mat4x4_type = mat4f;

    template <typename value_type, typename quat_type_raw,
        typename underlying_matrix3x4_type_raw,
        typename underlying_matrix4x4_type_raw, typename vec3_type,
        typename mat4x4_type>
    struct generic_quat
    {
        using component_type = value_type;
        using quat_type = typename quat_type_raw::type;
        using underlying_matrix3x4_type =
            typename underlying_matrix3x4_type_raw::type;
        using underlying_matrix4x4_type =
            typename underlying_matrix4x4_type_raw::type;

        constexpr static uint32_t num_components = 4;

    public:
        MVM_INLINE_NODISCARD generic_quat() noexcept
            : _value(rtm::quat_normalize(rtm::quat_set(
                  value_type(0), value_type(0), value_type(0), value_type(0))))
        {
        }

        MVM_INLINE_NODISCARD generic_quat(value_type x, value_type y = 0,
            value_type z = 0, value_type w = 0) noexcept
            : _value(rtm::quat_normalize((rtm::quat_set(x, y, z, w))))
        {
        }

        MVM_INLINE_NODISCARD generic_quat(const quat_type& v) noexcept
            : _value(v)
        {
        }

    public:
        MVM_INLINE_NODISCARD quat_type get_internal() const noexcept
        {
            return _value;
        }

    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& ar)
        {
            for (uint32_t i = 0; i < num_components; ++i)
            {
                /* If is reading */
                if constexpr (Archive::is_loading::value)
                {
                    value_type val;
                    ar(val);
                    set_component(i, val);
                }
                else
                {
                    ar(get_component(i));
                }
            }
        }

    public:
        MVM_INLINE_NODISCARD operator quat_type&()
        {
            return _value;
        }

        MVM_INLINE_NODISCARD bool operator==(
            const generic_quat& v) const noexcept
        {
            auto lhs = rtm::quat_to_vector(_value);
            auto rhs = rtm::quat_to_vector(v._value);
            return rtm::mask_all_true(rtm::vector_equal(lhs, rhs));
        }

        MVM_INLINE_NODISCARD bool operator!=(
            const generic_quat& v) const noexcept
        {
            auto lhs = rtm::quat_to_vector(_value);
            auto rhs = rtm::quat_to_vector(v._value);
            return !rtm::mask_all_true(rtm::vector_equal(lhs, rhs));
        }

        MVM_INLINE_NODISCARD generic_quat& operator=(
            const generic_quat& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        MVM_INLINE_NODISCARD generic_quat& operator*=(
            const generic_quat& v) noexcept
        {
            _value = rtm::quat_mul(_value, v._value);
            return *this;
        }

        MVM_INLINE_NODISCARD generic_quat operator*(
            const generic_quat& v) const noexcept
        {
            return generic_quat(rtm::quat_mul(_value, v._value));
        }

        MVM_INLINE_NODISCARD vec3_type operator*(
            const vec3_type& rhs) const noexcept
        {
            return rtm::quat_mul_vector3(rhs.get_internal(), _value);
        }

        MVM_INLINE_NODISCARD generic_quat operator-() const noexcept
        {
            return generic_quat(rtm::quat_neg(_value));
        }

    public:
        MVM_INLINE_NODISCARD value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return value_type(rtm::quat_get_x(_value));
                case 1:
                    return value_type(rtm::quat_get_y(_value));
                case 2:
                    return value_type(rtm::quat_get_z(_value));
                default:
                    return value_type(rtm::quat_get_w(_value));
            }
        }

        MVM_INLINE void set_component(int index, value_type value)
        {
            switch (index)
            {
                case 0:
                    set_x(value);
                    break;
                case 1:
                    set_y(value);
                    break;
                case 2:
                    set_z(value);
                    break;
                default:
                    set_w(value);
                    break;
            }
        }

        MVM_INLINE_NODISCARD value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        MVM_INLINE_NODISCARD value_type x() const noexcept
        {
            return rtm::quat_get_x(_value);
        }

        MVM_INLINE_NODISCARD value_type y() const noexcept
        {
            return rtm::quat_get_y(_value);
        }

        MVM_INLINE_NODISCARD value_type z() const noexcept
        {
            return rtm::quat_get_z(_value);
        }

        MVM_INLINE_NODISCARD value_type w() const noexcept
        {
            return rtm::quat_get_w(_value);
        }

        MVM_INLINE_NODISCARD generic_quat& x(value_type value) const noexcept
        {
            return rtm::quat_set_x(_value, value);
        }

        MVM_INLINE_NODISCARD generic_quat& y(value_type value) const noexcept
        {
            return rtm::quat_set_y(_value, value);
        }

        MVM_INLINE_NODISCARD generic_quat& z(value_type value) const noexcept
        {
            return rtm::quat_set_z(_value, value);
        }

        MVM_INLINE_NODISCARD generic_quat& w(value_type value) const noexcept
        {
            return rtm::quat_set_w(_value, value);
        }

        MVM_INLINE_NODISCARD generic_quat& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        MVM_INLINE_NODISCARD generic_quat& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        MVM_INLINE_NODISCARD generic_quat& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        MVM_INLINE_NODISCARD generic_quat& set_w(value_type w) noexcept
        {
            _value = rtm::vector_set_w(_value, w);
            return *this;
        }

    public:
        MVM_INLINE_NODISCARD bool is_nan() const noexcept
        {
            return !rtm::quat_is_finite(_value);
        }

        MVM_INLINE_NODISCARD bool is_infinite() const noexcept
        {
            return !rtm::quat_is_finite(_value);
        }

        MVM_INLINE_NODISCARD bool is_identity() const noexcept
        {
            return *this == identity();
        }

        MVM_INLINE_NODISCARD value_type length() const noexcept
        {
            return rtm::quat_length(_value);
        }

        MVM_INLINE_NODISCARD value_type squared_length() const noexcept
        {
            return rtm::quat_length_squared(_value);
        }

        MVM_INLINE_NODISCARD value_type reciprocal_length() const noexcept
        {
            return rtm::quat_length_reciprocal(_value);
        }

        MVM_INLINE_NODISCARD generic_quat normalized() const noexcept
        {
            return generic_quat(rtm::quat_normalize(_value));
        }

        MVM_INLINE_NODISCARD generic_quat
        normalized_approximate() const noexcept
        {
            // TODO: Approximate?
            return generic_quat(rtm::quat_normalize(_value));
        }

        MVM_INLINE_NODISCARD value_type dot(
            const generic_quat& v) const noexcept
        {
            return rtm::quat_dot(_value, v._value);
        }

        MVM_INLINE_NODISCARD generic_quat conjugate() const noexcept
        {
            return generic_quat(rtm::quat_conjugate(_value));
        }

        MVM_INLINE_NODISCARD generic_quat inverse() const noexcept
        {
            // XMVECTOR L = XMVector4LengthSq(Q);
            // XMVECTOR Conjugate = XMQuaternionConjugate(Q);

            // XMVECTOR Control = XMVectorLessOrEqual(L, g_XMEpsilon.v);

            // XMVECTOR Result = XMVectorDivide(Conjugate, L);

            // Result = XMVectorSelect(Result, g_XMZero, Control);

            // return Result;
            return generic_quat(rtm::ext::quat_inverse(_value));
        }

        // TODO
        // MVM_INLINE_NODISCARD generic_quat ln() const noexcept
        // {
        //     return generic_quat(rtm::ext::quat_ln(_value));
        // }

        // MVM_INLINE_NODISCARD generic_quat exp() const noexcept
        // {
        //     // return generic_quat(rtm::ext::quat_exp(_value));
        // }

        MVM_INLINE_NODISCARD generic_quat slerp(
            const generic_quat& v, value_type t) const noexcept
        {
            return generic_quat(rtm::quat_lerp(_value, v._value, t));
        }

        MVM_INLINE_NODISCARD static generic_quat slerp(const generic_quat& v1,
            const generic_quat& v2, value_type t) noexcept
        {
            return generic_quat(rtm::quat_lerp(v1._value, v2._value, t));
        }

    public:
        MVM_INLINE void normalize() noexcept
        {
            _value = rtm::quat_normalize(_value);
        }

    public:
        MVM_INLINE_NODISCARD static generic_quat identity() noexcept
        {
            return generic_quat(rtm::quat_identity());
        }

        // MVM_INLINE_NODISCARD static generic_quat barycentric(const
        // generic_quat& q0,
        //     const generic_quat& q1, const generic_quat& q2, value_type f,
        //     value_type g) noexcept
        // {
        // }

        // MVM_INLINE_NODISCARD static generic_quat barycentric_v(const
        // generic_quat& q1,
        //     const generic_quat& q2, const generic_quat& q3, value_type f,
        //     value_type g) noexcept
        // {
        // }

        MVM_INLINE_NODISCARD static generic_quat from_pitch_yaw_roll(
            value_type pitch, value_type yaw, value_type roll) noexcept
        {
            return generic_quat(rtm::quat_from_euler(pitch, yaw, roll));
        }

        MVM_INLINE_NODISCARD static generic_quat from_pitch_yaw_roll(
            const vec3_type& angles) noexcept
        {
            return generic_quat(
                rtm::quat_from_euler(angles.x(), angles.y(), angles.z()));
        }

        MVM_INLINE_NODISCARD static generic_quat from_axis_angle(
            const vec3_type& axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(axis.get_internal(), angle));
        }

        MVM_INLINE_NODISCARD static generic_quat from_rotation_normal(
            const vec3_type& normal_axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(normal_axis.get_internal(), angle));
        }

        MVM_INLINE_NODISCARD static generic_quat from_rotation_axis(
            const vec3_type& axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(axis.get_internal(), angle));
        }

        MVM_INLINE_NODISCARD static generic_quat rotation_matrix_to_quaternion(
            const mat4x4_type& matrix) noexcept
        {
            underlying_matrix3x4_type matrix3x4 =
                matrix_cast(matrix.get_internal());
            return generic_quat(rtm::quat_from_matrix(matrix3x4));
        }

    private:
        quat_type _value;
    };

    using quatf = generic_quat<float, wrappers::qf, wrappers::m3x4f,
        wrappers::m4x4f, vec3f, mat4f>;
    using quatd = generic_quat<double, wrappers::qd, wrappers::m3x4d,
        wrappers::m4x4d, vec3d, mat4d>;

#if defined(MOVE_VECTORMATH_USE_DOUBLE_PRECISION)
    using quat = quatd;
#else
    using quat = quatf;
#endif

    template <typename value_type, typename quat_type_raw,
        typename underlying_matrix3x4_type_raw,
        typename underlying_matrix4x4_type_raw, typename vec3_type,
        typename mat4x4_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_quat<value_type, quat_type_raw,
            underlying_matrix3x4_type_raw, underlying_matrix4x4_type_raw,
            vec3_type, mat4x4_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ", " << v.z() << ", "
                  << v.w() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::quatf);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::quatd);
#endif

template <typename value_type, typename quat_type_raw,
    typename underlying_matrix3x4_type_raw,
    typename underlying_matrix4x4_type_raw, typename vec3_type,
    typename mat4x4_type>
struct fmt::formatter<move::vectormath::generic_quat<value_type, quat_type_raw,
    underlying_matrix3x4_type_raw, underlying_matrix4x4_type_raw, vec3_type,
    mat4x4_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_quat<value_type, quat_type_raw,
            underlying_matrix3x4_type_raw, underlying_matrix4x4_type_raw,
            vec3_type, mat4x4_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {}, {}, {})", number.x(), number.y(),
            number.z(), number.w());
    }
};