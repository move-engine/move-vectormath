#pragma once
#include "DirectXMath.h"
#include "common.hpp"

#if !defined(MOVE_FASTVEC3_HPP)
#define MOVE_FASTVEC3_HPP
#endif

namespace move::vectormath
{
    struct fastvec3
    {
        using XMVECTOR = internal::DirectX::XMVECTOR;
        using CXMVECTOR = internal::DirectX::CXMVECTOR;
        using XMFLOAT3 = internal::DirectX::XMFLOAT3;

        inline fastvec3() noexcept : _vec(internal::DirectX::XMVectorZero())
        {
        }

        inline fastvec3(float x, float y = 0, float z = 0, float w = 0) noexcept
            : _vec(internal::DirectX::XMVectorSet(x, y, z, w))
        {
        }

        inline fastvec3(const XMVECTOR& v) noexcept : _vec(v)
        {
        }

        inline fastvec3(const XMFLOAT3& v) noexcept
            : _vec(internal::DirectX::XMLoadFloat3(&v))
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

        inline void store(XMFLOAT3& v) const noexcept
        {
            internal::DirectX::XMStoreFloat3(&v, _vec);
        }

        inline operator XMFLOAT3() const noexcept
        {
            XMFLOAT3 res;
            internal::DirectX::XMStoreFloat3(&res, _vec);
            return res;
        }

        inline bool operator==(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVector3Equal(_vec, v._vec);
        }

        inline bool operator!=(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVector3NotEqual(_vec, v._vec);
        }

        inline fastvec3& operator=(const XMVECTOR& v) noexcept
        {
            _vec = v;
            return *this;
        }

        inline fastvec3& operator=(const XMFLOAT3& v) noexcept
        {
            _vec = internal::DirectX::XMLoadFloat3(&v);
            return *this;
        }

        inline fastvec3& operator+=(const fastvec3& v) noexcept
        {
            _vec = internal::DirectX::XMVectorAdd(_vec, v._vec);
            return *this;
        }

        inline fastvec3& operator-=(const fastvec3& v) noexcept
        {
            _vec = internal::DirectX::XMVectorSubtract(_vec, v._vec);
            return *this;
        }

        inline fastvec3& operator*=(const fastvec3& v) noexcept
        {
            _vec = internal::DirectX::XMVectorMultiply(_vec, v._vec);
            return *this;
        }

        inline fastvec3& operator/=(const fastvec3& v) noexcept
        {
            _vec = internal::DirectX::XMVectorDivide(_vec, v._vec);
            return *this;
        }

        inline fastvec3 operator+(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorAdd(_vec, v._vec);
        }

        inline fastvec3 operator-(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorSubtract(_vec, v._vec);
        }

        inline fastvec3 operator*(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorMultiply(_vec, v._vec);
        }

        inline fastvec3 operator/(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorDivide(_vec, v._vec);
        }

        inline fastvec3 operator-() const noexcept
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

        inline fastvec3& set_x(float x) noexcept
        {
            _vec = internal::DirectX::XMVectorSetX(_vec, x);
            return *this;
        }

        inline fastvec3& set_y(float y) noexcept
        {
            _vec = internal::DirectX::XMVectorSetY(_vec, y);
            return *this;
        }

        inline fastvec3& set_z(float z) noexcept
        {
            _vec = internal::DirectX::XMVectorSetZ(_vec, z);
            return *this;
        }

        inline float length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3Length(_vec));
        }

        inline float length_approximate() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3LengthEst(_vec));
        }

        inline float squared_length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3LengthSq(_vec));
        }

        inline fastvec3 normalized() const noexcept
        {
            return internal::DirectX::XMVector3Normalize(_vec);
        }

        inline fastvec3 normalized_approximate() const noexcept
        {
            return internal::DirectX::XMVector3NormalizeEst(_vec);
        }

        inline float dot(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3Dot(_vec, v._vec));
        }

        inline float distance_to_point(const fastvec3& v) const noexcept
        {
            return (v - *this).length();
        }

        inline float distance_to_point_approximate(
            const fastvec3& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        inline float squared_distance_to_point(const fastvec3& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        inline float distance_to_line(
            const fastvec3& point1, const fastvec3& point2) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3LinePointDistance(
                    _vec, point1._vec, point2._vec));
        }

        inline fastvec3 cross(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVector3Cross(_vec, v._vec);
        }

        inline float angle_between_normals(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3AngleBetweenNormals(_vec, v._vec));
        }

        inline float angle_between_vectors(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector3AngleBetweenVectors(_vec, v._vec));
        }

        inline fastvec3 reflect(const fastvec3& normal) const noexcept
        {
            return internal::DirectX::XMVector3Reflect(_vec, normal._vec);
        }

        inline fastvec3 refract(
            const fastvec3& normal, float ior) const noexcept
        {
            return internal::DirectX::XMVector3Refract(_vec, normal._vec, ior);
        }

        inline fastvec3 min(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorMin(_vec, v._vec);
        }

        inline fastvec3 max(const fastvec3& v) const noexcept
        {
            return internal::DirectX::XMVectorMax(_vec, v._vec);
        }

    public:
        inline void normalize() noexcept
        {
            _vec = internal::DirectX::XMVector3Normalize(_vec);
        }

        inline void normalize_approximate() noexcept
        {
            _vec = internal::DirectX::XMVector3NormalizeEst(_vec);
        }

    public:
        static inline fastvec3 zero() noexcept
        {
            return internal::DirectX::XMVectorZero();
        }

        static inline fastvec3 one() noexcept
        {
            return internal::DirectX::XMVectorSetW(
                internal::DirectX::XMVectorSplatOne(), 0.0f);
        }

        static inline fastvec3 x_axis() noexcept
        {
            return internal::DirectX::XMVectorSetX(zero(), 1.0f);
        }

        static inline fastvec3 y_axis() noexcept
        {
            return internal::DirectX::XMVectorSetY(zero(), 1.0f);
        }

        static inline fastvec3 z_axis() noexcept
        {
            return internal::DirectX::XMVectorSetZ(zero(), 1.0f);
        }

        static inline fastvec3 w_axis() noexcept
        {
            return internal::DirectX::XMVectorSetW(zero(), 1.0f);
        }
        static inline fastvec3 right() noexcept
        {
            return x_axis();
        }

        static inline fastvec3 up() noexcept
        {
            return y_axis();
        }

        static inline fastvec3 forward() noexcept
        {
            return z_axis();
        }

    public:
        static inline void transform_array(const fastvec3* source,
            fastvec3* destination, size_t count,
            const fastmat4& transform) noexcept;
        // {
        //     internal::DirectX::XMVector3TransformCoordStream(destination,
        //         sizeof(fastvec3), source, sizeof(fastvec3), count,
        //         transform._mat);
        // }

    public:
        inline fastvec3 operator*(const fastquat& v) const noexcept;

    protected:
        XMVECTOR _vec;
    };

    struct fastnorm3 : public fastvec3
    {
    public:
        inline fastnorm3(const fastvec3& v) noexcept : fastvec3(v.normalized())
        {
        }

        inline fastnorm3(float x, float y, float z) noexcept
            : fastvec3(internal::DirectX::XMVector3Normalize(
                  internal::DirectX::XMVectorSet(x, y, z, 0.0f)))
        {
        }
    };
}  // namespace move::vectormath