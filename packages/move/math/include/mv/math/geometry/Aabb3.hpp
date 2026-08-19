#pragma once

#include <algorithm>
#include <array>
#include <cmath>
#include <limits>
#include <optional>
#include <span>
#include <type_traits>

#include <mv/math/semantic/Point3.hpp>

namespace mv::math
{
    // Provenance: MVM-PROV-C-PRIMITIVES.
    class Aabb3f
    {
    public:
        Aabb3f() noexcept :
            Minimum_(Infinity(), Infinity(), Infinity()),
            Maximum_(-Infinity(), -Infinity(), -Infinity())
        {
        }

        [[nodiscard]] static Aabb3f Empty() noexcept
        {
            return Aabb3f();
        }

        [[nodiscard]] static std::optional<Aabb3f> TryFromMinMax(
            const Point3f& minimum, const Point3f& maximum) noexcept
        {
            if (!IsFinitePoint(minimum) || !IsFinitePoint(maximum) ||
                minimum.X() > maximum.X() || minimum.Y() > maximum.Y() ||
                minimum.Z() > maximum.Z())
            {
                return std::nullopt;
            }
            return Aabb3f(minimum, maximum, UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Aabb3f> TryFromCenterHalfExtents(
            const Point3f& center, const Vec3f& halfExtents) noexcept
        {
            if (!IsFinitePoint(center) || !IsFiniteVector(halfExtents) ||
                halfExtents.X() < 0.0F || halfExtents.Y() < 0.0F ||
                halfExtents.Z() < 0.0F)
            {
                return std::nullopt;
            }
            return TryFromMinMax(center - halfExtents, center + halfExtents);
        }

        [[nodiscard]] static std::optional<Aabb3f> TryFromPoints(
            std::span<const Point3f> points) noexcept
        {
            Aabb3f result;
            for (const Point3f& point : points)
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

        [[nodiscard]] const Point3f& Minimum() const noexcept
        {
            return Minimum_;
        }

        [[nodiscard]] const Point3f& Maximum() const noexcept
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

        [[nodiscard]] std::optional<Point3f> TryCenter() const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            return Point3f(
                static_cast<float>((static_cast<double>(Minimum_.X()) +
                                    static_cast<double>(Maximum_.X())) *
                                   0.5),
                static_cast<float>((static_cast<double>(Minimum_.Y()) +
                                    static_cast<double>(Maximum_.Y())) *
                                   0.5),
                static_cast<float>((static_cast<double>(Minimum_.Z()) +
                                    static_cast<double>(Maximum_.Z())) *
                                   0.5));
        }

        [[nodiscard]] std::optional<Vec3f> TrySize() const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            const double x = static_cast<double>(Maximum_.X()) -
                             static_cast<double>(Minimum_.X());
            const double y = static_cast<double>(Maximum_.Y()) -
                             static_cast<double>(Minimum_.Y());
            const double z = static_cast<double>(Maximum_.Z()) -
                             static_cast<double>(Minimum_.Z());
            if (x > static_cast<double>(std::numeric_limits<float>::max()) ||
                y > static_cast<double>(std::numeric_limits<float>::max()) ||
                z > static_cast<double>(std::numeric_limits<float>::max()))
            {
                return std::nullopt;
            }
            return Vec3f(static_cast<float>(x), static_cast<float>(y),
                         static_cast<float>(z));
        }

        [[nodiscard]] std::optional<Vec3f> TryHalfExtents() const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            return Vec3f(
                static_cast<float>((static_cast<double>(Maximum_.X()) -
                                    static_cast<double>(Minimum_.X())) *
                                   0.5),
                static_cast<float>((static_cast<double>(Maximum_.Y()) -
                                    static_cast<double>(Minimum_.Y())) *
                                   0.5),
                static_cast<float>((static_cast<double>(Maximum_.Z()) -
                                    static_cast<double>(Minimum_.Z())) *
                                   0.5));
        }

        [[nodiscard]] bool Contains(const Point3f& point) const noexcept
        {
            return !IsEmpty() && point.X() >= Minimum_.X() &&
                   point.Y() >= Minimum_.Y() && point.Z() >= Minimum_.Z() &&
                   point.X() <= Maximum_.X() && point.Y() <= Maximum_.Y() &&
                   point.Z() <= Maximum_.Z();
        }

        [[nodiscard]] bool Contains(const Aabb3f& other) const noexcept
        {
            return other.IsEmpty() || (!IsEmpty() && Contains(other.Minimum_) &&
                                       Contains(other.Maximum_));
        }

        [[nodiscard]] bool Intersects(const Aabb3f& other) const noexcept
        {
            return !IsEmpty() && !other.IsEmpty() &&
                   Maximum_.X() >= other.Minimum_.X() &&
                   Minimum_.X() <= other.Maximum_.X() &&
                   Maximum_.Y() >= other.Minimum_.Y() &&
                   Minimum_.Y() <= other.Maximum_.Y() &&
                   Maximum_.Z() >= other.Minimum_.Z() &&
                   Minimum_.Z() <= other.Maximum_.Z();
        }

        [[nodiscard]] Aabb3f Merged(const Aabb3f& other) const noexcept
        {
            if (IsEmpty())
            {
                return other;
            }
            if (other.IsEmpty())
            {
                return *this;
            }
            return Aabb3f(Point3f(std::min(Minimum_.X(), other.Minimum_.X()),
                                  std::min(Minimum_.Y(), other.Minimum_.Y()),
                                  std::min(Minimum_.Z(), other.Minimum_.Z())),
                          Point3f(std::max(Maximum_.X(), other.Maximum_.X()),
                                  std::max(Maximum_.Y(), other.Maximum_.Y()),
                                  std::max(Maximum_.Z(), other.Maximum_.Z())),
                          UncheckedTag{});
        }

        [[nodiscard]] Aabb3f Intersection(const Aabb3f& other) const noexcept
        {
            if (!Intersects(other))
            {
                return Empty();
            }
            return Aabb3f(Point3f(std::max(Minimum_.X(), other.Minimum_.X()),
                                  std::max(Minimum_.Y(), other.Minimum_.Y()),
                                  std::max(Minimum_.Z(), other.Minimum_.Z())),
                          Point3f(std::min(Maximum_.X(), other.Maximum_.X()),
                                  std::min(Maximum_.Y(), other.Maximum_.Y()),
                                  std::min(Maximum_.Z(), other.Maximum_.Z())),
                          UncheckedTag{});
        }

        [[nodiscard]] std::optional<Aabb3f> TryExpanded(
            const Point3f& point) const noexcept
        {
            if (!IsFinitePoint(point))
            {
                return std::nullopt;
            }
            if (IsEmpty())
            {
                return Aabb3f(point, point, UncheckedTag{});
            }
            return Aabb3f(Point3f(std::min(Minimum_.X(), point.X()),
                                  std::min(Minimum_.Y(), point.Y()),
                                  std::min(Minimum_.Z(), point.Z())),
                          Point3f(std::max(Maximum_.X(), point.X()),
                                  std::max(Maximum_.Y(), point.Y()),
                                  std::max(Maximum_.Z(), point.Z())),
                          UncheckedTag{});
        }

        [[nodiscard]] std::optional<Point3f> TryClosestPoint(
            const Point3f& point) const noexcept
        {
            if (IsEmpty() || !IsFinitePoint(point))
            {
                return std::nullopt;
            }
            return Point3f(std::clamp(point.X(), Minimum_.X(), Maximum_.X()),
                           std::clamp(point.Y(), Minimum_.Y(), Maximum_.Y()),
                           std::clamp(point.Z(), Minimum_.Z(), Maximum_.Z()));
        }

        [[nodiscard]] float Volume() const noexcept
        {
            if (IsEmpty())
            {
                return 0.0F;
            }
            const double x = static_cast<double>(Maximum_.X()) -
                             static_cast<double>(Minimum_.X());
            const double y = static_cast<double>(Maximum_.Y()) -
                             static_cast<double>(Minimum_.Y());
            const double z = static_cast<double>(Maximum_.Z()) -
                             static_cast<double>(Minimum_.Z());
            return static_cast<float>(x * y * z);
        }

        [[nodiscard]] float SurfaceArea() const noexcept
        {
            if (IsEmpty())
            {
                return 0.0F;
            }
            const double x = static_cast<double>(Maximum_.X()) -
                             static_cast<double>(Minimum_.X());
            const double y = static_cast<double>(Maximum_.Y()) -
                             static_cast<double>(Minimum_.Y());
            const double z = static_cast<double>(Maximum_.Z()) -
                             static_cast<double>(Minimum_.Z());
            return static_cast<float>(2.0 * (x * y + y * z + z * x));
        }

        [[nodiscard]] std::optional<std::array<Point3f, 8>> TryCorners()
            const noexcept
        {
            if (IsEmpty())
            {
                return std::nullopt;
            }
            return std::array<Point3f, 8>{
                Point3f(Minimum_.X(), Minimum_.Y(), Minimum_.Z()),
                Point3f(Maximum_.X(), Minimum_.Y(), Minimum_.Z()),
                Point3f(Minimum_.X(), Maximum_.Y(), Minimum_.Z()),
                Point3f(Maximum_.X(), Maximum_.Y(), Minimum_.Z()),
                Point3f(Minimum_.X(), Minimum_.Y(), Maximum_.Z()),
                Point3f(Maximum_.X(), Minimum_.Y(), Maximum_.Z()),
                Point3f(Minimum_.X(), Maximum_.Y(), Maximum_.Z()),
                Point3f(Maximum_.X(), Maximum_.Y(), Maximum_.Z())};
        }

        [[nodiscard]] friend bool operator==(const Aabb3f& left,
                                             const Aabb3f& right) noexcept
        {
            return left.Minimum_ == right.Minimum_ &&
                   left.Maximum_ == right.Maximum_;
        }

    private:
        struct UncheckedTag
        {
        };

        Aabb3f(Point3f minimum, Point3f maximum, UncheckedTag) noexcept :
            Minimum_(minimum), Maximum_(maximum)
        {
        }

        [[nodiscard]] static constexpr float Infinity() noexcept
        {
            return std::numeric_limits<float>::infinity();
        }

        [[nodiscard]] static bool IsFinitePoint(const Point3f& point) noexcept
        {
            return std::isfinite(point.X()) && std::isfinite(point.Y()) &&
                   std::isfinite(point.Z());
        }

        [[nodiscard]] static bool IsFiniteVector(const Vec3f& vector) noexcept
        {
            return std::isfinite(vector.X()) && std::isfinite(vector.Y()) &&
                   std::isfinite(vector.Z());
        }

        Point3f Minimum_;
        Point3f Maximum_;
    };
}  // namespace mv::math

static_assert(sizeof(mv::math::Aabb3f) == 32);
static_assert(alignof(mv::math::Aabb3f) == 16);
static_assert(std::is_trivially_copyable_v<mv::math::Aabb3f>);
static_assert(std::is_standard_layout_v<mv::math::Aabb3f>);
