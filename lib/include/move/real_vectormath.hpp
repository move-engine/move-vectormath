#pragma once
#include <DirectXMath.h>

namespace move
{
    struct vector2;
    struct vector3;
    struct vector4;
    struct quaternion;
    struct matrix4;
    struct irect
    {
        int32_t x;
        int32_t y;
        int32_t width;
        int32_t height;

        // Creators
        irect() noexcept : x(0), y(0), width(0), height(0)
        {
        }

        constexpr irect(int32_t ix, int32_t iy, int32_t iw, int32_t ih) noexcept
            : x(ix), y(iy), width(iw), height(ih)
        {
        }

        irect(const irect&) = default;
        irect& operator=(const irect&) = default;

        irect(irect&&) = default;
        irect& operator=(irect&&) = default;

        // Comparison operators
#if (__cplusplus >= 202002L)
        bool operator==(const rectangle&) const = default;
        auto operator<=>(const rectangle&) const = default;
#else
        bool operator==(const irect& r) const noexcept
        {
            return (x == r.x) && (y == r.y) && (width == r.width) &&
                   (height == r.height);
        }
        bool operator!=(const irect& r) const noexcept
        {
            return (x != r.x) || (y != r.y) || (width != r.width) ||
                   (height != r.height);
        }
#endif

        // rectangle operations
        vector2 location() const noexcept;
        vector2 center() const noexcept;

        bool is_empty() const noexcept
        {
            return (width == 0 && height == 0 && x == 0 && y == 0);
        }

        bool contains(int32_t ix, int32_t iy) const noexcept
        {
            return (x <= ix) && (ix < (x + width)) && (y <= iy) &&
                   (iy < (y + height));
        }
        bool contains(const vector2& point) const noexcept;
        bool contains(const irect& r) const noexcept
        {
            return (x <= r.x) && ((r.x + r.width) <= (x + width)) &&
                   (y <= r.y) && ((r.y + r.height) <= (y + height));
        }

        void inflate(int32_t horizAmount, int32_t vertAmount) noexcept;

        bool intersects(const irect& r) const noexcept
        {
            return (r.x < (x + width)) && (x < (r.x + r.width)) &&
                   (r.y < (y + height)) && (y < (r.y + r.height));
        }

        void translate_inplace(int32_t ox, int32_t oy) noexcept
        {
            x += ox;
            y += oy;
        }

        // Static functions
        static irect create_intersection(
            const irect& ra, const irect& rb) noexcept;

        static irect create_union(const irect& ra, const irect& rb) noexcept;
    };

    struct vector2 : DirectX::XMFLOAT2
    {
        vector2() noexcept : XMFLOAT2(0.f, 0.f)
        {
        }
        constexpr explicit vector2(float ix) noexcept : XMFLOAT2(ix, ix)
        {
        }
        constexpr vector2(float ix, float iy) noexcept : XMFLOAT2(ix, iy)
        {
        }
        explicit vector2(_In_reads_(2) const float* pArray) noexcept
            : XMFLOAT2(pArray)
        {
        }
        vector2(DirectX::FXMVECTOR V) noexcept
        {
            XMStoreFloat2(this, V);
        }
        vector2(const XMFLOAT2& V) noexcept
        {
            this->x = V.x;
            this->y = V.y;
        }
        explicit vector2(const DirectX::XMVECTORF32& F) noexcept
        {
            this->x = F.f[0];
            this->y = F.f[1];
        }

        vector2(const vector2&) = default;
        vector2& operator=(const vector2&) = default;

        vector2(vector2&&) = default;
        vector2& operator=(vector2&&) = default;

        operator DirectX::XMVECTOR() const noexcept
        {
            return XMLoadFloat2(this);
        }

        // Comparison operators
        bool operator==(const vector2& V) const noexcept;
        bool operator!=(const vector2& V) const noexcept;

        // Assignment operators
        vector2& operator=(const DirectX::XMVECTORF32& F) noexcept
        {
            x = F.f[0];
            y = F.f[1];
            return *this;
        }
        vector2& operator+=(const vector2& V) noexcept;
        vector2& operator-=(const vector2& V) noexcept;
        vector2& operator*=(const vector2& V) noexcept;
        vector2& operator*=(float S) noexcept;
        vector2& operator/=(float S) noexcept;

        // Unary operators
        vector2 operator+() const noexcept
        {
            return *this;
        }
        vector2 operator-() const noexcept
        {
            return vector2(-x, -y);
        }

        // Vector operations
        bool in_bounds(const vector2& bounds) const noexcept;

        float length() const noexcept;
        float length_squared() const noexcept;

        float dot(const vector2& v) const noexcept;
        vector2 cross(const vector2& v) const noexcept;
        static void cross(
            const vector2& v1, const vector2& v2, vector2& result) noexcept;

        void normalize() noexcept;
        vector2 normalized() const noexcept;
        static void normalize(const vector2& result) noexcept;

        void clamp(const vector2& vmin, const vector2& vmax) noexcept;
    };
}  // namespace move