#pragma once

#include <cmath>
#include <cstdint>
#include <optional>
#include <type_traits>
#include <utility>

#include <mv/math/Rotation3.hpp>
#include <mv/math/detail/Normalization.hpp>

namespace mv::math
{
    template <typename T>
        requires std::is_floating_point_v<T>
    class Plane3;

    enum class RefractionStatus : std::uint8_t
    {
        Success,
        TotalInternalReflection,
        InvalidIndexRatio,
        NumericalFailure
    };

    template <typename T>
        requires std::is_floating_point_v<T>
    class Normal3;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] Normal3<T> Rotate(const Rotation3<T>&,
                                    const Normal3<T>&) noexcept;

    template <typename T>
        requires std::is_floating_point_v<T>
    class Normal3
    {
    public:
        using Component = T;

        Normal3() noexcept : Value_(T(1), T(0), T(0))
        {
        }

        [[nodiscard]] static std::optional<Normal3> TryFrom(
            const Vec3<T>& value) noexcept
        {
            const auto normalized = detail::TryNormalizeVector(value);
            if (!normalized)
            {
                return std::nullopt;
            }
            return Normal3(*normalized, UncheckedTag{});
        }

        [[nodiscard]] static Normal3 FromOr(const Vec3<T>& value,
                                            Normal3 fallback) noexcept
        {
            const auto result = TryFrom(value);
            return result ? *result : fallback;
        }

        [[nodiscard]] static Normal3 FromDirection(
            const Direction3<T>& direction) noexcept
        {
            return Normal3(direction.Vector(), UncheckedTag{});
        }

        [[nodiscard]] static Normal3 AxisX() noexcept
        {
            return Normal3(Vec3<T>(T(1), T(0), T(0)), UncheckedTag{});
        }

        [[nodiscard]] static Normal3 AxisY() noexcept
        {
            return Normal3(Vec3<T>(T(0), T(1), T(0)), UncheckedTag{});
        }

        [[nodiscard]] static Normal3 AxisZ() noexcept
        {
            return Normal3(Vec3<T>(T(0), T(0), T(1)), UncheckedTag{});
        }

        [[nodiscard]] const Vec3<T>& Vector() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] Direction3<T> ToDirection() const noexcept
        {
            return Direction3<T>(Value_,
                                 typename Direction3<T>::UncheckedTag{});
        }

        [[nodiscard]] Normal3 operator-() const noexcept
        {
            return Normal3(Value_ * T(-1), UncheckedTag{});
        }

        [[nodiscard]] friend bool operator==(const Normal3& left,
                                             const Normal3& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        explicit Normal3(const Vec3<T>& value, UncheckedTag) noexcept :
            Value_(value)
        {
        }

        Vec3<T> Value_;

        friend class Plane3<T>;
        friend Normal3<T> Rotate<T>(const Rotation3<T>&,
                                    const Normal3<T>&) noexcept;
    };

    using Normal3f = Normal3<float>;
    using Normal3d = Normal3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    struct RefractionResult3
    {
        RefractionStatus Status;
        std::optional<Direction3<T>> Direction;

        [[nodiscard]] explicit operator bool() const noexcept
        {
            return Status == RefractionStatus::Success;
        }
    };

    using RefractionResult3f = RefractionResult3<float>;
    using RefractionResult3d = RefractionResult3<double>;

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Normal3<T> Rotate(const Rotation3<T>& rotation,
                                           const Normal3<T>& normal) noexcept
    {
        return Normal3<T>(Rotate(rotation, normal.Vector()),
                          typename Normal3<T>::UncheckedTag{});
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> ProjectOntoPlane(
        const Vec3<T>& value, const Normal3<T>& planeNormal) noexcept
    {
        // Vector rejection from a unit normal follows Ericson, Real-Time
        // Collision Detection (2005), section 5.1.1.
        return value - planeNormal.Vector() * Dot(value, planeNormal.Vector());
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline Vec3<T> Reflect(const Vec3<T>& incident,
                                         const Normal3<T>& normal) noexcept
    {
        // Algebra follows GLSL 4.60 section 8.5's reflect operation; the
        // Normal3 contract supplies GLSL's normalized-normal precondition.
        return incident -
               normal.Vector() * (T(2) * Dot(normal.Vector(), incident));
    }

    template <typename T>
        requires std::is_floating_point_v<T>
    [[nodiscard]] inline RefractionResult3<T> Refract(
        const Direction3<T>& incident,
        const Normal3<T>& surfaceNormal,
        T incidentOverTransmittedIndex) noexcept
    {
        if (!(incidentOverTransmittedIndex > T(0)) ||
            !std::isfinite(incidentOverTransmittedIndex))
        {
            return {RefractionStatus::InvalidIndexRatio, std::nullopt};
        }

        // Algebra and direction convention follow GLSL 4.60 section 8.5's
        // refract(I, N, eta), with eta explicitly named n_incident/n_exit.
        // Unlike GLSL's zero-vector sentinel, Move reports TIR explicitly.
        const T normalDotIncident =
            Dot(surfaceNormal.Vector(), incident.Vector());
        const T discriminant =
            T(1) - incidentOverTransmittedIndex * incidentOverTransmittedIndex *
                       (T(1) - normalDotIncident * normalDotIncident);
        if (discriminant < T(0))
        {
            return {RefractionStatus::TotalInternalReflection, std::nullopt};
        }

        const Vec3<T> refracted =
            incident.Vector() * incidentOverTransmittedIndex -
            surfaceNormal.Vector() *
                (incidentOverTransmittedIndex * normalDotIncident +
                 std::sqrt(discriminant));
        auto direction = Direction3<T>::TryFrom(refracted);
        if (!direction)
        {
            return {RefractionStatus::NumericalFailure, std::nullopt};
        }
        return {RefractionStatus::Success, std::move(direction)};
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Normal3f) == sizeof(mv::math::Vec3f));
static_assert(alignof(mv::math::Normal3f) == alignof(mv::math::Vec3f));
static_assert(std::is_trivially_copyable_v<mv::math::Normal3f>);
static_assert(std::is_standard_layout_v<mv::math::Normal3f>);
static_assert(sizeof(mv::math::Normal3d) == sizeof(mv::math::Vec3d));
static_assert(alignof(mv::math::Normal3d) == alignof(mv::math::Vec3d));
static_assert(std::is_trivially_copyable_v<mv::math::Normal3d>);
static_assert(std::is_standard_layout_v<mv::math::Normal3d>);
