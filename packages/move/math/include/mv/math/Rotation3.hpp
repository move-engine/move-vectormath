#pragma once

#include <algorithm>
#include <cmath>
#include <optional>
#include <type_traits>

#include <mv/math/Angles.hpp>
#include <mv/math/Quat.hpp>
#include <mv/math/semantic/Direction3.hpp>

namespace mv::math
{
    class Rotation3f
    {
    public:
        constexpr Rotation3f() noexcept = default;

        [[nodiscard]] static constexpr Rotation3f Identity() noexcept
        {
            return Rotation3f(Quatf(), UncheckedTag{});
        }

        [[nodiscard]] static std::optional<Rotation3f> TryFrom(
            const Quatf& value) noexcept
        {
            const float maximum =
                std::max({std::abs(value.X()), std::abs(value.Y()),
                          std::abs(value.Z()), std::abs(value.W())});
            if (!(maximum > 0.0F) || !std::isfinite(maximum))
            {
                return std::nullopt;
            }

            const float x = value.X() / maximum;
            const float y = value.Y() / maximum;
            const float z = value.Z() / maximum;
            const float w = value.W() / maximum;
            const float lengthSquared = x * x + y * y + z * z + w * w;
            if (!(lengthSquared > 0.0F) || !std::isfinite(lengthSquared))
            {
                return std::nullopt;
            }

            const float reciprocalLength = 1.0F / std::sqrt(lengthSquared);
            return Rotation3f(Quatf(x * reciprocalLength, y * reciprocalLength,
                                    z * reciprocalLength, w * reciprocalLength),
                              UncheckedTag{});
        }

        [[nodiscard]] static Rotation3f FromAxisAngle(Direction3f axis,
                                                      Radiansf angle) noexcept
        {
            const float halfAngle = angle.Value() * 0.5F;
            const float sine = std::sin(halfAngle);
            const float cosine = std::cos(halfAngle);
            const Vec3f& vector = axis.Vector();
            return Rotation3f(Quatf(vector.X() * sine, vector.Y() * sine,
                                    vector.Z() * sine, cosine),
                              UncheckedTag{});
        }

        [[nodiscard]] constexpr const Quatf& Quaternion() const noexcept
        {
            return Value_;
        }

        [[nodiscard]] constexpr Rotation3f Inverse() const noexcept
        {
            return Rotation3f(
                Quatf(-Value_.X(), -Value_.Y(), -Value_.Z(), Value_.W()),
                UncheckedTag{});
        }

        [[nodiscard]] Rotation3f operator*(
            const Rotation3f& right) const noexcept
        {
            const Quatf& leftValue = Value_;
            const Quatf& rightValue = right.Value_;
            const Quatf composed(leftValue.W() * rightValue.X() +
                                     leftValue.X() * rightValue.W() +
                                     leftValue.Y() * rightValue.Z() -
                                     leftValue.Z() * rightValue.Y(),
                                 leftValue.W() * rightValue.Y() -
                                     leftValue.X() * rightValue.Z() +
                                     leftValue.Y() * rightValue.W() +
                                     leftValue.Z() * rightValue.X(),
                                 leftValue.W() * rightValue.Z() +
                                     leftValue.X() * rightValue.Y() -
                                     leftValue.Y() * rightValue.X() +
                                     leftValue.Z() * rightValue.W(),
                                 leftValue.W() * rightValue.W() -
                                     leftValue.X() * rightValue.X() -
                                     leftValue.Y() * rightValue.Y() -
                                     leftValue.Z() * rightValue.Z());

            return *TryFrom(composed);
        }

        [[nodiscard]] friend constexpr bool operator==(
            const Rotation3f& left, const Rotation3f& right) noexcept
        {
            return left.Value_ == right.Value_;
        }

    private:
        struct UncheckedTag
        {
        };

        constexpr explicit Rotation3f(Quatf value, UncheckedTag) noexcept :
            Value_(value)
        {
        }

        Quatf Value_{};
    };

    [[nodiscard]] inline Vec3f Rotate(const Rotation3f& rotation,
                                      const Vec3f& vector) noexcept
    {
        const Quatf& quaternion = rotation.Quaternion();
        const Vec3f imaginary(quaternion.X(), quaternion.Y(), quaternion.Z());
        const Vec3f twiceCross = Cross(imaginary, vector) * 2.0F;
        return vector + twiceCross * quaternion.W() +
               Cross(imaginary, twiceCross);
    }

    [[nodiscard]] inline Direction3f Rotate(const Rotation3f& rotation,
                                            Direction3f direction) noexcept
    {
        return Direction3f(Rotate(rotation, direction.Vector()),
                           Direction3f::UncheckedTag{});
    }
}  // namespace mv::math

static_assert(sizeof(mv::math::Rotation3f) == sizeof(mv::math::Quatf));
static_assert(alignof(mv::math::Rotation3f) == alignof(mv::math::Quatf));
static_assert(std::is_trivially_copyable_v<mv::math::Rotation3f>);
static_assert(std::is_standard_layout_v<mv::math::Rotation3f>);
