#include <cstdlib>
#include <type_traits>

#include <mv/math/Math.hpp>

namespace
{
    void Require(bool condition)
    {
        if (!condition)
        {
            std::abort();
        }
    }
}  // namespace

int main()
{
    using namespace mv::math;

    static_assert(std::is_same_v<Vec3f::Component, float>);
    static_assert(
        std::is_same_v<decltype(Vec3f::Zero() + Vec3f::One()), Vec3f>);
    static_assert(std::is_same_v<decltype(Cross(Vec3f{}, Vec3f{})), Vec3f>);
    static_assert(
        std::is_same_v<decltype(Mat4f::Identity() * Mat4f::Identity()), Mat4f>);

    Require(Saturate(-2) == 0);
    Require(Saturate(0) == 0);
    Require(Saturate(2) == 1);

    const Vec3f vector(3.0F, 4.0F, 0.0F);
    const auto direction = Direction3f::TryFrom(vector);
    Require(direction.has_value());
    Require(IsNearlyEqual(Length(direction->Vector()), 1.0F));
    Require(!Direction3f::TryFrom(Vec3f::Zero()).has_value());

    const Rotation3f quarterTurn = Rotation3f::FromAxisAngle(
        Direction3f::AxisY(), ToRadians(Degreesf(90.0F)));
    constexpr Tolerance<float> tolerance{1.0e-5F, 1.0e-5F, 1.0e-5F, 1.0e-5F};
    Require(IsNearlyEqual(Rotate(quarterTurn, Vec3f::AxisZ()), Vec3f::AxisX(),
                          tolerance));

    const RigidTransform3f transform(quarterTurn, Vec3f(4.0F, 5.0F, 6.0F));
    const Point3f transformed =
        TransformPoint(transform, Point3f(0.0F, 0.0F, 1.0F));
    Require(IsNearlyEqual(transformed.Vector(), Vec3f(5.0F, 5.0F, 6.0F),
                          tolerance));

    const Mat4f matrix = ToMat4(transform);
    const auto inverse = matrix.TryInverse();
    Require(inverse.has_value());
    Require(
        IsNearlyEqual((matrix * *inverse).Element(0U, 0U), 1.0F, tolerance));

    const auto bounds = Aabb3f::TryFromMinMax(Point3f(-1.0F, -1.0F, -1.0F),
                                              Point3f(1.0F, 1.0F, 1.0F));
    const auto ray = Ray3f::TryFromOriginDirection(Point3f(-3.0F, 0.0F, 0.0F),
                                                   Direction3f::AxisX());
    Require(bounds.has_value() && ray.has_value());
    Require(Intersects(*ray, *bounds));
}
