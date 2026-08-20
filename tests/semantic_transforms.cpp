#include <cmath>
#include <limits>
#include <optional>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Transforms.hpp>

namespace
{
    template <typename T>
    concept HasSelfAddition = requires(T left, T right) { left + right; };

    template <typename T>
    concept HasScalarMultiply = requires(T value) { value * 2.0F; };

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

        REQUIRE(moved == Point3f(5.0F, 1.0F, 5.0F));
        REQUIRE(moved - point == displacement);
        REQUIRE(moved - displacement == point);

        point += displacement;
        REQUIRE(point == moved);
        point -= displacement;
        REQUIRE(point == Point3f(1.0F, 2.0F, 3.0F));
        REQUIRE(Point3f::FromVector(point.Vector()) == point);
        REQUIRE(point.X() == 1.0F);
        REQUIRE(point.Y() == 2.0F);
        REQUIRE(point.Z() == 3.0F);

        static_assert(!HasSelfAddition<Point3f>);
        static_assert(!HasScalarMultiply<Point3f>);
    }

    void CheckDirectionAngles()
    {
        using namespace mv::math;

        REQUIRE(NearlyEqual(
            AngleBetween(Direction3f::AxisX(), Direction3f::AxisY()).Value(),
            HalfPi<float>));
        REQUIRE(NearlyEqual(
            AngleBetween(Direction3f::AxisX(), -Direction3f::AxisX()).Value(),
            Pi<float>));
        REQUIRE(
            NearlyEqual(SignedAngle(Direction3f::AxisX(), Direction3f::AxisY(),
                                    Direction3f::AxisZ())
                            .Value(),
                        HalfPi<float>));
        REQUIRE(
            NearlyEqual(SignedAngle(Direction3f::AxisY(), Direction3f::AxisX(),
                                    Direction3f::AxisZ())
                            .Value(),
                        -HalfPi<float>));

        REQUIRE(
            std::abs(AngleBetween(Direction3d::AxisX(), Direction3d::AxisZ())
                         .Value() -
                     HalfPi<double>) < 1.0e-12);
    }

    void CheckNormalInvariant()
    {
        using namespace mv::math;

        REQUIRE(!Normal3f::TryFrom(Vec3f()));
        REQUIRE(!Normal3f::TryFrom(
            Vec3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F)));

        const auto normal = Normal3f::TryFrom(Vec3f(0.0F, 3.0F, 4.0F));
        REQUIRE(normal.has_value());
        REQUIRE(NearlyEqual(normal->Vector(), Vec3f(0.0F, 0.6F, 0.8F)));
        REQUIRE(NearlyEqual(LengthSquared(normal->Vector()), 1.0F));

        const Direction3f direction = normal->ToDirection();
        REQUIRE(direction.Vector() == normal->Vector());
        REQUIRE(Normal3f::FromDirection(direction) == *normal);
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
        REQUIRE(
            NearlyEqual(transformedPoint.Vector(), Vec3f(11.0F, 0.0F, 0.0F)));
        REQUIRE(NearlyEqual(TransformVector(transform, point.Vector()),
                            Vec3f(1.0F, 0.0F, 0.0F)));

        const Direction3f direction =
            TransformDirection(transform, Direction3f::Forward());
        const Normal3f normal = TransformNormal(transform, Normal3f::AxisZ());
        REQUIRE(NearlyEqual(direction.Vector(), Vec3f(1.0F, 0.0F, 0.0F)));
        REQUIRE(NearlyEqual(normal.Vector(), Vec3f(1.0F, 0.0F, 0.0F)));

        const Point3f restored =
            TransformPoint(transform.Inverse(), transformedPoint);
        REQUIRE(NearlyEqual(restored.Vector(), point.Vector()));

        const RigidTransform3f first(
            Rotation3f::FromAxisAngle(Direction3f::AxisX(),
                                      ToRadians(30.0_deg)),
            Vec3f(0.0F, 0.0F, 1.0F));
        const RigidTransform3f composed = Compose(first, transform);
        const Point3f compositionInput(0.0F, 2.0F, 1.0F);
        const Point3f sequential =
            TransformPoint(transform, TransformPoint(first, compositionInput));
        REQUIRE(NearlyEqual(TransformPoint(composed, compositionInput).Vector(),
                            sequential.Vector()));
    }

    void CheckAffineSemantics()
    {
        using namespace mv::math;

        const AffineTransform3f transform(
            Vec3f(2.0F, 0.0F, 0.0F), Vec3f(0.0F, 3.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 4.0F), Vec3f(10.0F, 20.0F, 30.0F));

        const Point3f point(1.0F, 2.0F, 3.0F);
        REQUIRE(TransformPoint(transform, point) ==
                Point3f(12.0F, 26.0F, 42.0F));

        const auto direction =
            TryTransformDirection(transform, Direction3f::AxisX());
        REQUIRE(direction.has_value());
        REQUIRE(direction->Vector() == Vec3f(1.0F, 0.0F, 0.0F));

        const auto diagonalNormal = Normal3f::TryFrom(Vec3f(1.0F, 1.0F, 0.0F));
        REQUIRE(diagonalNormal.has_value());
        const auto transformedNormal =
            TryTransformNormal(transform, *diagonalNormal);
        REQUIRE(transformedNormal.has_value());
        REQUIRE(NearlyEqual(transformedNormal->Vector(),
                            Vec3f(0.8320503F, 0.5547002F, 0.0F)));

        const AffineTransform3f shear(Vec3f(1.0F, 0.0F, 0.0F),
                                      Vec3f(1.0F, 1.0F, 0.0F),
                                      Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        const auto shearNormal = TryTransformNormal(shear, Normal3f::AxisX());
        REQUIRE(shearNormal.has_value());
        const Vec3f transformedTangent =
            TransformVector(shear, Vec3f(0.0F, 1.0F, 0.0F));
        REQUIRE(
            NearlyEqual(Dot(shearNormal->Vector(), transformedTangent), 0.0F));

        const AffineTransform3f reflection(Vec3f(-1.0F, 0.0F, 0.0F),
                                           Vec3f(0.0F, 1.0F, 0.0F),
                                           Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        const auto reflected =
            TryTransformNormal(reflection, Normal3f::AxisX());
        REQUIRE(reflected.has_value());
        REQUIRE(reflected->Vector() == Vec3f(-1.0F, 0.0F, 0.0F));
        const auto orientedSurface =
            TryTransformOrientedSurfaceNormal(reflection, Normal3f::AxisX());
        REQUIRE(orientedSurface.has_value());
        REQUIRE(orientedSurface->Vector() == Vec3f(1.0F, 0.0F, 0.0F));

        const AffineTransform3f singular(Vec3f(1.0F, 0.0F, 0.0F),
                                         Vec3f(0.0F, 0.0F, 0.0F),
                                         Vec3f(0.0F, 0.0F, 1.0F), Vec3f());
        REQUIRE(!TryTransformNormal(singular, Normal3f::AxisY()));
        REQUIRE(!TryTransformDirection(singular, Direction3f::AxisY()));
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

        REQUIRE(NearlyEqual(TransformPoint(rigid, point).Vector(),
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
        REQUIRE(IsNearlyEqual(TransformPoint(rigid, point).Vector(),
                              Vec3d(3.0, 3.0, 4.0)));
        REQUIRE(IsNearlyEqual(
            TransformPoint(rigid.Inverse(), TransformPoint(rigid, point))
                .Vector(),
            point.Vector()));

        const AffineTransform3d affine(
            Vec3d(2.0, 0.0, 0.0), Vec3d(0.0, 3.0, 0.0), Vec3d(0.0, 0.0, 4.0),
            Vec3d(10.0, 20.0, 30.0));
        REQUIRE(TransformPoint(affine, Point3d(1.0, 2.0, 3.0)) ==
                Point3d(12.0, 26.0, 42.0));
        const auto transformedNormal =
            TryTransformNormal(affine, Normal3d::AxisY());
        REQUIRE(transformedNormal.has_value());
        REQUIRE(
            IsNearlyEqual(transformedNormal->Vector(), Vec3d(0.0, 1.0, 0.0)));
        REQUIRE(IsNearlyEqual(TransformPoint(ToAffine(rigid), point).Vector(),
                              TransformPoint(rigid, point).Vector()));
    }

    void CheckRotationCompletion()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        const Quatf arbitrary(2.0F, -3.0F, 4.0F, 5.0F);
        const auto normalized = TryNormalize(arbitrary);
        const auto inverse = TryInverse(arbitrary);
        REQUIRE(normalized.has_value());
        REQUIRE(inverse.has_value());
        REQUIRE(NearlyEqual(Length(*normalized), 1.0F));
        REQUIRE(IsNearlyEqual(
            arbitrary * *inverse, Quatf(),
            Tolerance<float>{1.0e-5F, 1.0e-5F, 1.0e-5F, 1.0e-5F}));
        REQUIRE(!TryNormalize(Quatf(0.0F, 0.0F, 0.0F, 0.0F)));

        const double large = std::numeric_limits<double>::max() * 0.25;
        const Quatd largeQuaternion(large, -large, large, large);
        const auto largeInverse = TryInverse(largeQuaternion);
        REQUIRE(largeInverse.has_value());
        REQUIRE(IsNearlyEqual(
            largeQuaternion * *largeInverse, Quatd(),
            Tolerance<double>{1.0e-12, 1.0e-12, 1.0e-12, 1.0e-12}));

        const double tiny = std::numeric_limits<double>::min();
        const Quatd tinyQuaternion(tiny, 0.0, 0.0, 0.0);
        const auto tinyInverse = TryInverse(tinyQuaternion);
        REQUIRE(tinyInverse.has_value());
        REQUIRE(IsNearlyEqual(
            tinyQuaternion * *tinyInverse, Quatd(),
            Tolerance<double>{1.0e-12, 1.0e-12, 1.0e-12, 1.0e-12}));

        const EulerAngles3f angles{ToRadians(20.0_deg),
                                   ToRadians(Degreesf(-35.0F)),
                                   ToRadians(15.0_deg), EulerOrder::ZXY};
        const Rotation3f euler = Rotation3f::FromEuler(angles);
        const Vec3f input(0.25F, -1.0F, 2.0F);
        const Rotation3f z =
            Rotation3f::FromAxisAngle(Direction3f::AxisZ(), angles.Z);
        const Rotation3f x =
            Rotation3f::FromAxisAngle(Direction3f::AxisX(), angles.X);
        const Rotation3f y =
            Rotation3f::FromAxisAngle(Direction3f::AxisY(), angles.Y);
        REQUIRE(NearlyEqual(Rotate(euler, input),
                            Rotate(y, Rotate(x, Rotate(z, input)))));

        const auto look = Rotation3f::TryLookTowards(Direction3f::AxisX(),
                                                     Direction3f::AxisY());
        REQUIRE(look.has_value());
        REQUIRE(NearlyEqual(Rotate(*look, Direction3f::AxisZ()).Vector(),
                            Vec3f(1.0F, 0.0F, 0.0F)));
        REQUIRE(!Rotation3f::TryLookTowards(Direction3f::AxisY(),
                                            Direction3f::AxisY()));

        const Rotation3f half =
            Slerp(Rotation3f::Identity(),
                  Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                            ToRadians(90.0_deg)),
                  0.5F);
        REQUIRE(NearlyEqual(Rotate(half, Direction3f::AxisZ()).Vector(),
                            Vec3f(0.7071068F, 0.0F, 0.7071068F)));
        REQUIRE(IsNearlyEquivalent(
            Nlerp(Rotation3f::Identity(), Rotation3f::Identity(), 0.25F),
            Rotation3f::Identity()));
    }

    void CheckAffineAndTrsCompletion()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        const TrsTransform3f authored(
            Vec3f(3.0F, -2.0F, 5.0F),
            Rotation3f::FromEuler(
                EulerAngles3f{ToRadians(15.0_deg), ToRadians(25.0_deg),
                              ToRadians(Degreesf(-10.0F)), EulerOrder::XYZ}),
            Vec3f(-2.0F, 3.0F, 4.0F));
        const AffineTransform3f affine = authored.ToAffine();
        const auto decomposition = DecomposeTrs(affine);
        REQUIRE(decomposition.Status == TrsDecompositionStatus::Success);
        REQUIRE(decomposition.Transform.has_value());
        REQUIRE(NearlyEqual(
            TransformPoint(decomposition.Transform->ToAffine(),
                           Point3f(0.5F, -1.0F, 2.0F))
                .Vector(),
            TransformPoint(affine, Point3f(0.5F, -1.0F, 2.0F)).Vector(),
            5.0e-5F));

        const auto affineInverse = affine.TryInverse();
        REQUIRE(affineInverse.has_value());
        const Point3f point(2.0F, 1.0F, -3.0F);
        REQUIRE(NearlyEqual(
            TransformPoint(*affineInverse, TransformPoint(affine, point))
                .Vector(),
            point.Vector(), 5.0e-5F));

        const AffineTransform3f second(
            Vec3f(1.0F, 0.0F, 0.0F), Vec3f(0.0F, 2.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 1.0F), Vec3f(-1.0F, 2.0F, 0.5F));
        const AffineTransform3f composed = Compose(affine, second);
        REQUIRE(NearlyEqual(
            TransformPoint(composed, point).Vector(),
            TransformPoint(second, TransformPoint(affine, point)).Vector(),
            5.0e-5F));

        const Mat4f matrix = ToMat4(affine);
        const auto restoredAffine = TryToAffine(matrix);
        REQUIRE(restoredAffine.has_value());
        REQUIRE(NearlyEqual(TransformPoint(*restoredAffine, point).Vector(),
                            TransformPoint(affine, point).Vector()));
        REQUIRE(!TryToRigid(affine));

        const RigidTransform3f rigid(
            Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                      ToRadians(30.0_deg)),
            Vec3f(1.0F, 2.0F, 3.0F));
        const auto restoredRigid = TryToRigid(ToMat4(rigid));
        REQUIRE(restoredRigid.has_value());
        REQUIRE(NearlyEqual(TransformPoint(*restoredRigid, point).Vector(),
                            TransformPoint(rigid, point).Vector()));

        const AffineTransform3f shear(Vec3f(1.0F, 0.0F, 0.0F),
                                      Vec3f(0.5F, 1.0F, 0.0F),
                                      Vec3f(0.0F, 0.0F, 1.0F), Vec3f::Zero());
        REQUIRE(DecomposeTrs(shear).Status == TrsDecompositionStatus::Sheared);
    }
}  // namespace

TEST_CASE("mv::math point algebra", "[mv][semantic][point]")
{
    CheckPointAlgebra();
}

TEST_CASE("mv::math direction angles", "[mv][semantic][direction][angle]")
{
    CheckDirectionAngles();
}

TEST_CASE("mv::math normal invariants", "[mv][semantic][normal]")
{
    CheckNormalInvariant();
}

TEST_CASE("mv::math rigid transforms", "[mv][transform][rigid]")
{
    CheckRigidTransforms();
}

TEST_CASE("mv::math affine semantics", "[mv][transform][affine]")
{
    CheckAffineSemantics();
    CheckRigidToAffine();
}

TEST_CASE("mv::math double-precision transforms", "[mv][transform][double]")
{
    CheckDoubleTransforms();
}

TEST_CASE("mv::math rotation completion", "[mv][rotation]")
{
    CheckRotationCompletion();
}

TEST_CASE("mv::math affine and TRS completion", "[mv][transform][trs]")
{
    CheckAffineAndTrsCompletion();
}
