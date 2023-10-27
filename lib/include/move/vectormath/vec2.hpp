#pragma once
#include <ostream>

#include <rtm/scalard.h>

namespace move::vectormath
{
    template <typename value_type>
    struct generic_vec2_scalar
    {
    public:
        /**
         * @brief The type of the components of the vector
         */
        using component_type = value_type;

        /**
         * @brief The number of components in the vector
         */
        constexpr static uint32_t num_components = 2;

    public:
        RTM_FORCE_INLINE generic_vec2_scalar() noexcept : _x(0), _y(0)
        {
        }

        RTM_FORCE_INLINE generic_vec2_scalar(
            value_type x, value_type y = 0) noexcept
            : _x(x), _y(y)
        {
        }

    public:
        template <typename Archive>
        RTM_FORCE_INLINE void serialize(Archive& ar)
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
        RTM_FORCE_INLINE bool operator==(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x == v._x && _y == v._y;
        }

        RTM_FORCE_INLINE bool operator!=(
            const generic_vec2_scalar& v) const noexcept
        {
            return _x != v._x || _y != v._y;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator=(
            const generic_vec2_scalar& v) noexcept
        {
            _x = v._x;
            _y = v._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator+=(
            const generic_vec2_scalar& v) noexcept
        {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator-=(
            const generic_vec2_scalar& v) noexcept
        {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator*=(
            const value_type& v) noexcept
        {
            _x *= v;
            _y *= v;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator*=(
            const generic_vec2_scalar& v) noexcept
        {
            _x *= v._x;
            _y *= v._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator/=(
            const value_type& v) noexcept
        {
            _x /= v;
            _y /= v;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& operator/=(
            const generic_vec2_scalar& v) noexcept
        {
            _x /= v._x;
            _y /= v._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator+(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x + v._x, _y + v._y);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator-(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x - v._x, _y - v._y);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator*(
            const value_type& v) const noexcept
        {
            return generic_vec2_scalar(_x * v, _y * v);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator*(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x * v._x, _y * v._y);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator/(
            const value_type& v) const noexcept
        {
            return generic_vec2_scalar(_x / v, _y / v);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator/(
            const generic_vec2_scalar& v) const noexcept
        {
            return generic_vec2_scalar(_x / v._x, _y / v._y);
        }

        RTM_FORCE_INLINE generic_vec2_scalar operator-() const noexcept
        {
            return generic_vec2_scalar(-_x, -_y);
        }

        RTM_FORCE_INLINE value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return _x;
                default:
                    return _y;
            }
        }

        RTM_FORCE_INLINE void set_component(int index, value_type value)
        {
            switch (index)
            {
                case 0:
                    set_x(value);
                    break;
                default:
                    set_y(value);
                    break;
            }
        }

        RTM_FORCE_INLINE value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

    public:
        RTM_FORCE_INLINE value_type x() const noexcept
        {
            return _x;
        }

        RTM_FORCE_INLINE value_type y() const noexcept
        {
            return _y;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& fill(value_type v) noexcept
        {
            _x = v;
            _y = v;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& set(
            value_type x, value_type y) noexcept
        {
            _x = x;
            _y = y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& set(
            const generic_vec2_scalar& rhs) noexcept
        {
            _x = rhs._x;
            _y = rhs._y;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& set_x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        RTM_FORCE_INLINE generic_vec2_scalar& set_y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        RTM_FORCE_INLINE value_type length() const noexcept
        {
            return rtm::scalar_sqrt(squared_length());
        }

        RTM_FORCE_INLINE value_type squared_length() const noexcept
        {
            return _x * _x + _y * _y;
        }

        RTM_FORCE_INLINE generic_vec2_scalar normalized() const noexcept
        {
            return *this / length();
        }

    public:
        RTM_FORCE_INLINE static value_type dot(const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return v1._x * v2._x + v1._y * v2._y;
        }

        RTM_FORCE_INLINE static value_type distance_between_points(
            const generic_vec2_scalar& point1,
            const generic_vec2_scalar& point2) noexcept
        {
            return (point2 - point1).length();
        }

        RTM_FORCE_INLINE static value_type squared_distance_between_points(
            const generic_vec2_scalar& point1,
            const generic_vec2_scalar& point2) noexcept
        {
            return (point2 - point1).squared_length();
        }

        RTM_FORCE_INLINE static value_type distance_to_line(
            const generic_vec2_scalar& point, const generic_vec2_scalar& v0,
            const generic_vec2_scalar& v1) noexcept
        {
            // Given a vector PointVector from LinePoint1 to Point and a vector
            // LineVector from LinePoint1 to LinePoint2, the scaled distance
            // PointProjectionScale from LinePoint1 to the perpendicular
            // projection of PointVector onto the line is defined as:
            //
            //     PointProjectionScale = dot(PointVector, LineVector) /
            //     LengthSq(LineVector)

            // XMVECTOR PointVector = XMVectorSubtract(Point, LinePoint1);
            // XMVECTOR LineVector = XMVectorSubtract(LinePoint2, LinePoint1);

            // XMVECTOR LengthSq = XMVector2LengthSq(LineVector);

            // XMVECTOR PointProjectionScale =
            //     XMVector2Dot(PointVector, LineVector);
            // PointProjectionScale =
            //     XMVectorDivide(PointProjectionScale, LengthSq);

            // XMVECTOR DistanceVector =
            //     XMVectorMultiply(LineVector, PointProjectionScale);
            // DistanceVector = XMVectorSubtract(PointVector, DistanceVector);

            // return XMVector2Length(DistanceVector);

            auto point_vector = point - v0;
            auto line_vector = v1 - v0;

            auto length_sq = line_vector.squared_length();

            auto point_projection_scale = dot(point_vector, line_vector);
            point_projection_scale /= length_sq;

            auto distance_vector = line_vector * point_projection_scale;
            distance_vector = point_vector - distance_vector;

            return distance_vector.length();
        }

        RTM_FORCE_INLINE static value_type cross(const generic_vec2_scalar& lhs,
            const generic_vec2_scalar& rhs) noexcept
        {
            return lhs._x * rhs._y - lhs._y * rhs._x;
        }

        RTM_FORCE_INLINE static value_type angle_between_normalized_vectors(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            // Compute with dot product
            auto res = dot(v1, v2);
            return rtm::scalar_acos(res);
        }

        RTM_FORCE_INLINE static value_type angle_between_vectors(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            // Compute with dot product
            auto norm = v1.normalized();
            auto vnorm = v2.normalized();
            return angle_between_normalized_vectors(norm, vnorm);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar reflect(
            const generic_vec2_scalar& incident,
            const generic_vec2_scalar& normal) noexcept
        {
            return incident - (normal * (2 * dot(incident, normal)));
        }

        RTM_FORCE_INLINE static generic_vec2_scalar refract(
            const generic_vec2_scalar& incident,
            const generic_vec2_scalar& normal, value_type ior) noexcept
        {
            // Based on XMVector2RefractV
            auto idotn = dot(incident, normal);

            auto ry = 1.0f - (idotn * idotn);
            auto rx = 1.0f - (ry * ior * ior);
            ry = 1.0f - (ry * ior * ior);

            if (rx >= 0.0f)
            {
                rx = (ior * incident.x()) -
                     (normal.x() * ((ior * idotn) + rtm::scalar_sqrt(rx)));
            }
            else
            {
                rx = 0.0f;
            }

            if (ry >= 0.0f)
            {
                ry = (ior * incident.y()) -
                     (normal.y() * ((ior * idotn) + rtm::scalar_sqrt(ry)));
            }
            else
            {
                ry = 0.0f;
            }

            return generic_vec2_scalar(rx, ry);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar min(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return generic_vec2_scalar(
                rtm::scalar_min(v1._x, v2._x), rtm::scalar_min(v1._y, v2._y));
        }

        RTM_FORCE_INLINE static generic_vec2_scalar max(
            const generic_vec2_scalar& v1,
            const generic_vec2_scalar& v2) noexcept
        {
            return generic_vec2_scalar(
                rtm::scalar_max(v1._x, v2._x), rtm::scalar_max(v1._y, v2._y));
        }

    public:
        RTM_FORCE_INLINE void normalize() noexcept
        {
            *this /= length();
        }

    public:
        RTM_FORCE_INLINE static generic_vec2_scalar zero() noexcept
        {
            return generic_vec2_scalar(0, 0);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar one() noexcept
        {
            return generic_vec2_scalar(1, 1);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar x_axis() noexcept
        {
            return generic_vec2_scalar(1, 0);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar y_axis() noexcept
        {
            return generic_vec2_scalar(0, 1);
        }

        RTM_FORCE_INLINE static generic_vec2_scalar left() noexcept
        {
            return -x_axis();
        }

        RTM_FORCE_INLINE static generic_vec2_scalar right() noexcept
        {
            return x_axis();
        }

        RTM_FORCE_INLINE static generic_vec2_scalar down() noexcept
        {
            return -y_axis();
        }

        RTM_FORCE_INLINE static generic_vec2_scalar up() noexcept
        {
            return y_axis();
        }

    private:
        value_type _x;
        value_type _y;
    };

    using vec2f = generic_vec2_scalar<float>;
    using vec2d = generic_vec2_scalar<double>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec2 = vec2d;
#else
    using vec2 = vec2f;
#endif

    template <typename value_type>
    inline std::ostream& operator<<(std::ostream& os,
        const move::vectormath::generic_vec2_scalar<value_type>& v)
    {
        return os << "(" << v.x() << ", " << v.y() << ")";
    }
}  // namespace move::vectormath

#if !defined(MOVE_VECTORMATH_NO_SERIALIZATION)
#include "vmathjson.hpp"
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2f);
MOVE_VECTORMATH_JSON_SERIALIZER(move::vectormath::vec2d);
#endif

template <typename value_type>
struct fmt::formatter<move::vectormath::generic_vec2_scalar<value_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(
        move::vectormath::generic_vec2_scalar<value_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {})", number.x(), number.y());
    }
};