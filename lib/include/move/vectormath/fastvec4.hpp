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
        using XMVECTOR = internal::DirectX::XMVECTOR;
        using CXMVECTOR = internal::DirectX::CXMVECTOR;
        using XMFLOAT4 = internal::DirectX::XMFLOAT4;

        inline fastvec4() noexcept : _vec(internal::DirectX::XMVectorZero())
        {
        }

        inline fastvec4(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _vec(internal::DirectX::XMVectorSet(x, y, z, w))
        {
        }

        inline fastvec4(const XMVECTOR& v) noexcept : _vec(v)
        {
        }

        inline fastvec4(const XMFLOAT4& v) noexcept
            : _vec(internal::DirectX::XMLoadFloat4(&v))
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

        inline void store(XMFLOAT4& v) const noexcept
        {
            XMStoreFloat4(&v, _vec);
        }

        inline operator XMFLOAT4() const noexcept
        {
            XMFLOAT4 v;
            XMStoreFloat4(&v, _vec);
            return v;
        }

        inline bool operator==(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVector4Equal(_vec, v._vec);
        }

        inline bool operator!=(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVector4NotEqual(_vec, v._vec);
        }

        inline fastvec4& operator=(const XMVECTOR& v) noexcept
        {
            _vec = v;
            return *this;
        }

        inline fastvec4& operator=(const XMFLOAT4& v) noexcept
        {
            _vec = internal::DirectX::XMLoadFloat4(&v);
            return *this;
        }

        inline fastvec4& operator+=(const fastvec4& v) noexcept
        {
            _vec = internal::DirectX::XMVectorAdd(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator-=(const fastvec4& v) noexcept
        {
            _vec = internal::DirectX::XMVectorSubtract(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator*=(const fastvec4& v) noexcept
        {
            _vec = internal::DirectX::XMVectorMultiply(_vec, v._vec);
            return *this;
        }

        inline fastvec4& operator/=(const fastvec4& v) noexcept
        {
            _vec = internal::DirectX::XMVectorDivide(_vec, v._vec);
            return *this;
        }

        inline fastvec4 operator+(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorAdd(_vec, v._vec);
        }

        inline fastvec4 operator-(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorSubtract(_vec, v._vec);
        }

        inline fastvec4 operator*(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorMultiply(_vec, v._vec);
        }

        inline fastvec4 operator/(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorDivide(_vec, v._vec);
        }

        inline fastvec4 operator-() const noexcept
        {
            return internal::DirectX::XMVectorNegate(_vec);
        }

    public:
        inline float x() const noexcept
        {
            return internal::DirectX::XMVectorGetX(_vec);
        }

        inline float y() const noexcept
        {
            return internal::DirectX::XMVectorGetY(_vec);
        }

        inline float z() const noexcept
        {
            return internal::DirectX::XMVectorGetZ(_vec);
        }

        inline float w() const noexcept
        {
            return internal::DirectX::XMVectorGetW(_vec);
        }

        inline fastvec4& set_x(float x) noexcept
        {
            _vec = internal::DirectX::XMVectorSetX(_vec, x);
            return *this;
        }

        inline fastvec4& set_y(float y) noexcept
        {
            _vec = internal::DirectX::XMVectorSetY(_vec, y);
            return *this;
        }

        inline fastvec4& set_z(float z) noexcept
        {
            _vec = internal::DirectX::XMVectorSetZ(_vec, z);
            return *this;
        }

        inline fastvec4& set_w(float w) noexcept
        {
            _vec = internal::DirectX::XMVectorSetW(_vec, w);
            return *this;
        }

        inline float length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4Length(_vec));
        }

        inline float length_approximate() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4LengthEst(_vec));
        }

        inline float squared_length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4LengthSq(_vec));
        }

        inline fastvec4 normalized() const noexcept
        {
            return internal::DirectX::XMVector4Normalize(_vec);
        }

        inline fastvec4 normalized_approximate() const noexcept
        {
            return internal::DirectX::XMVector4NormalizeEst(_vec);
        }

        inline float dot(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4Dot(_vec, v._vec));
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

        inline fastvec4 cross(
            const fastvec4& v0, const fastvec4& v1) const noexcept
        {
            return internal::DirectX::XMVector4Cross(_vec, v0._vec, v1._vec);
        }

        inline float angle_between_normals(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4AngleBetweenNormals(_vec, v._vec));
        }

        inline float angle_between_vectors(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector4AngleBetweenVectors(_vec, v._vec));
        }

        inline fastvec4 reflect(const fastvec4& normal) const noexcept
        {
            return internal::DirectX::XMVector4Reflect(_vec, normal._vec);
        }

        inline fastvec4 refract(
            const fastvec4& normal, float eta) const noexcept
        {
            return internal::DirectX::XMVector4Refract(_vec, normal._vec, eta);
        }

        inline fastvec4 min(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorMin(_vec, v._vec);
        }

        inline fastvec4 max(const fastvec4& v) const noexcept
        {
            return internal::DirectX::XMVectorMax(_vec, v._vec);
        }

    public:
        inline void normalize() noexcept
        {
            _vec = internal::DirectX::XMVector4Normalize(_vec);
        }

        inline void normalize_approximate() noexcept
        {
            _vec = internal::DirectX::XMVector4NormalizeEst(_vec);
        }

    public:
        static inline fastvec4 zero() noexcept
        {
            return internal::DirectX::XMVectorZero();
        }

        static inline fastvec4 one() noexcept
        {
            return internal::DirectX::XMVectorSplatOne();
        }

        static inline fastvec4 x_axis() noexcept
        {
            return internal::DirectX::XMVectorSetX(
                internal::DirectX::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 y_axis() noexcept
        {
            return internal::DirectX::XMVectorSetY(
                internal::DirectX::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 z_axis() noexcept
        {
            return internal::DirectX::XMVectorSetZ(
                internal::DirectX::XMVectorZero(), 1.0f);
        }

        static inline fastvec4 w_axis() noexcept
        {
            return internal::DirectX::XMVectorSetW(
                internal::DirectX::XMVectorZero(), 1.0f);
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