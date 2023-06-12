#pragma once
#include "DirectXMath.h"
#include "common.hpp"

#if !defined(MOVE_FASTVEC2_HPP)
#define MOVE_FASTVEC2_HPP
#endif

namespace move::vectormath
{
    /**
     *  @brief A 2D vector class optimized for speed.
     *  A 2D vector class optimized for speed.
     *  This class is a wrapper around the DirectXMath XMVECTOR type. It is
     *  designed to be used in places where speed is more important than
     *  convenience. As such, it does not provide any of the conveniences of
     *  the vec2 class. It does, however, provide a number of functions that
     *  operate on fastvec2s, and it can be converted to and from vec2s.
     *  @see vec2
     */
    struct fastvec2
    {
        using XMVECTOR = internal::DirectX::XMVECTOR;
        using CXMVECTOR = internal::DirectX::CXMVECTOR;
        using XMFLOAT2 = internal::DirectX::XMFLOAT2;

        /**
         * @brief Default constructor.  Initializes to (0, 0).
         * Default constructor.  Initializes the vector to (0, 0).
         */
        inline fastvec2() noexcept : _vec(internal::DirectX::XMVectorZero())
        {
        }

        /**
         * @brief Initializes the vector to (x, y).
         * Initializes the vector to (x, y).
         * @param x The x component of the vector.
         * @param y The y component of the vector.
         */
        inline fastvec2(float x, float y = 0) noexcept
            : _vec(internal::DirectX::XMVectorSet(x, y, 0, 0))
        {
        }

        /**
         * @brief Initializes the vector to (v.x, v.y).
         * Initializes the vector to (v.x, v.y).
         * @param v The vector to copy.
         */
        inline fastvec2(const XMVECTOR& v) noexcept : _vec(v)
        {
        }

        /**
         * @brief Initializes the vector to (v.x, v.y).
         * Initializes the vector to (v.x, v.y).
         * @param v The vector to copy.
         */
        inline fastvec2(const XMFLOAT2& v) noexcept
            : _vec(internal::DirectX::XMLoadFloat2(&v))
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
            internal::DirectX::XMStoreFloat2(&v, _vec);
        }

        inline operator XMFLOAT2() const noexcept
        {
            XMFLOAT2 res;
            internal::DirectX::XMStoreFloat2(&res, _vec);
            return res;
        }

        inline bool operator==(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVector2Equal(_vec, v._vec);
        }

        inline bool operator!=(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVector2NotEqual(_vec, v._vec);
        }

        inline fastvec2& operator=(const XMVECTOR& v) noexcept
        {
            _vec = v;
            return *this;
        }

        inline fastvec2& operator=(const XMFLOAT2& v) noexcept
        {
            _vec = internal::DirectX::XMLoadFloat2(&v);
            return *this;
        }

        inline fastvec2& operator+=(const fastvec2& v) noexcept
        {
            _vec = internal::DirectX::XMVectorAdd(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator-=(const fastvec2& v) noexcept
        {
            _vec = internal::DirectX::XMVectorSubtract(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator*=(const fastvec2& v) noexcept
        {
            _vec = internal::DirectX::XMVectorMultiply(_vec, v._vec);
            return *this;
        }

        inline fastvec2& operator/=(const fastvec2& v) noexcept
        {
            _vec = internal::DirectX::XMVectorDivide(_vec, v._vec);
            return *this;
        }

        inline fastvec2 operator+(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorAdd(_vec, v._vec);
        }

        inline fastvec2 operator-(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorSubtract(_vec, v._vec);
        }

        inline fastvec2 operator*(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorMultiply(_vec, v._vec);
        }

        inline fastvec2 operator/(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorDivide(_vec, v._vec);
        }

        inline fastvec2 operator-() const noexcept
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

        inline fastvec2& set_x(float x) noexcept
        {
            _vec = internal::DirectX::XMVectorSetX(_vec, x);
            return *this;
        }

        inline fastvec2& set_y(float y) noexcept
        {
            _vec = internal::DirectX::XMVectorSetY(_vec, y);
            return *this;
        }

        inline float length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2Length(_vec));
        }

        inline float length_approximate() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2LengthEst(_vec));
        }

        inline float squared_length() const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2LengthSq(_vec));
        }

        inline fastvec2 normalized() const noexcept
        {
            return internal::DirectX::XMVector2Normalize(_vec);
        }

        inline fastvec2 normalized_approximate() const noexcept
        {
            return internal::DirectX::XMVector2NormalizeEst(_vec);
        }

        inline float dot(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2Dot(_vec, v._vec));
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
            return internal::DirectX::XMVector2Cross(_vec, v._vec);
        }

        inline float angle_between_normals(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2AngleBetweenNormals(_vec, v._vec));
        }

        inline float angle_between_vectors(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2AngleBetweenVectors(_vec, v._vec));
        }

        inline fastvec2 reflect(const fastvec2& normal) const noexcept
        {
            return internal::DirectX::XMVector2Reflect(_vec, normal._vec);
        }

        inline fastvec2 refract(
            const fastvec2& normal, float ior) const noexcept
        {
            return internal::DirectX::XMVector2Refract(_vec, normal._vec, ior);
        }

        inline fastvec2 min(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorMin(_vec, v._vec);
        }

        inline fastvec2 max(const fastvec2& v) const noexcept
        {
            return internal::DirectX::XMVectorMax(_vec, v._vec);
        }

        inline float distance_to_line(
            const fastvec2& point1, const fastvec2& point2) const noexcept
        {
            return internal::DirectX::XMVectorGetX(
                internal::DirectX::XMVector2LinePointDistance(
                    _vec, point1._vec, point2._vec));
        }

    public:
        inline void normalize() noexcept
        {
            _vec = internal::DirectX::XMVector2Normalize(_vec);
        }

        inline void normalize_approximate() noexcept
        {
            _vec = internal::DirectX::XMVector2NormalizeEst(_vec);
        }

    public:
        static inline fastvec2 zero() noexcept
        {
            return internal::DirectX::XMVectorZero();
        }

        static inline fastvec2 one() noexcept
        {
            return internal::DirectX::XMVectorSet(1, 1, 0, 0);
        }

        static inline fastvec2 x_axis() noexcept
        {
            return internal::DirectX::XMVectorSetX(zero(), 1.0f);
        }

        static inline fastvec2 y_axis() noexcept
        {
            return internal::DirectX::XMVectorSetY(zero(), 1.0f);
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
            return internal::DirectX::XMVector2IntersectLine(line1point1._vec,
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
            : fastvec2(internal::DirectX::XMVector2Normalize(
                  internal::DirectX::XMVectorSet(x, y, 0.0f, 0.0f)))
        {
        }
    };
}  // namespace move::vectormath