#include <array>
#include <cmath>
#include <cstddef>
#include <cstring>
#include <limits>
#include <span>
#include <type_traits>
#include <vector>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Math.hpp>
#include "support/RepresentationFixtures.hpp"

namespace
{
    [[nodiscard]] bool NearlyEqual(float left,
                                   float right,
                                   float epsilon = 1.0e-5F)
    {
        return std::abs(left - right) <= epsilon;
    }

    [[nodiscard]] bool NearlyEqual(double left,
                                   double right,
                                   double epsilon = 1.0e-12)
    {
        return std::abs(left - right) <= epsilon;
    }

    void CheckVec2()
    {
        using namespace mv::math;

        const Vec2f a(1.0F, 2.0F);
        const Vec2f b(3.0F, 4.0F);
        const Vec2f result = (a + b) * 2.0F;

        REQUIRE(result == Vec2f(8.0F, 12.0F));
        REQUIRE(NearlyEqual(Dot(a, b), 11.0F));

        const test::AlignedVec2f16 alignedA(1.0F, 2.0F);
        const test::AlignedVec2f16 alignedB(3.0F, 4.0F);
        const auto alignedResult = (alignedA + alignedB) * 2.0F;
        REQUIRE(NearlyEqual(alignedResult.X(), 8.0F));
        REQUIRE(NearlyEqual(alignedResult.Y(), 12.0F));

        Vec2i integer(12, 18);
        integer /= Vec2i(3, 2);
        integer += 1;
        integer *= 2;
        REQUIRE(integer == Vec2i(10, 20));
        REQUIRE(LengthSquared(Vec2i(3, 4)) == 25);
        REQUIRE(DistanceSquared(Vec2i(1, 2), Vec2i(4, 6)) == 25);
        REQUIRE(NearlyEqual(Length(Vec2f(3.0F, 4.0F)), 5.0F));
        REQUIRE(
            NearlyEqual(Distance(Vec2f(1.0F, 2.0F), Vec2f(4.0F, 6.0F)), 5.0F));
        REQUIRE(Min(Vec2i(1, 5), Vec2i(3, 2)) == Vec2i(1, 2));
        REQUIRE(Max(Vec2i(1, 5), Vec2i(3, 2)) == Vec2i(3, 5));
        REQUIRE(Clamp(Vec2i(-2, 8), Vec2i(0, 1), Vec2i(4, 6)) == Vec2i(0, 6));
        REQUIRE(Abs(Vec2i(-2, 8)) == Vec2i(2, 8));
        REQUIRE(Sign(Vec2i(-2, 0)) == Vec2i(-1, 0));
        REQUIRE(Vec2i16(Vec2i8(1, 2)) == Vec2i16(1, 2));
        REQUIRE(Vec2i::AxisX() + Vec2i::AxisY() == Vec2i::One());
        REQUIRE(integer[0] == 10);
        REQUIRE(integer.Get<1>() == 20);
        REQUIRE(integer.SetComponent(0U, 7));
        REQUIRE(!integer.SetComponent(2U, 9));
        integer.Set<1>(11);
        REQUIRE(integer == Vec2i(7, 11));
        REQUIRE(Lerp(Vec2f(0.0F, 2.0F), Vec2f(10.0F, 6.0F), 1.5F) ==
                Vec2f(10.0F, 6.0F));
        REQUIRE(LerpUnclamped(Vec2f(0.0F, 2.0F), Vec2f(10.0F, 6.0F), 1.5F) ==
                Vec2f(15.0F, 8.0F));
        static_assert(std::is_same_v<Vec2i, Vec2i32>);
        static_assert(std::is_same_v<Vec2u, Vec2u32>);
    }

    void CheckVec3()
    {
        using namespace mv::math;

        const Vec3f x(1.0F, 0.0F, 0.0F);
        const Vec3f y(0.0F, 1.0F, 0.0F);
        const Vec3f z = Cross(x, y);

        REQUIRE(z == Vec3f(0.0F, 0.0F, 1.0F));
        REQUIRE(NearlyEqual(Dot(z, z), 1.0F));

        const Vec3f chained =
            ((Vec3f(1.0F, 2.0F, 3.0F) + Vec3f(4.0F, 5.0F, 6.0F)) * 2.0F) -
            Vec3f(1.0F, 1.0F, 1.0F);
        REQUIRE(chained == Vec3f(9.0F, 13.0F, 17.0F));

        Vec3f edited = chained;
        edited.SetX(-1.0F);
        edited.SetY(-2.0F);
        edited.SetZ(-3.0F);
        REQUIRE(edited == Vec3f(-1.0F, -2.0F, -3.0F));

        Vec3i integer(12, 18, 24);
        integer /= Vec3i(3, 2, 6);
        integer += 1;
        integer *= 2;
        REQUIRE(integer == Vec3i(10, 20, 10));
        REQUIRE(-Vec3i(1, -2, 3) == Vec3i(-1, 2, -3));
        REQUIRE(3 * Vec3i(1, 2, 3) == Vec3i(3, 6, 9));
        REQUIRE(DistanceSquared(Vec3i(1, 2, 3), Vec3i(4, 6, 3)) == 25);
        REQUIRE(NearlyEqual(Length(Vec3f(2.0F, 3.0F, 6.0F)), 7.0F));
        REQUIRE(Min(Vec3i(1, 5, 3), Vec3i(3, 2, 4)) == Vec3i(1, 2, 3));
        REQUIRE(Max(Vec3i(1, 5, 3), Vec3i(3, 2, 4)) == Vec3i(3, 5, 4));
        const float quietNaN = std::numeric_limits<float>::quiet_NaN();
        const Vec3f exceptionalLeft(quietNaN, -2.0F, 3.0F);
        const Vec3f exceptionalRight(1.0F, 4.0F, -1.0F);
        const Vec3f exceptionalMinimum = Min(exceptionalLeft, exceptionalRight);
        const Vec3f exceptionalMaximum = Max(exceptionalLeft, exceptionalRight);
        REQUIRE(std::isnan(exceptionalMinimum.X()));
        REQUIRE(std::isnan(exceptionalMaximum.X()));
        REQUIRE(exceptionalMinimum.Y() == -2.0F);
        REQUIRE(exceptionalMinimum.Z() == -1.0F);
        REQUIRE(exceptionalMaximum.Y() == 4.0F);
        REQUIRE(exceptionalMaximum.Z() == 3.0F);
        REQUIRE(
            Min(Vec3f(1.0F, 2.0F, 3.0F), Vec3f(quietNaN, quietNaN, quietNaN)) ==
            Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(
            Max(Vec3f(1.0F, 2.0F, 3.0F), Vec3f(quietNaN, quietNaN, quietNaN)) ==
            Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(Clamp(Vec3i(-2, 8, 3), Vec3i(0, 1, 4), Vec3i(4, 6, 5)) ==
                Vec3i(0, 6, 4));
        REQUIRE(Abs(Vec3i(-2, 8, -3)) == Vec3i(2, 8, 3));
        REQUIRE(Sign(Vec3i(-2, 0, 3)) == Vec3i(-1, 0, 1));
        REQUIRE(Vec3i(Vec2i(2, 3), 4) == Vec3i(2, 3, 4));
        REQUIRE(Vec3i(2, 3, 4).XY() == Vec2i(2, 3));
        REQUIRE(Vec3i::AxisX() + Vec3i::AxisY() + Vec3i::AxisZ() ==
                Vec3i::One());
        REQUIRE(integer[2] == 10);
        REQUIRE(integer.Get<0>() == 10);
        REQUIRE(integer.SetComponent(2U, 12));
        REQUIRE(!integer.SetComponent(3U, 9));
        integer.Set<0>(8);
        REQUIRE(integer == Vec3i(8, 20, 12));
        REQUIRE(Lerp(Vec3f(0.0F, 2.0F, 4.0F), Vec3f(10.0F, 6.0F, 8.0F), 0.5F) ==
                Vec3f(5.0F, 4.0F, 6.0F));
        static_assert(std::is_same_v<Vec3i, Vec3i32>);
        static_assert(std::is_same_v<Vec3u, Vec3u32>);

        std::array<float, 4> bytes{};
        static_assert(sizeof(bytes) == sizeof(Vec3f));
        std::memcpy(bytes.data(), &chained, sizeof(chained));
        REQUIRE(NearlyEqual(bytes[0], 9.0F));
        REQUIRE(NearlyEqual(bytes[1], 13.0F));
        REQUIRE(NearlyEqual(bytes[2], 17.0F));

        const test::NativeVec3f nativeA(1.0F, 2.0F, 3.0F);
        const test::NativeVec3f nativeB(4.0F, 5.0F, 6.0F);
        const auto nativeResult = (nativeA + nativeB) * 2.0F;
        REQUIRE(NearlyEqual(nativeResult.X(), 10.0F));
        REQUIRE(NearlyEqual(nativeResult.Y(), 14.0F));
        REQUIRE(NearlyEqual(nativeResult.Z(), 18.0F));
    }

    void CheckVec4()
    {
        using namespace mv::math;

        Vec4f value(1.0F, 2.0F, 3.0F, 4.0F);
        value *= Vec4f(2.0F, 3.0F, 4.0F, 5.0F);
        value -= 1.0F;
        value /= Vec4f(1.0F, 5.0F, 11.0F, 19.0F);
        REQUIRE(value == Vec4f(1.0F, 1.0F, 1.0F, 1.0F));
        REQUIRE(NearlyEqual(
            Dot(Vec4f(1.0F, 2.0F, 3.0F, 4.0F), Vec4f(4.0F, 3.0F, 2.0F, 1.0F)),
            20.0F));
        REQUIRE(LengthSquared(Vec4i(1, 2, 3, 4)) == 30);
        REQUIRE(DistanceSquared(Vec4i(1, 2, 3, 4), Vec4i(4, 6, 3, 4)) == 25);
        REQUIRE(NearlyEqual(Length(Vec4f(1.0F, 2.0F, 2.0F, 4.0F)), 5.0F));
        REQUIRE(Min(Vec4i(1, 5, 3, 7), Vec4i(3, 2, 4, 6)) == Vec4i(1, 2, 3, 6));
        REQUIRE(Max(Vec4i(1, 5, 3, 7), Vec4i(3, 2, 4, 6)) == Vec4i(3, 5, 4, 7));
        REQUIRE(Clamp(Vec4i(-2, 8, 3, 9), Vec4i(0, 1, 4, 2),
                      Vec4i(4, 6, 5, 8)) == Vec4i(0, 6, 4, 8));
        REQUIRE(Abs(Vec4i(-2, 8, -3, -9)) == Vec4i(2, 8, 3, 9));
        REQUIRE(Sign(Vec4i(-2, 0, 3, -9)) == Vec4i(-1, 0, 1, -1));
        REQUIRE(Vec4i(Vec3i(2, 3, 4), 5) == Vec4i(2, 3, 4, 5));
        REQUIRE(Vec4i(2, 3, 4, 5).XYZ() == Vec3i(2, 3, 4));
        REQUIRE(Vec4i(2, 3, 4, 5).ZW() == Vec2i(4, 5));
        REQUIRE(Vec4i::AxisX() + Vec4i::AxisY() + Vec4i::AxisZ() +
                    Vec4i::AxisW() ==
                Vec4i::One());
        REQUIRE(value[3] == 1.0F);
        REQUIRE(value.Get<2>() == 1.0F);
        REQUIRE(value.SetComponent(3U, 2.0F));
        REQUIRE(!value.SetComponent(4U, 9.0F));
        value.Set<2>(3.0F);
        REQUIRE(value == Vec4f(1.0F, 1.0F, 3.0F, 2.0F));
        REQUIRE(LerpUnclamped(Vec4f(0.0F, 2.0F, 4.0F, 6.0F),
                              Vec4f(10.0F, 6.0F, 8.0F, 10.0F),
                              0.5F) == Vec4f(5.0F, 4.0F, 6.0F, 8.0F));
        static_assert(std::is_same_v<Vec4i, Vec4i32>);
        static_assert(std::is_same_v<Vec4u, Vec4u32>);
    }

    void CheckStorageAndGpuLayouts()
    {
        using namespace mv::math;

        const PackedVec3f packed{1.0F, 2.0F, 3.0F};
        const Vec3f loaded = Load(packed);
        REQUIRE(loaded == Vec3f(1.0F, 2.0F, 3.0F));
        REQUIRE(Store(loaded) == packed);

        const gpu::Float3Slot16 slot = gpu::Store(loaded);
        REQUIRE(NearlyEqual(slot.X, 1.0F));
        REQUIRE(NearlyEqual(slot.Y, 2.0F));
        REQUIRE(NearlyEqual(slot.Z, 3.0F));
        REQUIRE(slot.Padding == 0U);

        const std::array<Vec3f, 2> compute = {Vec3f(1.0F, 2.0F, 3.0F),
                                              Vec3f(4.0F, 5.0F, 6.0F)};
        const auto gpuBytes = AsGpuBytes<GpuLayout::Float3Array16>(
            std::span<const Vec3f>(compute));
        REQUIRE(gpuBytes.size() == 32U);

        const std::array<PackedVec3f, 2> compact = {
            PackedVec3f{1.0F, 2.0F, 3.0F}, PackedVec3f{4.0F, 5.0F, 6.0F}};
        const auto packedBytes = AsGpuBytes<GpuLayout::Float3Packed12>(
            std::span<const PackedVec3f>(compact));
        REQUIRE(packedBytes.size() == 24U);
    }

    void CheckSemanticTypes()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        const Point3i gridPoint(4, 5, 6);
        REQUIRE(gridPoint + Vec3i(1, -2, 3) == Point3i(5, 3, 9));
        REQUIRE(Point3i(5, 8, 13) - gridPoint == Vec3i(1, 3, 7));
        static_assert(std::is_same_v<Point3i, Point3i32>);
        static_assert(std::is_same_v<Point3u, Point3u32>);

        const Tolerance<float> loose{1.0e-4F, 1.0e-4F, 1.0e-4F, 1.0e-4F};
        REQUIRE(IsNearlyEqual(1.0F, 1.00005F, loose));
        REQUIRE(IsNearlyEqual(Vec3f(1000.0F, 2.0F, 3.0F),
                              Vec3f(1000.05F, 2.0F, 3.0F), loose));
        REQUIRE(IsNearlyZero(Vec3f(1.0e-5F, -1.0e-5F, 0.0F), 2.0e-5F));
        REQUIRE(IsNearlyEqual(std::numeric_limits<float>::infinity(),
                              std::numeric_limits<float>::infinity()));
        REQUIRE(!IsNearlyEqual(std::numeric_limits<float>::quiet_NaN(),
                               std::numeric_limits<float>::quiet_NaN()));

        REQUIRE(!Direction3f::TryFrom(Vec3f(0.0F, 0.0F, 0.0F)));
        REQUIRE(!Direction3f::TryFrom(
            Vec3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F)));
        REQUIRE(!Direction3f::TryFrom(
            Vec3f(std::numeric_limits<float>::quiet_NaN(), 0.0F, 0.0F)));

        const auto tiny = Direction3f::TryFrom(Vec3f(1.0e-30F, 0.0F, 0.0F));
        REQUIRE(tiny.has_value());
        REQUIRE(tiny->Vector() == Vec3f(1.0F, 0.0F, 0.0F));

        const auto large = Direction3f::TryFrom(Vec3f(0.0F, 1.0e30F, 0.0F));
        REQUIRE(large.has_value());
        REQUIRE(large->Vector() == Vec3f(0.0F, 1.0F, 0.0F));

        const Radiansf halfTurn = ToRadians(180.0_deg);
        REQUIRE(NearlyEqual(halfTurn.Value(), 3.14159265F));

        const Rotation3f yaw = Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                                         ToRadians(90.0_deg));
        const Direction3f rotated = Rotate(yaw, Direction3f::Forward());
        REQUIRE(NearlyEqual(rotated.Vector().X(), 1.0F));
        REQUIRE(NearlyEqual(rotated.Vector().Y(), 0.0F));
        REQUIRE(NearlyEqual(rotated.Vector().Z(), 0.0F));
        REQUIRE(NearlyEqual(LengthSquared(rotated.Vector()), 1.0F));

        const Rotation3f halfTurnRotation = yaw * yaw;
        const Direction3f turnedAround =
            Rotate(halfTurnRotation, Direction3f::Forward());
        REQUIRE(NearlyEqual(turnedAround.Vector().X(), 0.0F));
        REQUIRE(NearlyEqual(turnedAround.Vector().Y(), 0.0F));
        REQUIRE(NearlyEqual(turnedAround.Vector().Z(), -1.0F));

        REQUIRE(!Rotation3f::TryFrom(Quatf(0.0F, 0.0F, 0.0F, 0.0F)));
        REQUIRE(!Rotation3f::TryFrom(
            Quatf(0.0F, 0.0F, 0.0F, std::numeric_limits<float>::infinity())));
        const auto normalized =
            Rotation3f::TryFrom(Quatf(0.0F, 0.0F, 0.0F, 2.0F));
        REQUIRE(normalized.has_value());
        REQUIRE(*normalized == Rotation3f::Identity());

        const auto preciseDirection =
            Direction3d::TryFrom(Vec3d(0.0, 0.0, 4.0));
        REQUIRE(preciseDirection.has_value());
        REQUIRE(preciseDirection->Vector() == Vec3d(0.0, 0.0, 1.0));
        const Normal3d preciseNormal =
            Normal3d::FromDirection(*preciseDirection);
        REQUIRE(preciseNormal.Vector() == Vec3d(0.0, 0.0, 1.0));
        REQUIRE(
            ProjectOntoDirection(Vec3d(2.0, 3.0, 4.0), Direction3d::AxisY()) ==
            Vec3d(0.0, 3.0, 0.0));
        REQUIRE(ProjectOntoPlane(Vec3d(2.0, 3.0, 4.0), Normal3d::AxisY()) ==
                Vec3d(2.0, 0.0, 4.0));
        REQUIRE(Reflect(Vec3d(1.0, -2.0, 3.0), Normal3d::AxisY()) ==
                Vec3d(1.0, 2.0, 3.0));
        const auto normalRefraction =
            Refract(-Direction3d::AxisY(), Normal3d::AxisY(), 1.0 / 1.5);
        REQUIRE(normalRefraction.Status == RefractionStatus::Success);
        REQUIRE(normalRefraction.Direction.has_value());
        REQUIRE(IsNearlyEqual(normalRefraction.Direction->Vector(),
                              Vec3d(0.0, -1.0, 0.0)));
        const auto tirIncident =
            Direction3d::TryFrom(Vec3d(0.9, 0.4358898943540673, 0.0));
        REQUIRE(tirIncident.has_value());
        const auto totalInternalReflection =
            Refract(*tirIncident, Normal3d::AxisY(), 1.5);
        REQUIRE(totalInternalReflection.Status ==
                RefractionStatus::TotalInternalReflection);
        REQUIRE(!totalInternalReflection.Direction);
        REQUIRE(Refract(-Direction3d::AxisY(), Normal3d::AxisY(), 0.0).Status ==
                RefractionStatus::InvalidIndexRatio);
        const Rotation3d preciseYaw = Rotation3d::FromAxisAngle(
            Direction3d::AxisY(), ToRadians(Degreesd(90.0)));
        const Direction3d preciseRotated =
            Rotate(preciseYaw, Direction3d::Forward());
        REQUIRE(NearlyEqual(preciseRotated.Vector().X(), 1.0));
        REQUIRE(NearlyEqual(preciseRotated.Vector().Y(), 0.0));
        REQUIRE(NearlyEqual(preciseRotated.Vector().Z(), 0.0));
    }

    void CheckTransforms()
    {
        using namespace mv::math;

        const AffineTransform3f transform(
            Vec3f(2.0F, 0.0F, 0.0F), Vec3f(0.0F, 3.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 4.0F), Vec3f(10.0F, 20.0F, 30.0F));

        REQUIRE(TransformPoint(transform, Vec3f(1.0F, 2.0F, 3.0F)) ==
                Vec3f(12.0F, 26.0F, 42.0F));

        const std::array<PackedVec3f, 2> input = {
            PackedVec3f{1.0F, 2.0F, 3.0F}, PackedVec3f{-1.0F, 0.0F, 1.0F}};
        std::array<gpu::Float3Slot16, 2> output{};

        TransformPoints(std::span<const PackedVec3f>(input), transform,
                        std::span<gpu::Float3Slot16>(output));

        REQUIRE(gpu::Load(output[0]) == Vec3f(12.0F, 26.0F, 42.0F));
        REQUIRE(gpu::Load(output[1]) == Vec3f(8.0F, 20.0F, 34.0F));
    }

    struct Particle
    {
        float Age{};
        mv::math::PackedVec3f Position{};
        float Lifetime{};
    };

    struct OutputParticle
    {
        std::uint32_t Id{};
        mv::math::PackedVec3f Position{};
        float SortKey{};
    };

    void CheckStridedTransforms()
    {
        using namespace mv::math;

        std::vector<Particle> particles = {{1.0F, {1.0F, 2.0F, 3.0F}, 4.0F},
                                           {2.0F, {-1.0F, 0.0F, 1.0F}, 8.0F}};
        std::vector<OutputParticle> output(2);

        const auto inputView = MakeFieldView(
            std::span<const Particle>(particles), &Particle::Position);
        auto outputView = MakeFieldView(std::span<OutputParticle>(output),
                                        &OutputParticle::Position);

        const AffineTransform3f transform(
            Vec3f(2.0F, 0.0F, 0.0F), Vec3f(0.0F, 3.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 4.0F), Vec3f(10.0F, 20.0F, 30.0F));

        TransformPoints(inputView, transform, outputView);

        REQUIRE(Load(output[0].Position) == Vec3f(12.0F, 26.0F, 42.0F));
        REQUIRE(Load(output[1].Position) == Vec3f(8.0F, 20.0F, 34.0F));
    }
}  // namespace

TEST_CASE("mv::math vector representations", "[mv][representation]")
{
    CheckVec2();
    CheckVec3();
    CheckVec4();
}

TEST_CASE("mv::math storage and GPU layouts", "[mv][representation][gpu]")
{
    CheckStorageAndGpuLayouts();
}

TEST_CASE("mv::math semantic value representations",
          "[mv][representation][semantic]")
{
    CheckSemanticTypes();
}

TEST_CASE("mv::math transform representations",
          "[mv][representation][transform]")
{
    CheckTransforms();
}

TEST_CASE("mv::math strided transforms", "[mv][representation][batch]")
{
    CheckStridedTransforms();
}
