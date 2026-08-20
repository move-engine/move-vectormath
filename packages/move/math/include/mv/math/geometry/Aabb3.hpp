#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <cstdint>
#include <limits>
#include <optional>
#include <span>
#include <type_traits>

#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Move-owned AABB contract; operations cross-checked against DirectXMath
    // DirectXCollision.inl (MIT).
    template <typename T>
        requires std::is_arithmetic_v<T>
    class Aabb3
    {
    public:
        using Component = T;

        Aabb3() noexcept :
            Minimum_(EmptyMinimum(), EmptyMinimum(), EmptyMinimum()),
            Maximum_(EmptyMaximum(), EmptyMaximum(), EmptyMaximum())
        {
        }

        [[nodiscard]] static Aabb3 Empty() noexcept
        {
            return Aabb3();
        }

        [[nodiscard]] static std::optional<Aabb3> TryFromMinMax(
            const Point3<T>& minimum, const Point3<T>& maximum) noexcept
        {
            if (!IsFinitePoint(minimum) || !IsFinitePoint(maximum) ||
                minimum.X() > maximum.X() || minimum.Y() > maximum.Y() ||
                minimum.Z() > maximum.Z())
            {
                return std::nullopt;
            }
            return Aabb3(minimum, maximum, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Aabb3> TryFromCenterHalfExtents(
            const Point3<T>& center, const Vec3<T>& halfExtents) noexcept
            requires std::is_floating_point_v<T>
        {
            if (!IsFinitePoint(center) || !IsFiniteVector(halfExtents) ||
                halfExtents.X() < T(0) || halfExtents.Y() < T(0) ||
                halfExtents.Z() < T(0))
            {
                return std::nullopt;
            }
            return TryFromMinMax(center - halfExtents, center + halfExtents);
        }

        [[nodiscard]] static std::optional<Aabb3> TryFromPoints(
            std::span<const Point3<T>> points) noexcept
        {
            Aabb3 result;
            for (const Point3<T>& point : points)
            {
                const auto expanded = result.TryExpanded(point);
                if (!expanded)
                {
                    return std::nullopt;
                }
                result = *expanded;
            }
            return result;
        }

        [[nodiscard]] const Point3<T>& Minimum() const noexcept
        {
            return Minimum_;
        }

        [[nodiscard]] const Point3<T>& Maximum() const noexcept
        {
            return Maximum_;
        }

        [[nodiscard]] bool IsEmpty() const noexcept
        {
            return Minimum_.X() > Maximum_.X();
        }

        [[nodiscard]] bool IsValid() const noexcept
        {
            return IsEmpty() ||
                   (IsFinitePoint(Minimum_) && IsFinitePoint(Maximum_) &&
                    Minimum_.X() <= Maximum_.X() &&
                    Minimum_.Y() <= Maximum_.Y() &&
                    Minimum_.Z() <= Maximum_.Z());
        }

        [[nodiscard]] std::optional<Point3<T>> TryCenter() const noexcept
            requires std::is_floating_point_v<T>
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            using Calculation =
                std::conditional_t<(sizeof(T) < sizeof(double)), double, T>;
            return Point3<T>(
                static_cast<T>((static_cast<Calculation>(Minimum_.X()) +
                                static_cast<Calculation>(Maximum_.X())) *
                               Calculation(0.5)),
                static_cast<T>((static_cast<Calculation>(Minimum_.Y()) +
                                static_cast<Calculation>(Maximum_.Y())) *
                               Calculation(0.5)),
                static_cast<T>((static_cast<Calculation>(Minimum_.Z()) +
                                static_cast<Calculation>(Maximum_.Z())) *
                               Calculation(0.5)));
        }

        [[nodiscard]] std::optional<Vec3<T>> TrySize() const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            const long double x = static_cast<long double>(Maximum_.X()) -
                                  static_cast<long double>(Minimum_.X());
            const long double y = static_cast<long double>(Maximum_.Y()) -
                                  static_cast<long double>(Minimum_.Y());
            const long double z = static_cast<long double>(Maximum_.Z()) -
                                  static_cast<long double>(Minimum_.Z());
            const long double maximum =
                static_cast<long double>(std::numeric_limits<T>::max());
            if (x > maximum || y > maximum || z > maximum)
            {
                return std::nullopt;
            }
            return Vec3<T>(static_cast<T>(x), static_cast<T>(y),
                           static_cast<T>(z));
        }

        [[nodiscard]] std::optional<Vec3<T>> TryHalfExtents() const noexcept
            requires std::is_floating_point_v<T>
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            using Calculation =
                std::conditional_t<(sizeof(T) < sizeof(double)), double, T>;
            return Vec3<T>(
                static_cast<T>((static_cast<Calculation>(Maximum_.X()) -
                                static_cast<Calculation>(Minimum_.X())) *
                               Calculation(0.5)),
                static_cast<T>((static_cast<Calculation>(Maximum_.Y()) -
                                static_cast<Calculation>(Minimum_.Y())) *
                               Calculation(0.5)),
                static_cast<T>((static_cast<Calculation>(Maximum_.Z()) -
                                static_cast<Calculation>(Minimum_.Z())) *
                               Calculation(0.5)));
        }

        [[nodiscard]] bool Contains(const Point3<T>& point) const noexcept
        {
            return !IsEmpty() && point.X() >= Minimum_.X() &&
                   point.Y() >= Minimum_.Y() && point.Z() >= Minimum_.Z() &&
                   point.X() <= Maximum_.X() && point.Y() <= Maximum_.Y() &&
                   point.Z() <= Maximum_.Z();
        }

        [[nodiscard]] bool Contains(const Aabb3& other) const noexcept
        {
            return other.IsEmpty() || (!IsEmpty() && Contains(other.Minimum_) &&
                                       Contains(other.Maximum_));
        }

        [[nodiscard]] bool Intersects(const Aabb3& other) const noexcept
        {
            return !IsEmpty() && !other.IsEmpty() &&
                   Maximum_.X() >= other.Minimum_.X() &&
                   Minimum_.X() <= other.Maximum_.X() &&
                   Maximum_.Y() >= other.Minimum_.Y() &&
                   Minimum_.Y() <= other.Maximum_.Y() &&
                   Maximum_.Z() >= other.Minimum_.Z() &&
                   Minimum_.Z() <= other.Maximum_.Z();
        }

        [[nodiscard]] Aabb3 Merged(const Aabb3& other) const noexcept
        {
            if (IsEmpty())
            {
                return other;
            }
            if (other.IsEmpty())
            {
                return *this;
            }
            return Aabb3(Point3<T>(std::min(Minimum_.X(), other.Minimum_.X()),
                                   std::min(Minimum_.Y(), other.Minimum_.Y()),
                                   std::min(Minimum_.Z(), other.Minimum_.Z())),
                         Point3<T>(std::max(Maximum_.X(), other.Maximum_.X()),
                                   std::max(Maximum_.Y(), other.Maximum_.Y()),
                                   std::max(Maximum_.Z(), other.Maximum_.Z())),
                         UncheckedTag{});
        }

        [[nodiscard]] Aabb3 Intersection(const Aabb3& other) const noexcept
        {
            if (!Intersects(other))
            {
                return Empty();
            }
            return Aabb3(Point3<T>(std::max(Minimum_.X(), other.Minimum_.X()),
                                   std::max(Minimum_.Y(), other.Minimum_.Y()),
                                   std::max(Minimum_.Z(), other.Minimum_.Z())),
                         Point3<T>(std::min(Maximum_.X(), other.Maximum_.X()),
                                   std::min(Maximum_.Y(), other.Maximum_.Y()),
                                   std::min(Maximum_.Z(), other.Maximum_.Z())),
                         UncheckedTag{});
        }

        [[nodiscard]] std::optional<Aabb3> TryExpanded(
            const Point3<T>& point) const noexcept
        {
            if (!IsFinitePoint(point))
            {
                return std::nullopt;
            }
            if (IsEmpty())
            {
                return Aabb3(point, point, UncheckedTag{});
            }
            return Aabb3(Point3<T>(std::min(Minimum_.X(), point.X()),
                                   std::min(Minimum_.Y(), point.Y()),
                                   std::min(Minimum_.Z(), point.Z())),
                         Point3<T>(std::max(Maximum_.X(), point.X()),
                                   std::max(Maximum_.Y(), point.Y()),
                                   std::max(Maximum_.Z(), point.Z())),
                         UncheckedTag{});
        }

        [[nodiscard]] std::optional<Point3<T>> TryClosestPoint(
            const Point3<T>& point) const noexcept
        {
            if (IsEmpty() || !IsFinitePoint(point))
            {
                return std::nullopt;
            }
            return Point3<T>(std::clamp(point.X(), Minimum_.X(), Maximum_.X()),
                             std::clamp(point.Y(), Minimum_.Y(), Maximum_.Y()),
                             std::clamp(point.Z(), Minimum_.Z(), Maximum_.Z()));
        }

        [[nodiscard]] T Volume() const noexcept
            requires std::is_floating_point_v<T>
        {
            if (IsEmpty())
            {
                return T(0);
            }
            const double x = static_cast<double>(Maximum_.X()) -
                             static_cast<double>(Minimum_.X());
            const double y = static_cast<double>(Maximum_.Y()) -
                             static_cast<double>(Minimum_.Y());
            const double z = static_cast<double>(Maximum_.Z()) -
                             static_cast<double>(Minimum_.Z());
            return static_cast<T>(x * y * z);
        }

        [[nodiscard]] T SurfaceArea() const noexcept
            requires std::is_floating_point_v<T>
        {
            if (IsEmpty())
            {
                return T(0);
            }
            const double x = static_cast<double>(Maximum_.X()) -
                             static_cast<double>(Minimum_.X());
            const double y = static_cast<double>(Maximum_.Y()) -
                             static_cast<double>(Minimum_.Y());
            const double z = static_cast<double>(Maximum_.Z()) -
                             static_cast<double>(Minimum_.Z());
            return static_cast<T>(2.0 * (x * y + y * z + z * x));
        }

        [[nodiscard]] std::optional<std::array<Point3<T>, 8>> TryCorners()
            const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            return std::array<Point3<T>, 8>{
                Point3<T>(Minimum_.X(), Minimum_.Y(), Minimum_.Z()),
                Point3<T>(Maximum_.X(), Minimum_.Y(), Minimum_.Z()),
                Point3<T>(Minimum_.X(), Maximum_.Y(), Minimum_.Z()),
                Point3<T>(Maximum_.X(), Maximum_.Y(), Minimum_.Z()),
                Point3<T>(Minimum_.X(), Minimum_.Y(), Maximum_.Z()),
                Point3<T>(Maximum_.X(), Minimum_.Y(), Maximum_.Z()),
                Point3<T>(Minimum_.X(), Maximum_.Y(), Maximum_.Z()),
                Point3<T>(Maximum_.X(), Maximum_.Y(), Maximum_.Z())};
        }

        [[nodiscard]] friend bool operator==(const Aabb3& left,
                                             const Aabb3& right) noexcept
        {
            return left.Minimum_ == right.Minimum_ &&
                   left.Maximum_ == right.Maximum_;
        }

    private:
        struct UncheckedTag
        {
        };

        Aabb3(const Point3<T>& minimum,
              const Point3<T>& maximum,
              UncheckedTag) noexcept :
            Minimum_(minimum), Maximum_(maximum)
        {
        }

        [[nodiscard]] static constexpr T EmptyMinimum() noexcept
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                return std::numeric_limits<T>::infinity();
            }
            else
            {
                return std::numeric_limits<T>::max();
            }
        }

        [[nodiscard]] static constexpr T EmptyMaximum() noexcept
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                return -std::numeric_limits<T>::infinity();
            }
            else
            {
                return std::numeric_limits<T>::lowest();
            }
        }

        [[nodiscard]] static bool IsFinitePoint(const Point3<T>& point) noexcept
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                       std::isfinite(point.Z());
            }
            return true;
        }

        [[nodiscard]] static bool IsFiniteVector(const Vec3<T>& vector) noexcept
        {
            if constexpr (std::is_floating_point_v<T>)
            {
                return std::isfinite(vector.X()) && std::isfinite(vector.Y()) &&
                       std::isfinite(vector.Z());
            }
            return true;
        }

        Point3<T> Minimum_;
        Point3<T> Maximum_;
    };

    using Aabb3f = Aabb3<float>;
    using Aabb3d = Aabb3<double>;
    using Aabb3i8 = Aabb3<std::int8_t>;
    using Aabb3u8 = Aabb3<std::uint8_t>;
    using Aabb3i16 = Aabb3<std::int16_t>;
    using Aabb3u16 = Aabb3<std::uint16_t>;
    using Aabb3i32 = Aabb3<std::int32_t>;
    using Aabb3u32 = Aabb3<std::uint32_t>;
    using Aabb3i64 = Aabb3<std::int64_t>;
    using Aabb3u64 = Aabb3<std::uint64_t>;
    using Aabb3i = Aabb3i32;
    using Aabb3u = Aabb3u32;
}  // namespace mv::math

static_assert(sizeof(mv::math::Aabb3f) == 32);
static_assert(alignof(mv::math::Aabb3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Aabb3f>);
static_assert(std::is_standard_layout_v<mv::math::Aabb3f>);
static_assert(sizeof(mv::math::Aabb3d) == 64);
static_assert(alignof(mv::math::Aabb3d) == 32);
static_assert(std::is_trivially_copyable_v<mv::math::Aabb3d>);
static_assert(std::is_standard_layout_v<mv::math::Aabb3d>);
