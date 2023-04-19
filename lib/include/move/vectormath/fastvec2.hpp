#pragma once
#include "DirectXMath.h"
#include "common.hpp"

#if !defined(MOVE_FASTVEC2_HPP)
#define MOVE_FASTVEC2_HPP
#endif

namespace move::vectormath
{
    struct fastvec2
    {
        using XMVECTOR = fastmath::XMVECTOR;
        using CXMVECTOR = fastmath::CXMVECTOR;
        using XMFLOAT2 = fastmath::XMFLOAT2;

        inline fastvec2() noexcept : _vec(fastmath::XMVectorZero())
        {
        }

        inline fastvec2(float x, float y = 0) noexcept
            : _vec(fastmath::XMVectorSet(x, y, 0, 0))
        {
        }

        inline fastvec2(const XMVECTOR& v) noexcept : _vec(v)
        {
        }

        inline fastvec2(const XMFLOAT2& v) noexcept
            : _vec(fastmath::XMLoadFloat2(&v))
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

        inline void store(XMFLOAT2& v) const noexcept
        {
            fastmath::XMStoreFloat2(&v, _vec);
        }

        inline operator XMFLOAT2() const noexcept
        {
            XMFLOAT2 res;
            fastmath::XMStoreFloat2(&res, _vec);
            return res;
        }

        inline bool operator==(const fastvec2& v) const noexcept
        {
            return fastmath::XMVector2Equal(_vec, v._vec);
        }

        inline bool operator!=(const fastvec2& v) const noexcept
        {
            return fastmath::XMVector2NotEqual(_vec, v._vec);
        }

        inline fastvec2& operator=(const XMVECTOR& v) noexcept
        {
            _vec = v;
            return *this;
        }

        inline fastvec2& operator=(const XMFLOAT2& v) noexcept
        {
            _vec = fastmath::XMLoadFloat2(&v);
            return *this;
        }

        inline fastvec2& operator+=(const fastvec2& v) noexcept
        {
            _vec = fastmath::XMVectorAdd(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator-=(const fastvec2& v) noexcept
        {
            _vec = fastmath::XMVectorSubtract(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator*=(const fastvec2& v) noexcept
        {
            _vec = fastmath::XMVectorMultiply(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator/=(const fastvec2& v) noexcept
        {
            _vec = fastmath::XMVectorDivide(_vec, v._vec);
            return *this;
        }

        inline fastvec2 operator+(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorAdd(_vec, v._vec);
        }

        inline fastvec2 operator-(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorSubtract(_vec, v._vec);
        }

        inline fastvec2 operator*(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorMultiply(_vec, v._vec);
        }

        inline fastvec2 operator/(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorDivide(_vec, v._vec);
        }

        inline fastvec2 operator-() const noexcept
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

        inline fastvec2& set_x(float x) noexcept
        {
            _vec = fastmath::XMVectorSetX(_vec, x);
            return *this;
        }

        inline fastvec2& set_y(float y) noexcept
        {
            _vec = fastmath::XMVectorSetY(_vec, y);
            return *this;
        }

        inline float length() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector2Length(_vec));
        }

        inline float length_approximate() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector2LengthEst(_vec));
        }

        inline float squared_length() const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector2LengthSq(_vec));
        }

        inline fastvec2 normalized() const noexcept
        {
            return fastmath::XMVector2Normalize(_vec);
        }

        inline fastvec2 normalized_approximate() const noexcept
        {
            return fastmath::XMVector2NormalizeEst(_vec);
        }

        inline float dot(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector2Dot(_vec, v._vec));
        }

        inline float distance_to_point(const fastvec2& v) const noexcept
        {
            return (v - *this).length();
        }

        inline float distance_to_point_approximate(
            const fastvec2& v) const noexcept
        {
            return (v - *this).length_approximate();
        }

        inline float squared_distance_to_point(const fastvec2& v) const noexcept
        {
            return (v - *this).squared_length();
        }

        inline fastvec2 cross(const fastvec2& v) const noexcept
        {
            return fastmath::XMVector2Cross(_vec, v._vec);
        }

        inline float angle_between_normals(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorGetX(
                fastmath::XMVector2AngleBetweenNormals(_vec, v._vec));
        }

        inline float angle_between_vectors(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorGetX(
                fastmath::XMVector2AngleBetweenVectors(_vec, v._vec));
        }

        inline fastvec2 reflect(const fastvec2& normal) const noexcept
        {
            return fastmath::XMVector2Reflect(_vec, normal._vec);
        }

        inline fastvec2 refract(
            const fastvec2& normal, float ior) const noexcept
        {
            return fastmath::XMVector2Refract(_vec, normal._vec, ior);
        }

        inline fastvec2 min(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorMin(_vec, v._vec);
        }

        inline fastvec2 max(const fastvec2& v) const noexcept
        {
            return fastmath::XMVectorMax(_vec, v._vec);
        }

        inline float distance_to_line(
            const fastvec2& point1, const fastvec2& point2) const noexcept
        {
            return fastmath::XMVectorGetX(fastmath::XMVector2LinePointDistance(
                _vec, point1._vec, point2._vec));
        }

    public:
        inline void normalize() noexcept
        {
            _vec = fastmath::XMVector2Normalize(_vec);
        }

        inline void normalize_approximate() noexcept
        {
            _vec = fastmath::XMVector2NormalizeEst(_vec);
        }

    public:
        static inline fastvec2 zero() noexcept
        {
            return fastmath::XMVectorZero();
        }

        static inline fastvec2 one() noexcept
        {
            return fastmath::XMVectorSet(1, 1, 0, 0);
        }

        static inline fastvec2 x_axis() noexcept
        {
            return fastmath::XMVectorSetX(zero(), 1.0f);
        }

        static inline fastvec2 y_axis() noexcept
        {
            return fastmath::XMVectorSetY(zero(), 1.0f);
        }

        static inline fastvec2 right() noexcept
        {
            return x_axis();
        }

        static inline fastvec2 up() noexcept
        {
            return y_axis();
        }

        inline static fastvec2 intersect_line(const fastvec2& line1point1,
            const fastvec2& line1point2, const fastvec2& line2point1,
            const fastvec2& line2point2) noexcept
        {
            return fastmath::XMVector2IntersectLine(line1point1._vec,
                line1point2._vec, line2point1._vec, line2point2._vec);
        }

    public:
        inline fastvec2 operator*(const fastquat& v) const noexcept;

    protected:
        XMVECTOR _vec;
    };

    struct fastnorm2 : public fastvec2
    {
    public:
        inline fastnorm2(const fastvec2& v) noexcept : fastvec2(v.normalized())
        {
        }

        inline fastnorm2(float x, float y) noexcept
            : fastvec2(fastmath::XMVector2Normalize(
                  fastmath::XMVectorSet(x, y, 0.0f, 0.0f)))
        {
        }
    };
}  // namespace move::vectormath