#pragma once
#include <type_traits>

#include <rtm/impl/matrix_common.h>
#include <rtm/matrix3x3d.h>
#include <rtm/matrix3x3f.h>
#include <rtm/types.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_common.hpp>
#include <move/math/rtm/rtm_ext.hpp>
#include "move/math/quat.hpp"
#include "move/math/vec3.hpp"
#include "move/math/vec4.hpp"

namespace move::math
{
    namespace simd_rtm::detail
    {
        MVM_TYPE_WRAPPER(m3x3f, rtm::matrix3x3f)
        MVM_TYPE_WRAPPER(m3x3d, rtm::matrix3x3d)

        template <typename T>
        using m3x3 = std::conditional_t<std::is_same_v<T, float>, m3x3f, m3x3d>;
    }  // namespace simd_rtm::detail

    template <typename T, typename wrapper_type = simd_rtm::detail::m3x3<T>>
        requires std::is_floating_point_v<T>
    struct alignas(16) mat3x3
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

    private:
        using rtm_t = typename wrapper_type::type;
        rtm_t _value;

        template <typename component_type, Acceleration OtherAccel>
        friend vec3<component_type, OtherAccel> operator*(
            const vec3<component_type, OtherAccel>& vec,
            const mat3x3<component_type>& mat);

    public:
        using rtm_vec4_t = typename simd_rtm::detail::v3<T>::type;
        using rtm_mat3x3_t = rtm_t;
        using vec3_t = vec3<T, Acceleration::Scalar>;
        using fast_vec3_t = vec3<T, acceleration>;
        using vec4_t = vec4<T, Acceleration::Scalar>;
        using fast_vec4_t = vec4<T, acceleration>;
        using quat_t = quat<T>;
        using component_type = T;

    public:
        // Constructors
        MVM_INLINE mat3x3() : _value(rtm::matrix_identity())
        {
        }

        MVM_INLINE mat3x3(const rtm_t& data) : _value(data)
        {
        }

        MVM_INLINE mat3x3(const mat3x3& other) : _value(other._value)
        {
        }

        MVM_INLINE mat3x3(const T& _11,
                          const T& _12,
                          const T& _13,
                          const T& _21,
                          const T& _22,
                          const T& _23,
                          const T& _31,
                          const T& _32,
                          const T& _33) :
            _value(rtm::matrix_set(rtm::vector_set(_11, _12, _13, T(0)),
                                   rtm::vector_set(_21, _22, _23, T(0)),
                                   rtm::vector_set(_31, _32, _33, T(0))))
        {
        }

        MVM_INLINE mat3x3(const fast_vec4_t& row0,
                          const fast_vec4_t& row1,
                          const fast_vec4_t& row2) :
            _value(rtm::matrix_set(row0.to_rtm(), row1.to_rtm(), row2.to_rtm()))
        {
        }

        MVM_INLINE mat3x3& operator=(const mat3x3& other)
        {
            _value = other._value;
            return *this;
        }

    public:
        MVM_INLINE void store_array(T* out) const
        {
            using namespace rtm;

            T temp[4 * 4];
            for (uint8_t i = 0; i < 4; i++)
            {
                vector_store(matrix_get_axis(_value, (axis3)i), temp + i * 4);
            }

            for (uint8_t i = 0; i < 3; i++)
            {
                out[i * 3] = temp[i * 4];
                out[i * 3 + 1] = temp[i * 4 + 1];
                out[i * 3 + 2] = temp[i * 4 + 2];
            }
        }

        MVM_INLINE void load_array(const T* in)
        {
            T temp[3 * 4];
            for (uint8_t i = 0; i < 3; i++)
            {
                temp[i * 4] = in[i * 3];
                temp[i * 4 + 1] = in[i * 3 + 1];
                temp[i * 4 + 2] = in[i * 3 + 2];
                temp[i * 4 + 3] = T(0);
            }

            using namespace rtm;
            rtm_vec4_t x = vector_load(temp);
            rtm_vec4_t y = vector_load(temp + 4);
            rtm_vec4_t z = vector_load(temp + 8);
            rtm_vec4_t w = vector_zero();
            _value = matrix_set(x, y, z, w);
        }

        MVM_INLINE_NODISCARD rtm_t to_rtm()
        {
            return _value;
        }

        MVM_INLINE_NODISCARD static mat3x3 from_rtm(const rtm_t& data)
        {
            return data;
        }

        // Arithmetic operations
    public:
        MVM_INLINE_NODISCARD mat3x3 operator*(const mat3x3& other) const
        {
            return mat3x3(rtm::matrix_mul(_value, other._value));
        }

        MVM_INLINE_NODISCARD fast_vec3_t
        transform_vector(const fast_vec3_t& rhs) const
        {
            rtm_vec4_t mul = rtm::vector_set_w(rhs.to_rtm(), component_type(0));
            rtm_vec4_t res = rtm::vector_mul(
                mul, rtm::matrix_get_axis(_value, rtm::axis3::x));
            return fast_vec3_t::from_rtm(res);
        }

        // Stream overload operators
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const mat3x3& vec)
        {
#if defined(MVM_HAS_MOVE_CORE)
            os << move::meta::type_name<mat3x3>() << "(";
#else
            os << "mat3x3(";
#endif
            for (uint8_t i = 0; i < 3; i++)
            {
                auto row = rtm::matrix_get_axis(vec._value, (rtm::axis3)i);
                os << "(" << rtm::vector_get_x(row) << ", "
                   << rtm::vector_get_y(row) << ", " << rtm::vector_get_z(row)
                   << ")";
                if (i < 2)
                {
                    os << ", ";
                }
            }
            os << ")";
            return os;
        }

        // Comparison operators
    public:
        MVM_INLINE_NODISCARD bool operator==(const mat3x3& other) const
        {
            using namespace rtm;

            // Load all rows
            auto r1 = rtm::matrix_get_axis(_value, axis3::x);
            auto r2 = rtm::matrix_get_axis(_value, axis3::y);
            auto r3 = rtm::matrix_get_axis(_value, axis3::z);

            // Load all rows of the other matrix
            auto o1 = rtm::matrix_get_axis(other._value, axis3::x);
            auto o2 = rtm::matrix_get_axis(other._value, axis3::y);
            auto o3 = rtm::matrix_get_axis(other._value, axis3::z);

            // Compare all rows
            return rtm::mask_all_true(rtm::vector_equal(r1, o1)) &&
                   rtm::mask_all_true(rtm::vector_equal(r2, o2)) &&
                   rtm::mask_all_true(rtm::vector_equal(r3, o3));
        }

        MVM_INLINE_NODISCARD bool operator!=(const mat3x3& other) const
        {
            return !(*this == other);
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[9];
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

    public:
        MVM_INLINE_NODISCARD mat3x3 inverse() const
        {
            return rtm::matrix_inverse(_value);
        }

        MVM_INLINE_NODISCARD mat3x3 transposed() const
        {
            return rtm::matrix_transpose(_value);
        }

        MVM_INLINE_NODISCARD T determinant() const
        {
            T res;
            rtm::scalar_store(rtm::matrix_determinant(_value), &res);
            return res;
        }

    public:
        MVM_INLINE mat3x3& fill(const T& val)
        {
            _value = rtm::matrix_set(val);
            return *this;
        }

        MVM_INLINE mat3x3& invert_in_place()
        {
            _value = rtm::matrix_inverse(_value);
            return *this;
        }

        MVM_INLINE mat3x3& transpose_in_place()
        {
            _value = rtm::matrix_transpose(_value);
            return *this;
        }

        // Statics
    public:
        MVM_INLINE_NODISCARD static mat3x3 identity()
        {
            return mat3x3(rtm::matrix_identity());
        }

        MVM_INLINE_NODISCARD static mat3x3 filled(const T& val)
        {
            return mat3x3(val, val, val, val, val, val, val, val, val);
        }

        MVM_INLINE_NODISCARD static mat3x3 zero()
        {
            return filled(T(0));
        }

        MVM_INLINE_NODISCARD static mat3x3 one()
        {
            return filled(T(1));
        }

        MVM_INLINE_NODISCARD static mat3x3 infinity()
        {
            return filled(std::numeric_limits<T>::infinity());
        }

        MVM_INLINE_NODISCARD static mat3x3 negative_infinity()
        {
            return filled(-std::numeric_limits<T>::infinity());
        }

        MVM_INLINE_NODISCARD static mat3x3 nan()
        {
            return filled(std::numeric_limits<T>::quiet_NaN());
        }

    public:
        MVM_INLINE_NODISCARD static mat3x3 rotation(const quat_t& quat)
        {
            using namespace rtm;
            return matrix_cast<rtm_t>(matrix_from_quat(quat.to_rtm()));
        }

        MVM_INLINE_NODISCARD static mat3x3 rotation_x(const T& angle)
        {
            using namespace rtm;
            return rotation(
                quat_from_axis_angle(vector_set(T(1), T(0), T(0)), angle));
        }

        MVM_INLINE_NODISCARD static mat3x3 rotation_y(const T& angle)
        {
            using namespace rtm;
            return rotation(
                quat_from_axis_angle(vector_set(T(0), T(1), T(0)), angle));
        }

        MVM_INLINE_NODISCARD static mat3x3 rotation_z(const T& angle)
        {
            using namespace rtm;
            return rotation(
                quat_from_axis_angle(vector_set(T(0), T(0), T(1)), angle));
        }

        /**
         * @brief Creates a rotation matrix from an axis and an angle.
         *
         * @param axis The axis to rotate around
         * @param angle The angle in radians to rotate by
         * @return mat4 The rotation matrix
         */
        MVM_INLINE_NODISCARD static mat3x3 angle_axis(const fast_vec3_t& axis,
                                                      const T& angle)
        {
            using namespace rtm;

            auto quat = quat_from_axis_angle(axis.to_rtm(), angle);
            return rtm_mat3x3_t(matrix_cast<rtm_t>(matrix_from_quat(quat)));
        }

        MVM_INLINE_NODISCARD static mat3x3 scale(component_type x,
                                                 component_type y,
                                                 component_type z) noexcept
        {
            return rtm_mat3x3_t(rtm::matrix_cast<rtm_t>(
                rtm::matrix_from_scale(rtm::vector_set(x, y, z, 1))));
        }

        MVM_INLINE_NODISCARD static mat3x3 scale(
            const fast_vec3_t& scale) noexcept
        {
            return rtm_mat3x3_t(rtm::matrix_cast<rtm_t>(
                rtm::matrix_from_scale(scale.to_rtm())));
        }
    };

    // Multiplication operator.  Row-major order.
    template <typename component_type, Acceleration OtherAccel>
    MVM_INLINE_NODISCARD vec3<component_type, OtherAccel> operator*(
        const vec3<component_type, OtherAccel>& vec,
        const mat3x3<component_type>& mat)
    {
        using namespace rtm;
        using Accel = move::math::Acceleration;
        using vector_type = typename simd_rtm::detail::v3<component_type>::type;

        vector_type v = vec.to_rtm();
        vector_type result = matrix_mul_vector3(v, mat._value);
        return vec3<component_type, OtherAccel>::from_rtm(result);
    }

    template <typename T>
    struct storage_mat3x3
    {
    public:
        union
        {
            T data[9];
            struct
            {
                T _11, _12, _13;
                T _21, _22, _23;
                T _31, _32, _33;
            };
        };

    public:
        inline storage_mat3x3() :
            _11(1),
            _12(0),
            _13(0),
            _21(0),
            _22(1),
            _23(0),
            _31(0),
            _32(0),
            _33(1)
        {
        }

        inline storage_mat3x3(const mat3x3<T>& mat)
        {
            mat.store_array(data);
        }

        inline storage_mat3x3(const storage_mat3x3& rhs)
        {
            for (uint8_t i = 0; i < 9; i++)
            {
                data[i] = rhs.data[i];
            }
        }

        inline storage_mat3x3& operator=(const storage_mat3x3& rhs)
        {
            for (uint8_t i = 0; i < 9; i++)
            {
                data[i] = rhs.data[i];
            }
            return *this;
        }

        inline storage_mat3x3& operator=(const mat3x3<T>& rhs)
        {
            rhs.store_array(data);
            return *this;
        }

    public:
        template <typename Archive>
        inline void serialize(Archive& archive)
        {
            archive(data);
        }
    };

    using mat3x3f = mat3x3<float>;
    using mat3x3d = mat3x3<double>;

    using storage_mat3x3f = storage_mat3x3<float>;
    using storage_mat3x3d = storage_mat3x3<double>;

    template <typename T>
    MVM_INLINE_NODISCARD bool approx_equal(
        const mat3x3<T>& a,
        const mat3x3<T>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        T aloaded[9];
        T bloaded[9];

        a.store_array(aloaded);
        b.store_array(bloaded);

        for (uint8_t i = 0; i < 9; i++)
        {
            if (!approx_equal(aloaded[i], bloaded[i], epsilon))
            {
                return false;
            }
        }
        return true;
    }
}  // namespace move::math