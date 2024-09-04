#pragma once
#include <cstdint>
#include <limits>
#include <type_traits>

#include <move/math/common.hpp>
#include <move/math/macros.hpp>
#include <move/math/rtm/base_vec4.hpp>
#include <move/math/scalar/base_vec4.hpp>
#include <move/math/vec2.hpp>
#include <move/math/vec3.hpp>

namespace move::math
{
    template <move::math::Acceleration Accel>
    static constexpr auto vec4_acceleration =
        Accel == Acceleration::Default ? Acceleration::RTM : Accel;

    // If we're doing floating point AND it was requested, use SIMD.  Otherwise,
    // use scalar.
    template <typename T, move::math::Acceleration Accel>
    using base_vec4_t =
        std::conditional_t<vec4_acceleration<Accel> == Acceleration::RTM,
                           std::conditional_t<std::is_floating_point_v<T>,
                                              simd_rtm::base_vec4<T>,
                                              scalar::base_vec4<T>>,
                           scalar::base_vec4<T>>;

    template <typename T, move::math::Acceleration Accel>
    struct vec4 : public base_vec4_t<T, Accel>
    {
    public:
        using base_t = base_vec4_t<T, Accel>;
        using component_type = T;
        using rtm_t = simd_rtm::detail::v4<T>::type;

        constexpr static auto acceleration = base_t::acceleration;
        constexpr static bool has_fields = base_t::has_fields;
        constexpr static bool has_pointer_semantics =
            base_t::has_pointer_semantics;

        // Constructors
    public:
        MVM_INLINE vec4() : base_t()
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        MVM_INLINE vec4(vec4<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z(), other.get_w())
        {
        }

        // implicit conversion from base_vec4_t
        MVM_INLINE vec4(base_t other) : base_t(other)
        {
        }

        template <typename ComponentT, Acceleration OtherAccel>
        // implicit conversion from base_vec4_t
        MVM_INLINE vec4(base_vec4_t<ComponentT, OtherAccel> other) :
            base_t(other.get_x(), other.get_y(), other.get_z(), other.get_w())
        {
        }

        MVM_INLINE vec4(const T& x,
                        const T& y = 0,
                        const T& z = 0,
                        const T& w = 0) :
            base_t(x, y, z, w)
        {
        }

        MVM_INLINE vec4(const vec4& other) : base_t(other)
        {
        }

        MVM_INLINE vec4& operator=(const vec4& other)
        {
            base_t::operator=(other);
            return *this;
        }

        // Vector length conversions
    public:
        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE vec4(const vec2<OtherT, OtherAccel>& vec,
                        const T& z = 0,
                        const T& w = 0) :
            base_t(vec.get_x(), vec.get_y(), z, w)
        {
        }

        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE vec4(const vec3<OtherT, OtherAccel>& vec, const T& w = 0) :
            base_t(vec.get_x(), vec.get_y(), vec.get_z(), w)
        {
        }

        template <typename OtherT, Acceleration OtherAccel>
        MVM_INLINE operator vec3<OtherT, OtherAccel>()
        {
            using self_t = vec4;
            using other_t = vec3<OtherT, OtherAccel>;
            if constexpr (other_t::acceleration == self_t::acceleration &&
                          self_t::acceleration == Acceleration::RTM)
            {
                return other_t::from_rtm(to_rtm());
            }

            return other_t(base_t::get_x(), base_t::get_y(), base_t::get_z());
        }

        // Swizzles
    public:
        // Vec2 swizzles
        using vec2_t = vec2<T, Accel>;
        MVM_INLINE_NODISCARD vec2_t xy() const
        {
            return vec2_t(base_t::get_x(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec2_t xz() const
        {
            return vec2_t(base_t::get_x(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t xw() const
        {
            return vec2_t(base_t::get_x(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec2_t yx() const
        {
            return vec2_t(base_t::get_y(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t yz() const
        {
            return vec2_t(base_t::get_y(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec2_t yw() const
        {
            return vec2_t(base_t::get_y(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec2_t zx() const
        {
            return vec2_t(base_t::get_z(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t zy() const
        {
            return vec2_t(base_t::get_z(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec2_t zw() const
        {
            return vec2_t(base_t::get_z(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec2_t wx() const
        {
            return vec2_t(base_t::get_w(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec2_t wy() const
        {
            return vec2_t(base_t::get_w(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec2_t wz() const
        {
            return vec2_t(base_t::get_w(), base_t::get_z());
        }

        // Vec3 swizzles
        using vec3_t = vec3<T, Accel>;
        MVM_INLINE_NODISCARD vec3_t xyz() const
        {
            return vec3_t(base_t::get_x(), base_t::get_y(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t xyw() const
        {
            return vec3_t(base_t::get_x(), base_t::get_y(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t xzy() const
        {
            return vec3_t(base_t::get_x(), base_t::get_z(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec3_t xzw() const
        {
            return vec3_t(base_t::get_x(), base_t::get_z(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t xwy() const
        {
            return vec3_t(base_t::get_x(), base_t::get_w(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec3_t xwz() const
        {
            return vec3_t(base_t::get_x(), base_t::get_w(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t yxz() const
        {
            return vec3_t(base_t::get_y(), base_t::get_x(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t yxw() const
        {
            return vec3_t(base_t::get_y(), base_t::get_x(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t yzx() const
        {
            return vec3_t(base_t::get_y(), base_t::get_z(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t yzw() const
        {
            return vec3_t(base_t::get_y(), base_t::get_z(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t ywx() const
        {
            return vec3_t(base_t::get_y(), base_t::get_w(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t ywz() const
        {
            return vec3_t(base_t::get_y(), base_t::get_w(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t zxy() const
        {
            return vec3_t(base_t::get_z(), base_t::get_x(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec3_t zxw() const
        {
            return vec3_t(base_t::get_z(), base_t::get_x(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t zyx() const
        {
            return vec3_t(base_t::get_z(), base_t::get_y(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t zyw() const
        {
            return vec3_t(base_t::get_z(), base_t::get_y(), base_t::get_w());
        }

        MVM_INLINE_NODISCARD vec3_t zwx() const
        {
            return vec3_t(base_t::get_z(), base_t::get_w(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t zwy() const
        {
            return vec3_t(base_t::get_z(), base_t::get_w(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec3_t wxy() const
        {
            return vec3_t(base_t::get_w(), base_t::get_x(), base_t::get_y());
        }

        MVM_INLINE_NODISCARD vec3_t wxz() const
        {
            return vec3_t(base_t::get_w(), base_t::get_x(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t wyx() const
        {
            return vec3_t(base_t::get_w(), base_t::get_y(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t wyz() const
        {
            return vec3_t(base_t::get_w(), base_t::get_y(), base_t::get_z());
        }

        MVM_INLINE_NODISCARD vec3_t wzx() const
        {
            return vec3_t(base_t::get_w(), base_t::get_z(), base_t::get_x());
        }

        MVM_INLINE_NODISCARD vec3_t wzy() const
        {
            return vec3_t(base_t::get_w(), base_t::get_z(), base_t::get_y());
        }

        // Conversions
    public:
        MVM_INLINE_NODISCARD base_t storage() const
        {
            return *this;
        }

        MVM_INLINE_NODISCARD static vec4 from_rtm(const rtm_t& rtm_vec)
        {
            if constexpr (acceleration == Acceleration::RTM)
            {
                return vec4(base_t(rtm_vec));
            }
            else
            {
                return vec4(
                    rtm::vector_get_x(rtm_vec), rtm::vector_get_y(rtm_vec),
                    rtm::vector_get_z(rtm_vec), rtm::vector_get_w(rtm_vec));
            }
        }

        MVM_INLINE_NODISCARD rtm_t to_rtm() const
        {
            if constexpr (acceleration == Acceleration::RTM)
            {
                return base_t::to_rtm();
            }
            else
            {
                return rtm::vector_set(base_t::get_x(), base_t::get_y(),
                                       base_t::get_z(), base_t::get_w());
            }
        }

        // Assignment operators
    public:
        MVM_INLINE vec4& operator+=(const vec4& other)
        {
            *this = *this + other;
            return *this;
        }

        MVM_INLINE vec4& operator-=(const vec4& other)
        {
            *this = *this - other;
            return *this;
        }

        MVM_INLINE vec4& operator*=(const vec4& other)
        {
            *this = *this * other;
            return *this;
        }

        MVM_INLINE vec4& operator/=(const vec4& other)
        {
            *this = *this / other;
            return *this;
        }

        MVM_INLINE vec4& operator+=(const T& scalar)
        {
            *this = *this + scalar;
            return *this;
        }

        MVM_INLINE vec4& operator-=(const T& scalar)
        {
            *this = *this - scalar;
            return *this;
        }

        MVM_INLINE vec4& operator*=(const T& scalar)
        {
            *this = *this * scalar;
            return *this;
        }

        MVM_INLINE vec4& operator/=(const T& scalar)
        {
            *this = *this / scalar;
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

    using float4 = vec4<float, Acceleration::Default>;
    using double4 = vec4<double, Acceleration::Default>;

    using long4 = vec4<int64_t, Acceleration::Default>;
    using ulong4 = vec4<uint64_t, Acceleration::Default>;

    using int4 = vec4<int32_t, Acceleration::Default>;
    using uint4 = vec4<uint32_t, Acceleration::Default>;

    using short4 = vec4<int16_t, Acceleration::Default>;
    using ushort4 = vec4<uint16_t, Acceleration::Default>;

    using sbyte4 = vec4<int8_t, Acceleration::Default>;
    using byte4 = vec4<uint8_t, Acceleration::Default>;

    template <typename T, move::math::Acceleration Accel>
    MVM_INLINE_NODISCARD bool approx_equal(
        const vec4<T, Accel>& a,
        const vec4<T, Accel>& b,
        const T& epsilon = std::numeric_limits<T>::epsilon())
    {
        T aloaded[4];
        T bloaded[4];

        a.store_array(aloaded);
        b.store_array(bloaded);

        return approx_equal(aloaded[0], bloaded[0], epsilon) &&
               approx_equal(aloaded[1], bloaded[1], epsilon) &&
               approx_equal(aloaded[2], bloaded[2], epsilon) &&
               approx_equal(aloaded[3], bloaded[3], epsilon);
    }
}  // namespace move::math