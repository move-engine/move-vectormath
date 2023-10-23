#pragma once
#include <ostream>

#include <rtm/scalard.h>

namespace move::vectormath
{
    template <typename value_type>
    struct generic_vec2
    {
    public:
        using component_type = value_type;
        constexpr static uint32_t num_elements = 2;

        inline generic_vec2() noexcept : _x(0), _y(0)
        {
        }

        inline generic_vec2(value_type x, value_type y = 0) noexcept
            : _x(x), _y(y)
        {
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
        inline bool operator==(const generic_vec2& v) const noexcept
        {
            return _x == v._x && _y == v._y;
        }

        inline bool operator!=(const generic_vec2& v) const noexcept
        {
            return _x != v._x || _y != v._y;
        }

        inline generic_vec2& operator=(const generic_vec2& v) noexcept
        {
            _x = v._x;
            _y = v._y;
            return *this;
        }

        inline generic_vec2& operator+=(const generic_vec2& v) noexcept
        {
            _x += v._x;
            _y += v._y;
            return *this;
        }

        inline generic_vec2& operator-=(const generic_vec2& v) noexcept
        {
            _x -= v._x;
            _y -= v._y;
            return *this;
        }

        inline generic_vec2& operator*=(const value_type& v) noexcept
        {
            _x *= v;
            _y *= v;
            return *this;
        }

        inline generic_vec2& operator*=(const generic_vec2& v) noexcept
        {
            _x *= v._x;
            _y *= v._y;
            return *this;
        }

        inline generic_vec2& operator/=(const value_type& v) noexcept
        {
            _x /= v;
            _y /= v;
            return *this;
        }

        inline generic_vec2& operator/=(const generic_vec2& v) noexcept
        {
            _x /= v._x;
            _y /= v._y;
            return *this;
        }

        inline generic_vec2 operator+(const generic_vec2& v) const noexcept
        {
            return generic_vec2(_x + v._x, _y + v._y);
        }

        inline generic_vec2 operator-(const generic_vec2& v) const noexcept
        {
            return generic_vec2(_x - v._x, _y - v._y);
        }

        inline generic_vec2 operator*(const value_type& v) const noexcept
        {
            return generic_vec2(_x * v, _y * v);
        }

        inline generic_vec2 operator*(const generic_vec2& v) const noexcept
        {
            return generic_vec2(_x * v._x, _y * v._y);
        }

        inline generic_vec2 operator/(const value_type& v) const noexcept
        {
            return generic_vec2(_x / v, _y / v);
        }

        inline generic_vec2 operator/(const generic_vec2& v) const noexcept
        {
            return generic_vec2(_x / v._x, _y / v._y);
        }

        inline generic_vec2 operator-() const noexcept
        {
            return generic_vec2(-_x, -_y);
        }

        inline value_type get_component(int index) const noexcept
        {
            switch (index)
            {
                case 0:
                    return _x;
                default:
                    return _y;
            }
        }

        inline void set_component(int index, value_type value)
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

        inline value_type operator[](int i) const noexcept
        {
            return get_component(i);
        }

    public:
        inline value_type x() const noexcept
        {
            return _x;
        }

        inline value_type y() const noexcept
        {
            return _y;
        }

        inline generic_vec2& x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        inline generic_vec2& y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        inline generic_vec2& fill(value_type v) noexcept
        {
            _x = v;
            _y = v;
            return *this;
        }

        inline generic_vec2& set(value_type x, value_type y) noexcept
        {
            _x = x;
            _y = y;
            return *this;
        }

        inline generic_vec2& set(const generic_vec2& rhs) noexcept
        {
            _x = rhs._x;
            _y = rhs._y;
            return *this;
        }

        inline generic_vec2& set_x(value_type x) noexcept
        {
            _x = x;
            return *this;
        }

        inline generic_vec2& set_y(value_type y) noexcept
        {
            _y = y;
            return *this;
        }

        inline value_type length() const noexcept
        {
            return rtm::scalar_sqrt(squared_length());
        }

        inline value_type length_approximate() const noexcept
        {
            // TODO: Implement approximate length
            return length();
        }

        inline value_type squared_length() const noexcept
        {
            return _x * _x + _y * _y;
        }

        inline generic_vec2 normalized() const noexcept
        {
            return *this / length();
        }

        inline generic_vec2 normalized_approximate() const noexcept
        {
            return *this / length_approximate();
        }

        inline value_type dot(const generic_vec2& v) const noexcept
        {
            return _x * v._x + _y * v._y;
        }

        inline value_type distance_to_point(
            const generic_vec2& v) const noexcept
        {
            return (v - *this).length();
        }

        inline value_type distance_to_point_approximate(
            const generic_vec2& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        inline value_type squared_distance_to_point(
            const generic_vec2& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        inline value_type distance_to_line(
            const generic_vec2& v0, const generic_vec2& v1) const noexcept
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

            auto point_vector = *this - v0;
            auto line_vector = v1 - v0;

            auto length_sq = line_vector.squared_length();

            auto point_projection_scale = point_vector.dot(line_vector);
            point_projection_scale /= length_sq;

            auto distance_vector = line_vector * point_projection_scale;
            distance_vector = point_vector - distance_vector;

            return distance_vector.length();
        }

        inline value_type cross(const generic_vec2& rhs) const noexcept
        {
            return _x * rhs._y - _y * rhs._x;
        }

        inline value_type angle_between_normalized_vectors(
            const generic_vec2& v) const noexcept
        {
            // Compute with dot product
            auto dot = this->dot(v);
            return rtm::scalar_acos(dot);
        }

        inline value_type angle_between_vectors(
            const generic_vec2& v) const noexcept
        {
            // Compute with dot product
            auto norm = normalized();
            auto vnorm = v.normalized();
            return norm.angle_between_normalized_vectors(vnorm);
        }

        inline generic_vec2 reflect(const generic_vec2& normal) const noexcept
        {
            return *this - (normal * (2 * dot(normal)));
        }

        inline generic_vec2 refract(
            const generic_vec2& normal, value_type ior) const noexcept
        {
            // Based on XMVector2RefractV
            const auto& incident = *this;
            auto idotn = incident.dot(normal);

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

            return generic_vec2(rx, ry);
        }

        inline generic_vec2 min(const generic_vec2& v) const noexcept
        {
            return generic_vec2(
                rtm::scalar_min(_x, v._x), rtm::scalar_min(_y, v._y));
        }

        inline generic_vec2 max(const generic_vec2& v) const noexcept
        {
            return generic_vec2(
                rtm::scalar_max(_x, v._x), rtm::scalar_max(_y, v._y));
        }

    public:
        inline void normalize() noexcept
        {
            *this /= length();
        }

        inline void normalize_approximate() noexcept
        {
            *this /= length_approximate();
        }

    public:
        inline static generic_vec2 zero() noexcept
        {
            return generic_vec2(0, 0);
        }

        inline static generic_vec2 one() noexcept
        {
            return generic_vec2(1, 1);
        }

        inline static generic_vec2 x_axis() noexcept
        {
            return generic_vec2(1, 0);
        }

        inline static generic_vec2 y_axis() noexcept
        {
            return generic_vec2(0, 1);
        }

        inline static generic_vec2 right() noexcept
        {
            return x_axis();
        }

        inline static generic_vec2 up() noexcept
        {
            return y_axis();
        }

    private:
        float _x;
        float _y;
    };

    using vec2f = generic_vec2<float>;
    using vec2d = generic_vec2<double>;

#if MOVE_VECTORMATH_USE_DOUBLE_PRECISION
    using vec2 = vec2d;
#else
    using vec2 = vec2f;
#endif

    template <typename value_type>
    inline std::ostream& operator<<(
        std::ostream& os, const move::vectormath::generic_vec2<value_type>& v)
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
struct fmt::formatter<move::vectormath::generic_vec2<value_type>>
{
    template <typename ParseContext>
    constexpr inline auto parse(ParseContext& ctx)
    {
        return ctx.begin();
    }

    template <typename FormatContext>
    auto inline format(move::vectormath::generic_vec2<value_type> const& number,
        FormatContext& ctx)
    {
        return format_to(ctx.out(), "({}, {})", number.x(), number.y());
    }
};