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

        constexpr static uint32_t num_elements = 4;

    public:
        inline generic_quat() noexcept
            : _value(rtm::quat_normalize(rtm::quat_set(
                  value_type(0), value_type(0), value_type(0), value_type(0))))
        {
        }

        inline generic_quat(value_type x, value_type y = 0, value_type z = 0,
            value_type w = 0) noexcept
            : _value(rtm::quat_normalize((rtm::quat_set(x, y, z, w))))
        {
        }

        inline generic_quat(const quat_type& v) noexcept : _value(v)
        {
        }

    public:
        inline quat_type get_internal() const noexcept
        {
            return _value;
        }

    public:
        template <typename Archive>
        inline void serialize(Archive& ar)
        {
            for (uint32_t i = 0; i < num_elements; ++i)
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
        inline operator quat_type&()
        {
            return _value;
        }

        inline bool operator==(const generic_quat& v) const noexcept
        {
            auto lhs = rtm::quat_to_vector(_value);
            auto rhs = rtm::quat_to_vector(v._value);
            return rtm::mask_all_true(rtm::vector_equal(lhs, rhs));
        }

        inline bool operator!=(const generic_quat& v) const noexcept
        {
            auto lhs = rtm::quat_to_vector(_value);
            auto rhs = rtm::quat_to_vector(v._value);
            return !rtm::mask_all_true(rtm::vector_equal(lhs, rhs));
        }

        inline generic_quat& operator=(const generic_quat& v) noexcept
        {
            _value = v._value;
            return *this;
        }

        inline generic_quat& operator*=(const generic_quat& v) noexcept
        {
            _value = rtm::quat_mul(_value, v._value);
            return *this;
        }

        inline generic_quat operator*(const generic_quat& v) const noexcept
        {
            return generic_quat(rtm::quat_mul(_value, v._value));
        }

        inline vec3_type operator*(const vec3_type& rhs) const noexcept
        {
            return rtm::quat_mul_vector3(rhs.get_internal(), _value);
        }

        inline generic_quat operator-() const noexcept
        {
            return generic_quat(rtm::quat_neg(_value));
        }

    public:
        inline value_type get_component(int index) const noexcept
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

        inline void set_component(int index, value_type value)
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

        class component_accessor
        {
        public:
            inline component_accessor(generic_quat& vec, int index) noexcept
                : _vec(vec), _index(index)
            {
            }

            inline operator value_type() const noexcept
            {
                return _vec.get_component(_index);
            }

            inline component_accessor& operator=(const value_type& v) noexcept
            {
                _vec.set_component(_index, v);
                return *this;
            }

        private:
            generic_quat& _vec;
            int _index;
        };

        inline value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

        inline component_accessor operator[](int i) noexcept
        {
            return component_accessor(*this, i);
        }

        inline component_accessor x() noexcept
        {
            return component_accessor(*this, 0);
        }

        inline component_accessor y() noexcept
        {
            return component_accessor(*this, 1);
        }

        inline component_accessor z() noexcept
        {
            return component_accessor(*this, 2);
        }

        inline component_accessor w() noexcept
        {
            return component_accessor(*this, 3);
        }

        inline value_type x() const noexcept
        {
            return rtm::quat_get_x(_value);
        }

        inline value_type y() const noexcept
        {
            return rtm::quat_get_y(_value);
        }

        inline value_type z() const noexcept
        {
            return rtm::quat_get_z(_value);
        }

        inline value_type w() const noexcept
        {
            return rtm::quat_get_w(_value);
        }

        inline generic_quat& set_x(value_type x) noexcept
        {
            _value = rtm::vector_set_x(_value, x);
            return *this;
        }

        inline generic_quat& set_y(value_type y) noexcept
        {
            _value = rtm::vector_set_y(_value, y);
            return *this;
        }

        inline generic_quat& set_z(value_type z) noexcept
        {
            _value = rtm::vector_set_z(_value, z);
            return *this;
        }

        inline generic_quat& set_w(value_type w) noexcept
        {
            _value = rtm::vector_set_w(_value, w);
            return *this;
        }

    public:
        inline bool is_nan() const noexcept
        {
            return !rtm::quat_is_finite(_value);
        }

        inline bool is_infinite() const noexcept
        {
            return !rtm::quat_is_finite(_value);
        }

        inline bool is_identity() const noexcept
        {
            return *this == identity();
        }

        inline value_type length() const noexcept
        {
            return rtm::quat_length(_value);
        }

        inline value_type squared_length() const noexcept
        {
            return rtm::quat_length_squared(_value);
        }

        inline value_type reciprocal_length() const noexcept
        {
            return rtm::quat_length_reciprocal(_value);
        }

        inline generic_quat normalized() const noexcept
        {
            return generic_quat(rtm::quat_normalize(_value));
        }

        inline generic_quat normalized_approximate() const noexcept
        {
            // TODO: Approximate?
            return generic_quat(rtm::quat_normalize(_value));
        }

        inline value_type dot(const generic_quat& v) const noexcept
        {
            return rtm::quat_dot(_value, v._value);
        }

        inline generic_quat conjugate() const noexcept
        {
            return generic_quat(rtm::quat_conjugate(_value));
        }

        inline generic_quat inverse() const noexcept
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
        // inline generic_quat ln() const noexcept
        // {
        //     return generic_quat(rtm::ext::quat_ln(_value));
        // }

        // inline generic_quat exp() const noexcept
        // {
        //     // return generic_quat(rtm::ext::quat_exp(_value));
        // }

        inline generic_quat slerp(
            const generic_quat& v, value_type t) const noexcept
        {
            return generic_quat(rtm::quat_lerp(_value, v._value, t));
        }

        static inline generic_quat slerp(const generic_quat& v1,
            const generic_quat& v2, value_type t) noexcept
        {
            return generic_quat(rtm::quat_lerp(v1._value, v2._value, t));
        }

    public:
        inline void normalize() noexcept
        {
            _value = rtm::quat_normalize(_value);
        }

        inline void normalize_approximate() noexcept
        {
            // TODO: Approximate?
            _value = rtm::quat_normalize(_value);
        }

    public:
        static inline generic_quat identity() noexcept
        {
            return generic_quat(rtm::quat_identity());
        }

        // inline static generic_quat barycentric(const generic_quat& q0,
        //     const generic_quat& q1, const generic_quat& q2, value_type f,
        //     value_type g) noexcept
        // {
        // }

        // inline static generic_quat barycentric_v(const generic_quat& q1,
        //     const generic_quat& q2, const generic_quat& q3, value_type f,
        //     value_type g) noexcept
        // {
        // }

        static inline generic_quat from_pitch_yaw_roll(
            value_type pitch, value_type yaw, value_type roll) noexcept
        {
            return generic_quat(rtm::quat_from_euler(pitch, yaw, roll));
        }

        static inline generic_quat from_pitch_yaw_roll(
            const vec3_type& angles) noexcept
        {
            return generic_quat(
                rtm::quat_from_euler(angles.x(), angles.y(), angles.z()));
        }

        static inline generic_quat from_axis_angle(
            const vec3_type& axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(axis.get_internal(), angle));
        }

        static inline generic_quat from_rotation_normal(
            const vec3_type& normal_axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(normal_axis.get_internal(), angle));
        }

        static inline generic_quat from_rotation_axis(
            const vec3_type& axis, value_type angle) noexcept
        {
            return generic_quat(
                rtm::quat_from_axis_angle(axis.get_internal(), angle));
        }

        static inline generic_quat rotation_matrix_to_quaternion(
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

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
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