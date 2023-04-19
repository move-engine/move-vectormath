#pragma once
#include <DirectXMath.h>
#include "common.hpp"

#if !defined(MOVE_FASTVEC4_HPP)
#define MOVE_FASTVEC4_HPP
#endif

namespace move::vectormath
{
    struct fastvec4
    {
        using XMVECTOR = fastmath::XMVECTOR;
        using CXMVECTOR = fastmath::CXMVECTOR;
        using XMFLOAT4 = fastmath::XMFLOAT4;

        inline fastvec4() noexcept : _vec(fastmath::XMVectorZero())
        {
        }

        inline fastvec4(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _vec(fastmath::XMVectorSet(x, y, z, w))
        {
        }

        inline fastvec4(const XMVECTOR& v) noexcept : _vec(v)
        {
        }

        inline fastvec4(const XMFLOAT4& v) noexcept
            : _vec(fastmath::XMLoadFloat4(&v))
        {
        }

    public:
        inline operator XMVECTOR&() noexcept
        {
            return _vec;
        }

        inline operator CXMVECTOR() const noexcept
        {
            return _vec;
        }

        inline operator XMFLOAT4() const noexcept
        {
            XMFLOAT4 v;
            XMStoreFloat4(&v, _vec);
            return v;
        }

        inline fastvec4& operator=(const XMVECTOR& v) noexcept
        {
            _vec = v;
            return *this;
        }

        inline fastvec4& operator=(const XMFLOAT4& v) noexcept
        {
            _vec = fastmath::XMLoadFloat4(&v);
            return *this;
        }

        inline fastvec4& operator+=(const fastvec4& v) noexcept
        {
            _vec = fastmath::XMVectorAdd(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator-=(const fastvec4& v) noexcept
        {
            _vec = fastmath::XMVectorSubtract(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator*=(const fastvec4& v) noexcept
        {
            _vec = fastmath::XMVectorMultiply(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator/=(const fastvec4& v) noexcept
        {
            _vec = fastmath::XMVectorDivide(_vec, v._vec);
            return *this;
        }

        inline fastvec4 operator+(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorAdd(_vec, v._vec);
        }

        inline fastvec4 operator-(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorSubtract(_vec, v._vec);
        }

        inline fastvec4 operator*(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorMultiply(_vec, v._vec);
        }

        inline fastvec4 operator/(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorDivide(_vec, v._vec);
        }

        inline fastvec4 operator-() const noexcept
        {
            return fastmath::XMVectorNegate(_vec);
        }

    public:
        inline float x() const noexcept
        {
            return fastmath::XMVectorGetX(_vec);
        }

        inline float y() const noexcept
        {
            return fastmath::XMVectorGetY(_vec);
        }

        inline float z() const noexcept
        {
            return fastmath::XMVectorGetZ(_vec);
        }

        inline float w() const noexcept
        {
            return fastmath::XMVectorGetW(_vec);
        }

        inline fastvec4& set_x(float x) noexcept
        {
            _vec = fastmath::XMVectorSetX(_vec, x);
            return *this;
        }

        inline fastvec4& set_y(float y) noexcept
        {
            _vec = fastmath::XMVectorSetY(_vec, y);
            return *this;
        }

        inline fastvec4& set_z(float z) noexcept
        {
            _vec = fastmath::XMVectorSetZ(_vec, z);
            return *this;
        }

        inline fastvec4& set_w(float w) noexcept
        {
            _vec = fastmath::XMVectorSetW(_vec, w);
            return *this;
        }

        inline float length() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector4Length(_vec));
        }

        inline float length_approximate() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector4LengthEst(_vec));
        }

        inline float squared_length() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector4LengthSq(_vec));
        }

        inline fastvec4 normalized() const noexcept
        {
            return fastmath::XMVector4Normalize(_vec);
        }

        inline fastvec4 normalized_approximate() const noexcept
        {
            return fastmath::XMVector4NormalizeEst(_vec);
        }

        inline float dot(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector4Dot(_vec, v._vec));
        }

        inline float distance_to_point(const fastvec4& v) const noexcept
        {
            return (v - *this).length();
        }

        inline float distance_to_point_approximate(
            const fastvec4& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        inline float squared_distance_to_point(const fastvec4& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        inline fastvec4 min(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorMin(_vec, v._vec);
        }

        inline fastvec4 max(const fastvec4& v) const noexcept
        {
            return fastmath::XMVectorMax(_vec, v._vec);
        }

    public:
        inline void normalize() noexcept
        {
            _vec = fastmath::XMVector4Normalize(_vec);
        }

        inline void normalize_approximate() noexcept
        {
            _vec = fastmath::XMVector4NormalizeEst(_vec);
        }

    public:
        static inline fastvec4 zero() noexcept
        {
            return fastmath::XMVectorZero();
        }

        static inline fastvec4 one() noexcept
        {
            return fastmath::XMVectorSplatOne();
        }

        static inline fastvec4 x_axis() noexcept
        {
            return fastmath::XMVectorSetX(fastmath::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 y_axis() noexcept
        {
            return fastmath::XMVectorSetY(fastmath::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 z_axis() noexcept
        {
            return fastmath::XMVectorSetZ(fastmath::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 w_axis() noexcept
        {
            return fastmath::XMVectorSetW(fastmath::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 forward() noexcept
        {
            return z_axis();
        }

        static inline fastvec4 right() noexcept
        {
            return x_axis();
        }

        static inline fastvec4 up() noexcept
        {
            return y_axis();
        }

    private:
        XMVECTOR _vec;
    };
}  // namespace move::vectormath