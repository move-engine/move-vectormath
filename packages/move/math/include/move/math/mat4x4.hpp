#pragma once
#include <type_traits>

#include <rtm/impl/matrix_common.h>
#include <rtm/matrix4x4d.h>
#include <rtm/matrix4x4f.h>
#include <rtm/types.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_ext.hpp>

#include <move/math/mat3x4.hpp>
#include <move/math/quat.hpp>
#include <move/math/vec3.hpp>
#include <move/math/vec4.hpp>

namespace move::math
{
    namespace simd_rtm::detail
    {
        MVM_TYPE_WRAPPER(m4x4f, rtm::matrix4x4f)
        MVM_TYPE_WRAPPER(m4x4d, rtm::matrix4x4d)

        template <typename T>
        using m4x4 = std::conditional_t<std::is_same_v<T, float>, m4x4f, m4x4d>;
    }  // namespace simd_rtm::detail

    // mat4 always uses RTM under the hood
    template <typename T, typename wrapper_type = simd_rtm::detail::m4x4<T>>
        requires std::is_floating_point_v<T>
    struct mat4x4
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

    private:
        using underlying_type = wrapper_type::type;
        underlying_type _value;

        template <typename component_type, Acceleration OtherAccel>
        friend vec4<component_type, OtherAccel> operator*(
            const vec4<component_type, OtherAccel>& vec,
            const mat4x4<component_type>& mat);

    public:
        using rtm_vec4_t = typename simd_rtm::detail::v4<T>::type;
        using rtm_mat3x4_t = typename simd_rtm::detail::m3x4<T>::type;
        using rtm_mat4x4_t = underlying_type;
        using vec3_t = vec3<T, acceleration>;
        using vec4_t = vec4<T, acceleration>;
        using quat_t = quat<T>;
        using component_type = T;

    public:
        // Constructors
        MVM_INLINE mat4x4() : _value(rtm::matrix_identity())
        {
        }

        MVM_INLINE mat4x4(const underlying_type& data) : _value(data)
        {
        }

        MVM_INLINE mat4x4(const mat4x4& other) : _value(other._value)
        {
        }

        MVM_INLINE mat4x4(const T& _11,
                          const T& _12,
                          const T& _13,
                          const T& _14,
                          const T& _21,
                          const T& _22,
                          const T& _23,
                          const T& _24,
                          const T& _31,
                          const T& _32,
                          const T& _33,
                          const T& _34,
                          const T& _41,
                          const T& _42,
                          const T& _43,
                          const T& _44) :
            _value(rtm::matrix_set(rtm::vector_set(_11, _12, _13, _14),
                                   rtm::vector_set(_21, _22, _23, _24),
                                   rtm::vector_set(_31, _32, _33, _34),
                                   rtm::vector_set(_41, _42, _43, _44)))
        {
        }

        MVM_INLINE mat4x4& operator=(const mat4x4& other)
        {
            _value = other._value;
            return *this;
        }

        // Pointers
    public:
        MVM_INLINE void store_array(T* out) const
        {
            using namespace rtm;
            for (uint8_t i = 0; i < 4; i++)
            {
                vector_store(matrix_get_axis(_value, (axis4)i), out + i * 4);
            }
        }

        MVM_INLINE void load_array(const T* in)
        {
            using namespace rtm;
            vector4f x = vector_load(in);
            vector4f y = vector_load(in + 4);
            vector4f z = vector_load(in + 8);
            vector4f w = vector_load(in + 12);
            _value = matrix_set(x, y, z, w);
        }

        MVM_INLINE_NODISCARD underlying_type to_rtm()
        {
            return _value;
        }

        MVM_INLINE_NODISCARD static mat4x4 from_rtm(const underlying_type& data)
        {
            return data;
        }

        // Arithmetic operations
    public:
        MVM_INLINE_NODISCARD mat4x4 operator*(const mat4x4& other) const
        {
            return mat4x4(rtm::matrix_mul(_value, other._value));
        }

        MVM_INLINE_NODISCARD vec3_t transform_point(const vec3_t& rhs) const
        {
            rtm_vec4_t mul = rtm::vector_set_w(rhs.to_rtm(), component_type(1));
            rtm_vec4_t res = rtm::vector_mul(
                mul, rtm::matrix_get_axis(_value, rtm::axis4::x));
            return vec3_t::from_rtm(res);
        }

        MVM_INLINE_NODISCARD vec3_t transform_vector(const vec3_t& rhs) const
        {
            rtm_vec4_t mul = rtm::vector_set_w(rhs.to_rtm(), component_type(0));
            rtm_vec4_t res = rtm::vector_mul(
                mul, rtm::matrix_get_axis(_value, rtm::axis4::x));
            return vec3_t::from_rtm(res);
        }

        MVM_INLINE_NODISCARD vec4_t transform_vector4(const vec4_t& rhs) const
        {
            rtm_vec4_t mul = rhs.to_rtm();
            rtm_vec4_t res = rtm::vector_mul(
                mul, rtm::matrix_get_axis(_value, rtm::axis4::x));
            return vec3_t::from_rtm(res);
        }

        // Stream overload operators
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const mat4x4& vec)
        {
            os << move::meta::type_name<mat4x4>() << "(";
            for (uint8_t i = 0; i < 4; i++)
            {
                auto row = rtm::matrix_get_axis(vec._value, (rtm::axis4)i);
                os << "(" << rtm::vector_get_x(row) << ", "
                   << rtm::vector_get_y(row) << ", " << rtm::vector_get_z(row)
                   << ", " << rtm::vector_get_w(row) << ")";
                if (i < 3)
                {
                    os << ", ";
                }
            }
            os << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator==(const mat4x4& other) const
        {
            using namespace rtm;

            // Load all rows
            auto r1 = rtm::matrix_get_axis(_value, axis4::x);
            auto r2 = rtm::matrix_get_axis(_value, axis4::y);
            auto r3 = rtm::matrix_get_axis(_value, axis4::z);
            auto r4 = rtm::matrix_get_axis(_value, axis4::w);

            // Load all rows of the other matrix
            auto o1 = rtm::matrix_get_axis(other._value, axis4::x);
            auto o2 = rtm::matrix_get_axis(other._value, axis4::y);
            auto o3 = rtm::matrix_get_axis(other._value, axis4::z);
            auto o4 = rtm::matrix_get_axis(other._value, axis4::w);

            // Compare all rows
            return rtm::mask_all_true(rtm::vector_equal(r1, o1)) &&
                   rtm::mask_all_true(rtm::vector_equal(r2, o2)) &&
                   rtm::mask_all_true(rtm::vector_equal(r3, o3)) &&
                   rtm::mask_all_true(rtm::vector_equal(r4, o4));
        }

        MVM_INLINE_NODISCARD bool operator!=(const mat4x4& other) const
        {
            return !(*this == other);
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[16];
            if constexpr (Archive::is_loading::value)
            {
                archive(data);
                load_array(data);
            }
            else
            {
                store_array(data);
                archive(data);
            }
        }

        // Mathematical operations
    public:
        MVM_INLINE_NODISCARD mat4x4 inverse() const
        {
            return rtm::matrix_inverse(_value);
        }

        MVM_INLINE_NODISCARD mat4x4 transposed() const
        {
            return rtm::matrix_transpose(_value);
        }

        MVM_INLINE_NODISCARD T determinant() const
        {
            T res;
            rtm::scalar_store(rtm::matrix_determinant(_value), &res);
            return res;
        }

        // Mutators
    public:
        MVM_INLINE mat4x4& fill(const T& val)
        {
            _value = rtm::matrix_set(val);
            return *this;
        }

        MVM_INLINE mat4x4& invert_in_place()
        {
            _value = rtm::matrix_inverse(_value);
            return *this;
        }

        MVM_INLINE mat4x4& transpose_in_place()
        {
            _value = rtm::matrix_transpose(_value);
            return *this;
        }

        // Statics
    public:
        MVM_INLINE_NODISCARD static mat4x4 identity()
        {
            return mat4x4(rtm::matrix_identity());
        }

        MVM_INLINE_NODISCARD static mat4x4 filled(const T& val)
        {
            return mat4x4(
                rtm::matrix_set(rtm::vector_set(val), rtm::vector_set(val),
                                rtm::vector_set(val), rtm::vector_set(val)));
        }

        MVM_INLINE_NODISCARD static mat4x4 zero()
        {
            return filled(T(0));
        }

        MVM_INLINE_NODISCARD static mat4x4 one()
        {
            return filled(T(1));
        }

        MVM_INLINE_NODISCARD static mat4x4 infinity()
        {
            return filled(std::numeric_limits<T>::infinity());
        }

        MVM_INLINE_NODISCARD static mat4x4 negative_infinity()
        {
            return filled(-std::numeric_limits<T>::infinity());
        }

        MVM_INLINE_NODISCARD static mat4x4 nan()
        {
            return filled(std::numeric_limits<T>::quiet_NaN());
        }

        // Transformation matrix helpers
    public:
        MVM_INLINE_NODISCARD static mat4x4 translation(
            const vec3_t& translation) noexcept
        {
            typename simd_rtm::detail::v4<T>::type translation_row =
                rtm::vector_set_w(translation.to_rtm(), component_type(1));

            return mat4x4(rtm::matrix_set(
                rtm::vector_set(component_type(1), component_type(0),
                                component_type(0), component_type(0)),
                rtm::vector_set(component_type(0), component_type(1),
                                component_type(0), component_type(0)),
                rtm::vector_set(component_type(0), component_type(0),
                                component_type(1), component_type(0)),
                translation_row));
        }

        MVM_INLINE_NODISCARD static mat4x4 rotation(const quat_t& quat)
        {
            using namespace rtm;
            return rtm_mat4x4_t(
                matrix_cast<rtm_mat3x4_t>(matrix_from_quat(quat.to_rtm())));
        }

        /**
         * @brief Creates a rotation matrix from an axis and an angle.
         *
         * @param axis The axis to rotate around
         * @param angle The angle in radians to rotate by
         * @return mat4 The rotation matrix
         */
        MVM_INLINE_NODISCARD static mat4x4 angle_axis(const vec3_t& axis,
                                                      const T& angle)
        {
            using namespace rtm;

            auto quat = quat_from_axis_angle(axis.to_rtm(), angle);
            return rtm_mat4x4_t(
                matrix_cast<rtm_mat3x4_t>(matrix_from_quat(quat)));
        }

        MVM_INLINE_NODISCARD static mat4x4 scale(component_type x,
                                                 component_type y,
                                                 component_type z) noexcept
        {
            using rtm::vector_set;
            return mat4x4(rtm::matrix_set(
                vector_set(x, component_type(0), component_type(0),
                           component_type(0)),
                vector_set(component_type(0), y, component_type(0),
                           component_type(0)),
                vector_set(component_type(0), component_type(0), z,
                           component_type(0)),
                vector_set(component_type(0), component_type(0),
                           component_type(0), component_type(1))));
        }

        MVM_INLINE_NODISCARD static mat4x4 scale(const vec3_t& scale) noexcept
        {
            using rtm::vector_set;
            component_type loaded[3];
            scale.store_array(loaded);
            return mat4x4::scale(loaded[0], loaded[1], loaded[2]);
        }

        /**
         * @brief Creates a TRS matrix.  Identical to `scale * rotation *
         * translation`, but faster.
         *
         * @param translation The translation vector
         * @param rotation The rotation quaternion
         * @param scale The scale vector
         * @return mat4 The TRS matrix
         */
        MVM_INLINE_NODISCARD static mat4x4 trs(const vec3_t& translation,
                                               const quat_t& rotation,
                                               const vec3_t& scale) noexcept
        {
            return rtm::ext::transform_4x4(translation.to_rtm(),
                                           rotation.to_rtm(), scale.to_rtm());
        }

        // Camera matrix helpers
    public:
        MVM_INLINE_NODISCARD static mat4x4 look_at(const vec3_t& eye,
                                                   const vec3_t& target,
                                                   const vec3_t& up) noexcept
        {
            return rtm::ext::look_at_lh(eye.to_rtm(), target.to_rtm(),
                                        up.to_rtm());
        }

        MVM_INLINE_NODISCARD static mat4x4 perspective(const T& fov,
                                                       const T& aspect,
                                                       const T& near,
                                                       const T& far) noexcept
        {
            return rtm::ext::perspective_fov_lh(fov, aspect, near, far);
        }

        MVM_INLINE_NODISCARD static mat4x4 orthographic(const T& width,
                                                        const T& height,
                                                        const T& near,
                                                        const T& far) noexcept
        {
            return rtm::ext::ortho_lh(width, height, near, far);
        }

        MVM_INLINE_NODISCARD static mat4x4 orthographic(const T& left,
                                                        const T& right,
                                                        const T& bottom,
                                                        const T& top,
                                                        const T& near,
                                                        const T& far) noexcept
        {
            return rtm::ext::ortho_off_center_lh(left, right, bottom, top, near,
                                                 far);
        }
    };

    // Multiplication operator.  Row-major order.
    template <typename component_type, Acceleration OtherAccel>
    MVM_INLINE_NODISCARD vec4<component_type, OtherAccel> operator*(
        const vec4<component_type, OtherAccel>& vec,
        const mat4x4<component_type>& mat)
    {
        using namespace rtm;
        using Accel = move::math::Acceleration;
        using vector_type = simd_rtm::detail::v4<component_type>::type;

        vector_type v = vec.to_rtm();
        vector_type result = matrix_mul_vector(v, mat._value);
        return vec4<component_type, OtherAccel>::from_rtm(result);
    }

    using float4x4 = mat4x4<float>;
    using double4x4 = mat4x4<double>;

    template <typename T>
    MVM_INLINE_NODISCARD bool approx_equal(
        const mat4x4<T>& a,
        const mat4x4<T>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        T aloaded[16];
        T bloaded[16];

        a.store_array(aloaded);
        b.store_array(bloaded);

        for (uint8_t i = 0; i < 16; i++)
        {
            if (!approx_equal(aloaded[i], bloaded[i], epsilon))
            {
                return false;
            }
        }
        return true;
    }
}  // namespace move::math