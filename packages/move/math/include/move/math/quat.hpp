#pragma once
#include <cmath>
#include <type_traits>

#include <rtm/impl/vector_common.h>
#include <rtm/matrix3x3d.h>
#include <rtm/matrix3x3f.h>
#include <rtm/quatd.h>
#include <rtm/quatf.h>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/rtm_ext.hpp>
#include <move/math/vec4.hpp>

namespace move::math
{
    namespace simd_rtm::detail
    {
        MVM_TYPE_WRAPPER(quatf, rtm::quatf)
        MVM_TYPE_WRAPPER(quatd, rtm::quatd)

        template <typename T>
        using quat = std::conditional_t<std::is_same_v<T, float>, quatf, quatd>;
    }  // namespace simd_rtm::detail

    template <typename T, typename wrapper_type = simd_rtm::detail::quat<T>>
        requires std::is_floating_point_v<T>
    struct quat
    {
    public:
        constexpr static auto acceleration = Acceleration::RTM;
        constexpr static bool has_fields = false;
        constexpr static bool has_pointer_semantics = false;

    private:
        using underlying_type = typename wrapper_type::type;
        underlying_type _value;

        template <typename component_type, Acceleration OtherAccel>
        friend vec3<component_type, OtherAccel> operator*(
            const vec3<component_type, OtherAccel>& vec,
            const quat<component_type>& quat);

    public:
        using rtm_vec4_t = typename simd_rtm::detail::v4<T>::type;
        using rtm_quat_t = underlying_type;
        using vec3_t = vec3<T, acceleration>;
        using vec4_t = vec4<T, acceleration>;
        using component_type = T;

        // Constructors
    public:
        MVM_INLINE quat() : _value(rtm::quat_identity())
        {
        }

        MVM_INLINE quat(const underlying_type& data) : _value(data)
        {
        }

        MVM_INLINE quat(const quat& other) : _value(other._value)
        {
        }

        MVM_INLINE quat(const T& x, const T& y, const T& z, const T& w) :
            _value(rtm::quat_set(x, y, z, w))
        {
        }

        MVM_INLINE quat& operator=(const quat& other)
        {
            _value = other._value;
            return *this;
        }

        // Pointers
    public:
        MVM_INLINE void store_array(T* out) const
        {
            using namespace rtm;
            quat_store(_value, out);
        }

        MVM_INLINE void load_array(const T* in)
        {
            using namespace rtm;
            _value = quat_load(in);
        }

        MVM_INLINE_NODISCARD underlying_type to_rtm() const
        {
            return _value;
        }

        MVM_INLINE_NODISCARD static quat from_rtm(const underlying_type& data)
        {
            quat result;
            result._value = data;
            return result;
        }

        // Arithmetic operations
    public:
        MVM_INLINE_NODISCARD quat operator*(const quat& other) const
        {
            using namespace rtm;
            return quat::from_rtm(quat_mul(_value, other._value));
        };

        MVM_INLINE_NODISCARD vec3_t rotate_point(const vec3_t& point) const
        {
            using namespace rtm;
            return vec3_t::from_rtm(quat_mul_vector3(point.to_rtm(), _value));
        }

        // Stream overload operators
    public:
        template <typename CharT, typename Traits>
        friend std::basic_ostream<CharT, Traits>& operator<<(
            std::basic_ostream<CharT, Traits>& os, const quat& vec)
        {
#if defined(MVM_HAS_MOVE_CORE)
            os << move::meta::type_name<quat>() << "(";
#else
            os << "quat(";
#endif
            T data[4];
            vec.store_array(data);
            for (uint8_t i = 0; i < 4; i++)
            {
                os << data[i];
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
        MVM_INLINE_NODISCARD bool operator==(const quat& other) const
        {
            return get_x() == other.get_x() && get_y() == other.get_y() &&
                   get_z() == other.get_z() && get_w() == other.get_w();
        }

        MVM_INLINE_NODISCARD bool operator!=(const quat& other) const
        {
            return !(*this == other);
        }

        // Element access
    public:
        MVM_INLINE_NODISCARD T operator[](const std::size_t index) const
        {
            using namespace rtm;
            switch (index)
            {
                case 0:
                    return quat_get_x(_value);
                case 1:
                    return quat_get_y(_value);
                case 2:
                    return quat_get_z(_value);
                case 3:
                    return quat_get_w(_value);
                default:
                    return T(0);
            }
        }

        MVM_INLINE_NODISCARD T get_x() const
        {
            using namespace rtm;
            return quat_get_x(_value);
        }

        MVM_INLINE_NODISCARD T get_y() const
        {
            using namespace rtm;
            return quat_get_y(_value);
        }

        MVM_INLINE_NODISCARD T get_z() const
        {
            using namespace rtm;
            return quat_get_z(_value);
        }

        MVM_INLINE_NODISCARD T get_w() const
        {
            using namespace rtm;
            return quat_get_w(_value);
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            T data[4];
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
        MVM_INLINE_NODISCARD quat inverse() const
        {
            return quat::from_rtm(rtm::ext::quat_inverse(_value));
        }

        MVM_INLINE_NODISCARD quat conjugate() const
        {
            using namespace rtm;
            return quat::from_rtm(quat_conjugate(_value));
        }

        MVM_INLINE_NODISCARD quat normalized() const
        {
            using namespace rtm;
            return length_squared() == T(0)
                       ? zero()
                       : quat::from_rtm(quat_normalize(_value));
        }

        MVM_INLINE_NODISCARD T length() const
        {
            using namespace rtm;
            return quat_length(_value);
        }

        MVM_INLINE_NODISCARD T length_squared() const
        {
            using namespace rtm;
            return quat_length_squared(_value);
        }

        MVM_INLINE_NODISCARD T dot(const quat& other) const
        {
            using namespace rtm;
            return quat_dot(_value, other._value);
        }

        MVM_INLINE_NODISCARD quat ln() const
        {
            using namespace rtm;
            return quat::from_rtm(rtm::ext::quat_ln(_value));
        }

        MVM_INLINE_NODISCARD quat exp() const
        {
            using namespace rtm;
            return quat::from_rtm(rtm::ext::quat_exp(_value));
        }

        // Mutators
    public:
        MVM_INLINE quat& invert_in_place()
        {
            _value = rtm::ext::quat_inverse(_value);
            return *this;
        }

        MVM_INLINE quat& normalize()
        {
            *this = normalized();
            return *this;
        }

        // Statics
    public:
        MVM_INLINE_NODISCARD static quat zero()
        {
            return {0, 0, 0, 0};
        }

        MVM_INLINE_NODISCARD static quat identity()
        {
            return quat::from_rtm(rtm::quat_identity());
        }

        MVM_INLINE_NODISCARD static quat nan()
        {
            return quat::from_rtm(
                rtm::quat_set(std::numeric_limits<T>::quiet_NaN(),
                              std::numeric_limits<T>::quiet_NaN(),
                              std::numeric_limits<T>::quiet_NaN(),
                              std::numeric_limits<T>::quiet_NaN()));
        }

        MVM_INLINE_NODISCARD static quat angle_axis(const vec3_t& axis,
                                                    const T& angle)
        {
            using namespace rtm;
            return quat::from_rtm(quat_from_axis_angle(axis.to_rtm(), angle));
        }

        MVM_INLINE_NODISCARD static quat euler(const T& pitch,
                                               const T& yaw,
                                               const T& roll)
        {
            using namespace rtm;
            return quat::from_rtm(quat_from_euler(pitch, yaw, roll));
        }

        MVM_INLINE_NODISCARD static quat euler(const vec3_t& euler)
        {
            using namespace rtm;
            T data[3];
            euler.store_array(data);
            return quat::from_rtm(quat_from_euler(data[0], data[1], data[2]));
        }

        // Provenance: MVM-PROV-LEGACY-QUATERNION-AUDIT.
        MVM_INLINE_NODISCARD static quat look_rotation(const vec3_t& forward,
                                                       const vec3_t& up)
        {
            if (forward.length_squared() == T(0))
            {
                return identity();
            }

            const vec3_t normalized_forward = forward.normalized();
            vec3_t right = vec3_t::cross(up, normalized_forward);
            if (right.length_squared() <= std::numeric_limits<T>::epsilon())
            {
                const vec3_t fallback_up =
                    math::abs(normalized_forward.get_y()) < T(0.999)
                        ? vec3_t::up()
                        : vec3_t::right();
                right = vec3_t::cross(fallback_up, normalized_forward);
            }

            right.normalize();
            const vec3_t corrected_up =
                vec3_t::cross(normalized_forward, right);

            if constexpr (std::is_same_v<T, float>)
            {
                const rtm::matrix3x3f rotation{right.to_rtm(),
                                               corrected_up.to_rtm(),
                                               normalized_forward.to_rtm()};
                return quat::from_rtm(rtm::quat_from_matrix(rotation));
            }
            else
            {
                const rtm::matrix3x3d rotation{right.to_rtm(),
                                               corrected_up.to_rtm(),
                                               normalized_forward.to_rtm()};
                return quat::from_rtm(rtm::quat_from_matrix(rotation));
            }
        }

        MVM_INLINE_NODISCARD static quat rotation_x(const T& angle)
        {
            using namespace rtm;
            return quat::from_rtm(
                quat_from_axis_angle(vector_set(T(1), T(0), T(0)), angle));
        }

        MVM_INLINE_NODISCARD static quat rotation_y(const T& angle)
        {
            using namespace rtm;
            return quat::from_rtm(
                quat_from_axis_angle(vector_set(T(0), T(1), T(0)), angle));
        }

        MVM_INLINE_NODISCARD static quat rotation_z(const T& angle)
        {
            using namespace rtm;
            return quat::from_rtm(
                quat_from_axis_angle(vector_set(T(0), T(0), T(1)), angle));
        }

        MVM_INLINE_NODISCARD static quat ln(const quat& q)
        {
            return q.ln();
        }

        MVM_INLINE_NODISCARD static quat exp(const quat& q)
        {
            return q.exp();
        }
    };

    template <typename component_type, Acceleration OtherAccel>
    MVM_INLINE_NODISCARD vec3<component_type, OtherAccel> operator*(
        const vec3<component_type, OtherAccel>& vec,
        const quat<component_type>& quat)
    {
        using vector_type = typename simd_rtm::detail::v4<component_type>::type;

        vector_type result = rtm::quat_mul_vector3(vec.to_rtm(), quat._value);
        return vec3<component_type, OtherAccel>::from_rtm(result);
    }

    using quatf = quat<float>;
    using quatd = quat<double>;

    using floatq = quatf;
    using doubleq = quatd;

    template <typename T>
    MVM_INLINE_NODISCARD bool approx_equal(
        const quat<T>& a,
        const quat<T>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        const auto lane_equal = [&](T lhs, T rhs)
        {
            return approx_equal(lhs, rhs, epsilon);
        };

        return lane_equal(a.get_x(), b.get_x()) &&
               lane_equal(a.get_y(), b.get_y()) &&
               lane_equal(a.get_z(), b.get_z()) &&
               lane_equal(a.get_w(), b.get_w());
    }

    template <typename T>
    // Provenance: MVM-PROV-LEGACY-QUATERNION-AUDIT.
    MVM_INLINE_NODISCARD bool same_rotation(
        const quat<T>& a,
        const quat<T>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        if (approx_equal(a, b, epsilon))
        {
            return true;
        }
        return approx_equal(a.get_x(), -b.get_x(), epsilon) &&
               approx_equal(a.get_y(), -b.get_y(), epsilon) &&
               approx_equal(a.get_z(), -b.get_z(), epsilon) &&
               approx_equal(a.get_w(), -b.get_w(), epsilon);
    }
}  // namespace move::math
