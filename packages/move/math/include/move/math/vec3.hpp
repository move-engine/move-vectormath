#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/base_vec3.hpp>
#include <move/math/scalar/base_vec3.hpp>
#include <move/math/traits.hpp>
#include <move/math/vec2.hpp>

namespace move::math
{
    namespace detail
    {
        template <typename T, move::math::Acceleration Accel>
        static constexpr auto real_vec3_acceleration =
            Accel == Acceleration::RTM || Accel == Acceleration::Default
                ? (std::is_floating_point_v<T> ? Acceleration::RTM
                                               : Acceleration::Scalar)
                : Acceleration::Scalar;
    }

    // If we're doing floating point AND it was requested, use SIMD.  Otherwise,
    // use scalar.
    template <typename T, move::math::Acceleration Accel>
    using base_vec3_t =
        std::conditional_t<detail::real_vec3_acceleration<T, Accel> ==
                               Acceleration::RTM,
                           simd_rtm::base_vec3<T>,
                           scalar::base_vec3<T>>;

    template <typename T,
              move::math::Acceleration RequestedAccel,
              move::math::Acceleration Accel =
                  detail::real_vec3_acceleration<T, RequestedAccel>>
    struct vec3 : public base_vec3_t<T, Accel>
    {
    public:
        using base_t = base_vec3_t<T, Accel>;
        using component_type = T;
        using rtm_t = typename simd_rtm::detail::v3<T>::type;

        constexpr static auto acceleration = base_t::acceleration;
        constexpr static bool has_fields = base_t::has_fields;
        constexpr static bool has_pointer_semantics =
            base_t::has_pointer_semantics;
        constexpr static uint32_t element_count = base_t::element_count;

        // Constructors
    public:
        MVM_INLINE vec3() : base_t()
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        MVM_INLINE vec3(vec3<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z())
        {
        }

        MVM_INLINE vec3(const scalar::base_vec3<T>& rhs)
            requires std::is_same_v<base_t, scalar::base_vec3<T>>
            : base_t(rhs)
        {
        }

        MVM_INLINE vec3(const scalar::base_vec3<T>& rhs)
            requires(!std::is_same_v<base_t, scalar::base_vec3<T>>)
            : base_t(rhs.x, rhs.y, rhs.z)
        {
        }

        MVM_INLINE vec3(const simd_rtm::base_vec3<T>& rhs)
            requires std::is_same_v<base_t, simd_rtm::base_vec3<T>>
            : base_t(rhs)
        {
        }

        MVM_INLINE vec3(const simd_rtm::base_vec3<T>& rhs)
            requires(!std::is_same_v<base_t, simd_rtm::base_vec3<T>>)
            : base_t(rhs.get_x(), rhs.get_y(), rhs.get_z())
        {
        }

        MVM_INLINE vec3(const T& x, const T& y = 0, const T& z = 0) :
            base_t(x, y, z)
        {
        }

        MVM_INLINE vec3(const vec3& other) : base_t(other)
        {
        }

        MVM_INLINE vec3& operator=(const vec3& other)
        {
            base_t::operator=(other);
            return *this;
        }

        // Vector length conversions
    public:
        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE vec3(const vec2<OtherT, OtherAccel>& vec, const T& z = 0) :
            base_t(vec.get_x(), vec.get_y(), z)
        {
        }

        // Public operations return the public wrapper, never the backend type.
    public:
        MVM_INLINE_NODISCARD static vec3 from_array(const T* src)
        {
            return vec3(base_t::from_array(src));
        }

        MVM_INLINE vec3& load_array(const T* src)
        {
            base_t::load_array(src);
            return *this;
        }

        MVM_INLINE_NODISCARD vec3 operator+(const vec3& rhs) const
        {
            return vec3(base_t::operator+(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator-(const vec3& rhs) const
        {
            return vec3(base_t::operator-(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator*(const vec3& rhs) const
        {
            return vec3(base_t::operator*(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator/(const vec3& rhs) const
        {
            return vec3(base_t::operator/(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator+(const T& rhs) const
        {
            return vec3(base_t::operator+(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator-(const T& rhs) const
        {
            return vec3(base_t::operator-(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator*(const T& rhs) const
        {
            return vec3(base_t::operator*(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator/(const T& rhs) const
        {
            return vec3(base_t::operator/(rhs));
        }

        MVM_INLINE_NODISCARD vec3 operator-() const
        {
            return vec3(base_t::operator-());
        }

        MVM_INLINE_NODISCARD vec3 normalized() const
            requires std::is_floating_point_v<T>
        {
            return vec3(base_t::normalized());
        }

        MVM_INLINE vec3& normalize()
            requires std::is_floating_point_v<T>
        {
            base_t::normalize();
            return *this;
        }

        MVM_INLINE vec3& fill(const T& value)
        {
            base_t::fill(value);
            return *this;
        }

        MVM_INLINE vec3& set(const T& x, const T& y, const T& z)
        {
            base_t::set(x, y, z);
            return *this;
        }

        MVM_INLINE vec3& set_zero()
        {
            base_t::set_zero();
            return *this;
        }

        MVM_INLINE_NODISCARD static vec3 cross(const vec3& a,
                                               const vec3& b) noexcept
        {
            return vec3(base_t::cross(a, b));
        }

        MVM_INLINE_NODISCARD static vec3 reflect(const vec3& incident,
                                                 const vec3& normal) noexcept
        {
            return vec3(base_t::reflect(incident, normal));
        }

        MVM_INLINE_NODISCARD static vec3 refract(const vec3& incident,
                                                 const vec3& normal,
                                                 T ior) noexcept
        {
            return vec3(base_t::refract(incident, normal, ior));
        }

        MVM_INLINE_NODISCARD static vec3 lerp_unclamped(const vec3& a,
                                                        const vec3& b,
                                                        T t) noexcept
        {
            return vec3(base_t::lerp_unclamped(a, b, t));
        }

        MVM_INLINE_NODISCARD static vec3 lerp_unclamped(const vec3& a,
                                                        const vec3& b,
                                                        const vec3& t) noexcept
        {
            return vec3(base_t::lerp_unclamped(a, b, t));
        }

        MVM_INLINE_NODISCARD static vec3 lerp(const vec3& a,
                                              const vec3& b,
                                              T t) noexcept
        {
            return vec3(base_t::lerp(a, b, t));
        }

        MVM_INLINE_NODISCARD static vec3 lerp(const vec3& a,
                                              const vec3& b,
                                              const vec3& t) noexcept
        {
            return vec3(base_t::lerp(a, b, t));
        }

        MVM_INLINE_NODISCARD static vec3 min(const vec3& a,
                                             const vec3& b) noexcept
        {
            return vec3(base_t::min(a, b));
        }

        MVM_INLINE_NODISCARD static vec3 max(const vec3& a,
                                             const vec3& b) noexcept
        {
            return vec3(base_t::max(a, b));
        }

        MVM_INLINE_NODISCARD static vec3 clamp(const vec3& value,
                                               const vec3& minimum,
                                               const vec3& maximum) noexcept
        {
            return vec3(base_t::clamp(value, minimum, maximum));
        }

        MVM_INLINE_NODISCARD static vec3 clamp(const vec3& value,
                                               const T& minimum,
                                               const T& maximum) noexcept
        {
            return vec3(base_t::clamp(value, minimum, maximum));
        }

        MVM_INLINE_NODISCARD static vec3 filled(T value) noexcept
        {
            return vec3(base_t::filled(value));
        }

        MVM_INLINE_NODISCARD static vec3 infinity() noexcept
        {
            return vec3(base_t::infinity());
        }

        MVM_INLINE_NODISCARD static vec3 negative_infinity() noexcept
        {
            return vec3(base_t::negative_infinity());
        }

        MVM_INLINE_NODISCARD static vec3 nan() noexcept
        {
            return vec3(base_t::nan());
        }

        MVM_INLINE_NODISCARD static vec3 zero() noexcept
        {
            return vec3(base_t::zero());
        }

        MVM_INLINE_NODISCARD static vec3 one() noexcept
        {
            return vec3(base_t::one());
        }

        MVM_INLINE_NODISCARD static vec3 x_axis() noexcept
        {
            return vec3(base_t::x_axis());
        }

        MVM_INLINE_NODISCARD static vec3 y_axis() noexcept
        {
            return vec3(base_t::y_axis());
        }

        MVM_INLINE_NODISCARD static vec3 z_axis() noexcept
        {
            return vec3(base_t::z_axis());
        }

        MVM_INLINE_NODISCARD static vec3 left() noexcept
        {
            return vec3(base_t::left());
        }

        MVM_INLINE_NODISCARD static vec3 right() noexcept
        {
            return vec3(base_t::right());
        }

        MVM_INLINE_NODISCARD static vec3 down() noexcept
        {
            return vec3(base_t::down());
        }

        MVM_INLINE_NODISCARD static vec3 up() noexcept
        {
            return vec3(base_t::up());
        }

        MVM_INLINE_NODISCARD static vec3 backward() noexcept
        {
            return vec3(base_t::backward());
        }

        MVM_INLINE_NODISCARD static vec3 forward() noexcept
        {
            return vec3(base_t::forward());
        }

        MVM_INLINE_NODISCARD static vec3 abs(const vec3& value) noexcept
        {
            return vec3(base_t::abs(value));
        }

        MVM_INLINE_NODISCARD static vec3 sign(const vec3& value) noexcept
        {
            return vec3(base_t::sign(value));
        }

        MVM_INLINE_NODISCARD static vec3 project_onto_plane(
            const vec3& value, const vec3& plane_normal) noexcept
        {
            return vec3(base_t::project_onto_plane(value, plane_normal));
        }

        // Acceleration conversions
    public:
        template <Acceleration TargetAccel>
        MVM_INLINE_NODISCARD vec3<T, TargetAccel> to_accel() const
        {
            if constexpr (acceleration ==
                          detail::real_vec3_acceleration<T, TargetAccel>)
            {
                return *this;
            }
            else
            {
                T data[3];
                base_t::store_array(data);
                return vec3<T, TargetAccel>::base_t::from_array(data);
            }
        }

        MVM_INLINE_NODISCARD
        vec3<T, detail::real_vec3_acceleration<T, Acceleration::RTM>> fast()
            const
        {
            return to_accel<
                detail::real_vec3_acceleration<T, Acceleration::RTM>>();
        }

        MVM_INLINE_NODISCARD vec3<T, Acceleration::Scalar> storable() const
        {
            return to_accel<Acceleration::Scalar>();
        }

        // MVM_NODISCARD operator vec3<T, Acceleration::Scalar>() const
        // {
        //     return storable();
        // }

        // MVM_NODISCARD operator vec3<T, Acceleration::RTM>() const
        // {
        //     return fast();
        // }

        MVM_NODISCARD operator rtm_t() const
        {
            return to_rtm();
        }

        // Swizzles
    public:
        using vec2_t = vec2<T, Acceleration::Scalar>;
        MVM_INLINE_NODISCARD vec2_t xy() const
        {
            return vec2_t(base_t::get_x(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec2_t xz() const
        {
            return vec2_t(base_t::get_x(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t yx() const
        {
            return vec2_t(base_t::get_y(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t yz() const
        {
            return vec2_t(base_t::get_y(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t zx() const
        {
            return vec2_t(base_t::get_z(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t zy() const
        {
            return vec2_t(base_t::get_z(), base_t::get_y());
        }

        // Conversions
    public:
        MVM_INLINE_NODISCARD rtm_t to_rtm() const
        {
            if constexpr (acceleration == Acceleration::RTM)
            {
                return rtm::vector_set_w(base_t::to_rtm(), T(1));
            }
            else
            {
                return rtm::vector_set(base_t::get_x(), base_t::get_y(),
                                       base_t::get_z(), T(1));
            }
        }

        MVM_INLINE_NODISCARD static vec3 from_rtm(const rtm_t& rtm_vec)
        {
            if constexpr (acceleration == Acceleration::RTM)
            {
                return vec3(base_t(rtm::vector_set_w(rtm_vec, T(0))));
            }
            else
            {
                return vec3(rtm::vector_get_x(rtm_vec),
                            rtm::vector_get_y(rtm_vec),
                            rtm::vector_get_z(rtm_vec));
            }
        }

        // Assignment operators
    public:
        MVM_INLINE vec3& operator+=(const vec3& other)
        {
            base_t::operator+=(other);
            return *this;
        }

        MVM_INLINE vec3& operator-=(const vec3& other)
        {
            base_t::operator-=(other);
            return *this;
        }

        MVM_INLINE vec3& operator*=(const vec3& other)
        {
            base_t::operator*=(other);
            return *this;
        }

        MVM_INLINE vec3& operator/=(const vec3& other)
        {
            base_t::operator/=(other);
            return *this;
        }

        MVM_INLINE vec3& operator+=(const T& scalar)
        {
            base_t::operator+=(scalar);
            return *this;
        }

        MVM_INLINE vec3& operator-=(const T& scalar)
        {
            base_t::operator-=(scalar);
            return *this;
        }

        MVM_INLINE vec3& operator*=(const T& scalar)
        {
            base_t::operator*=(scalar);
            return *this;
        }

        MVM_INLINE vec3& operator/=(const T& scalar)
        {
            base_t::operator/=(scalar);
            return *this;
        }

        // Serialization
    public:
        template <typename Archive>
        MVM_INLINE void serialize(Archive& archive)
        {
            base_t::serialize(archive);
        }
    };

    using fast_float3 = vec3<float, Acceleration::RTM>;
    using fast_double3 = vec3<double, Acceleration::RTM>;
    using storage_float3 = vec3<float, Acceleration::Scalar>;
    using storage_double3 = vec3<double, Acceleration::Scalar>;

    using fast_long3 = vec3<int64_t, Acceleration::Default>;
    using fast_ulong3 = vec3<uint64_t, Acceleration::Default>;
    using storage_long3 = vec3<int64_t, Acceleration::Scalar>;
    using storage_ulong3 = vec3<uint64_t, Acceleration::Scalar>;

    using fast_int3 = vec3<int32_t, Acceleration::Default>;
    using fast_uint3 = vec3<uint32_t, Acceleration::Default>;
    using storage_int3 = vec3<int32_t, Acceleration::Scalar>;
    using storage_uint3 = vec3<uint32_t, Acceleration::Scalar>;

    using fast_short3 = vec3<int16_t, Acceleration::Default>;
    using fast_ushort3 = vec3<uint16_t, Acceleration::Default>;
    using storage_short3 = vec3<int16_t, Acceleration::Scalar>;
    using storage_ushort3 = vec3<uint16_t, Acceleration::Scalar>;

    using fast_sbyte3 = vec3<int8_t, Acceleration::Default>;
    using fast_byte3 = vec3<uint8_t, Acceleration::Default>;
    using storage_sbyte3 = vec3<int8_t, Acceleration::Scalar>;
    using storage_byte3 = vec3<uint8_t, Acceleration::Scalar>;

    using float3 = fast_float3;
    using double3 = fast_double3;

    using long3 = storage_long3;
    using ulong3 = storage_ulong3;

    using int3 = storage_int3;
    using uint3 = storage_uint3;

    using short3 = storage_short3;
    using ushort3 = storage_ushort3;

    using sbyte3 = storage_sbyte3;
    using byte3 = storage_byte3;

    using vec3f = storage_float3;
    using vec3d = storage_double3;
    using vec3i = storage_int3;
    using vec3u = storage_uint3;
    using vec3l = storage_long3;
    using vec3ul = storage_ulong3;
    using vec3s = storage_short3;
    using vec3us = storage_ushort3;

    using vec3b = storage_byte3;
    using vec3sb = storage_sbyte3;

    template <typename T, move::math::Acceleration Accel>
    MVM_INLINE_NODISCARD bool approx_equal(
        const vec3<T, Accel>& a,
        const vec3<T, Accel>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        return approx_equal(a.get_x(), b.get_x(), epsilon) &&
               approx_equal(a.get_y(), b.get_y(), epsilon) &&
               approx_equal(a.get_z(), b.get_z(), epsilon);
    }

    namespace traits
    {
        template <typename T, Acceleration Accel>
        constexpr bool is_vector_type_v<vec3<T, Accel>> = true;

        template <typename T, Acceleration Accel>
        constexpr uint32_t component_count_v<vec3<T, Accel>> = 3;
    }  // namespace traits
}  // namespace move::math
