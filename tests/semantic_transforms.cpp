#include <cmath>
#include <cstdlib>
#include <limits>
#include <optional>
#include <type_traits>

#include <mv/math/Transforms.hpp>

namespace
{
    template <typename T>
    concept HasSelfAddition = requires(T left, T right) { left + right; };

    template <typename T>
    concept HasScalarMultiply = requires(T value) { value * 2.0F; };

    void Require(bool condition)
    {
        if (!condition)
        {
            std::abort();
        }
    }

    [[nodiscard]] bool NearlyEqual(float left,
                                   float right,
                                   float epsilon = 1.0e-5F)
    {
        return std::abs(left - right) <= epsilon;
    }

    [[nodiscard]] bool NearlyEqual(const mv::math::Vec3f& left,
                                   const mv::math::Vec3f& right,
                                   float epsilon = 1.0e-5F)
    {
        return NearlyEqual(left.X(), right.X(), epsilon) &&
               NearlyEqual(left.Y(), right.Y(), epsilon) &&
               NearlyEqual(left.Z(), right.Z(), epsilon);
    }

    void CheckPointAlgebra()
    {
        using namespace mv::math;

        Point3f point(1.0F, 2.0F, 3.0F);
        const Vec3f displacement(4.0F, -1.0F, 2.0F);
        const Point3f moved = point + displacement;

        Require(moved == Point3f(5.0F, 1.0F, 5.0F));
        Require(moved - point == displacement);
        Require(moved - displacement == point);

        point += displacement;
        Require(point == moved);
        point -= displacement;
        Require(point == Point3f(1.0F, 2.0F, 3.0F));
        Require(Point3f::FromVector(point.Vector()) == point);
        Require(point.X() == 1.0F);
        Require(point.Y() == 2.0F);
        Require(point.Z() == 3.0F);

        static_assert(!HasSelfAddition<Point3f>);
        static_assert(!HasScalarMultiply<Point3f>);
    }

    void CheckNormalInvariant()
    {
        using namespace mv::math;

        Require(!Normal3f::TryFrom(Vec3f()));
        Require(!Normal3f::TryFrom(
            Vec3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F)));

        const auto normal = Normal3f::TryFrom(Vec3f(0.0F, 3.0F, 4.0F));
        Require(normal.has_value());
        Require(NearlyEqual(normal->Vector(), Vec3f(0.0F, 0.6F, 0.8F)));
        Require(NearlyEqual(LengthSquared(normal->Vector()), 1.0F));

        const Direction3f direction = normal->ToDirection();
        Require(direction.Vector() == normal->Vector());
        Require(Normal3f::FromDirection(direction) == *normal);
    }

    void CheckRigidTransforms()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        const Rotation3f yaw = Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                                         ToRadians(90.0_deg));
        const RigidTransform3f transform(yaw, Vec3f(10.0F, 0.0F, 0.0F));

        const Point3f point(0.0F, 0.0F, 1.0F);
        const Point3f transformedPoint = TransformPoint(transform, point);
        Require(
            NearlyEqual(transformedPoint.Vector(), Vec3f(11.0F, 0.0F, 0.0F)));
        Require(NearlyEqual(TransformVector(transform, point.Vector()),
                            Vec3f(1.0F, 0.0F, 0.0F)));

        const Direction3f direction =
            TransformDirection(transform, Direction3f::Forward());
        const Normal3f normal = TransformNormal(transform, Normal3f::AxisZ());
        Require(NearlyEqual(direction.Vector(), Vec3f(1.0F, 0.0F, 0.0F)));
        Require(NearlyEqual(normal.Vector(), Vec3f(1.0F, 0.0F, 0.0F)));

        const Point3f restored =
            TransformPoint(transform.Inverse(), transformedPoint);
        Require(NearlyEqual(restored.Vector(), point.Vector()));

        const RigidTransform3f first(
            Rotation3f::FromAxisAngle(Direction3f::AxisX(),
                                      ToRadians(30.0_deg)),
            Vec3f(0.0F, 0.0F, 1.0F));
        const RigidTransform3f composed = Compose(first, transform);
        const Point3f compositionInput(0.0F, 2.0F, 1.0F);
        const Point3f sequential =
            TransformPoint(transform, TransformPoint(first, compositionInput));
        Require(NearlyEqual(TransformPoint(composed, compositionInput).Vector(),
                            sequential.Vector()));
    }

    void CheckAffineSemantics()
    {
        using namespace mv::math;

        const AffineTransform3f transform(
            Vec3f(2.0F, 0.0F, 0.0F), Vec3f(0.0F, 3.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 4.0F), Vec3f(10.0F, 20.0F, 30.0F));

        const Point3f point(1.0F, 2.0F, 3.0F);
        Require(TransformPoint(transform, point) ==
                Point3f(12.0F, 26.0F, 42.0F));

        const auto direction =
            TryTransformDirection(transform, Direction3f::AxisX());
        Require(direction.has_value());
        Require(direction->Vector() == Vec3f(1.0F, 0.0F, 0.0F));

        const auto diagonalNormal = Normal3f::TryFrom(Vec3f(1.0F, 1.0F, 0.0F));
        Require(diagonalNormal.has_value());
        const auto transformedNormal =
            TryTransformNormal(transform, *diagonalNormal);
        Require(transformedNormal.has_value());
        Require(NearlyEqual(transformedNormal->Vector(),
                            Vec3f(0.8320503F, 0.5547002F, 0.0F)));

        const AffineTransform3f shear(Vec3f(1.0F, 0.0F, 0.0F),
                                      Vec3f(1.0F, 1.0F, 0.0F),
                                      Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        const auto shearNormal = TryTransformNormal(shear, Normal3f::AxisX());
        Require(shearNormal.has_value());
        const Vec3f transformedTangent =
            TransformVector(shear, Vec3f(0.0F, 1.0F, 0.0F));
        Require(
            NearlyEqual(Dot(shearNormal->Vector(), transformedTangent), 0.0F));

        const AffineTransform3f reflection(Vec3f(-1.0F, 0.0F, 0.0F),
                                           Vec3f(0.0F, 1.0F, 0.0F),
                                           Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        const auto reflected =
            TryTransformNormal(reflection, Normal3f::AxisX());
        Require(reflected.has_value());
        Require(reflected->Vector() == Vec3f(-1.0F, 0.0F, 0.0F));

        const AffineTransform3f singular(Vec3f(1.0F, 0.0F, 0.0F),
                                         Vec3f(0.0F, 0.0F, 0.0F),
                                         Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        Require(!TryTransformNormal(singular, Normal3f::AxisY()));
        Require(!TryTransformDirection(singular, Direction3f::AxisY()));
    }

    void CheckRigidToAffine()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        const RigidTransform3f rigid(
            Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                      ToRadians(90.0_deg)),
            Vec3f(2.0F, 3.0F, 4.0F));
        const AffineTransform3f affine = ToAffine(rigid);
        const Point3f point(0.0F, 0.0F, 1.0F);

        Require(NearlyEqual(TransformPoint(rigid, point).Vector(),
                            TransformPoint(affine, point).Vector()));
    }

    void CheckDoubleTransforms()
    {
        using namespace mv::math;

        const RigidTransform3d rigid(
            Rotation3d::FromAxisAngle(Direction3d::AxisY(),
                                      ToRadians(Degreesd(90.0))),
            Vec3d(2.0, 3.0, 4.0));
        const Point3d point(0.0, 0.0, 1.0);
        Require(IsNearlyEqual(TransformPoint(rigid, point).Vector(),
                              Vec3d(3.0, 3.0, 4.0)));
        Require(IsNearlyEqual(
            TransformPoint(rigid.Inverse(), TransformPoint(rigid, point))
                .Vector(),
            point.Vector()));

        const AffineTransform3d affine(
            Vec3d(2.0, 0.0, 0.0), Vec3d(0.0, 3.0, 0.0), Vec3d(0.0, 0.0, 4.0),
            Vec3d(10.0, 20.0, 30.0));
        Require(TransformPoint(affine, Point3d(1.0, 2.0, 3.0)) ==
                Point3d(12.0, 26.0, 42.0));
        const auto transformedNormal =
            TryTransformNormal(affine, Normal3d::AxisY());
        Require(transformedNormal.has_value());
        Require(
            IsNearlyEqual(transformedNormal->Vector(), Vec3d(0.0, 1.0, 0.0)));
        Require(IsNearlyEqual(TransformPoint(ToAffine(rigid), point).Vector(),
                              TransformPoint(rigid, point).Vector()));
    }
}  // namespace

int main()
{
    CheckPointAlgebra();
    CheckNormalInvariant();
    CheckRigidTransforms();
    CheckAffineSemantics();
    CheckRigidToAffine();
    CheckDoubleTransforms();
    return 0;
}
