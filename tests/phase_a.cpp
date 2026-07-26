#include <array>
#include <cmath>
#include <cstddef>
#include <cstdlib>
#include <cstring>
#include <limits>
#include <span>
#include <type_traits>
#include <vector>

#include <mv/math/PhaseA.hpp>
#include <mv/math/experimental/RepresentationProof.hpp>

namespace
{
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

    void CheckVec2()
    {
        using namespace mv::math;

        const Vec2f a(1.0F, 2.0F);
        const Vec2f b(3.0F, 4.0F);
        const Vec2f result = (a + b) * 2.0F;

        Require(result == Vec2f(8.0F, 12.0F));
        Require(NearlyEqual(Dot(a, b), 11.0F));

        const experimental::AlignedVec2f16 alignedA(1.0F, 2.0F);
        const experimental::AlignedVec2f16 alignedB(3.0F, 4.0F);
        const auto alignedResult = (alignedA + alignedB) * 2.0F;
        Require(NearlyEqual(alignedResult.X(), 8.0F));
        Require(NearlyEqual(alignedResult.Y(), 12.0F));
    }

    void CheckVec3()
    {
        using namespace mv::math;

        const Vec3f x(1.0F, 0.0F, 0.0F);
        const Vec3f y(0.0F, 1.0F, 0.0F);
        const Vec3f z = Cross(x, y);

        Require(z == Vec3f(0.0F, 0.0F, 1.0F));
        Require(NearlyEqual(Dot(z, z), 1.0F));

        const Vec3f chained =
            ((Vec3f(1.0F, 2.0F, 3.0F) + Vec3f(4.0F, 5.0F, 6.0F)) * 2.0F) -
            Vec3f(1.0F, 1.0F, 1.0F);
        Require(chained == Vec3f(9.0F, 13.0F, 17.0F));

        Vec3f edited = chained;
        edited.SetX(-1.0F);
        edited.SetY(-2.0F);
        edited.SetZ(-3.0F);
        Require(edited == Vec3f(-1.0F, -2.0F, -3.0F));

        std::array<float, 4> bytes{};
        static_assert(sizeof(bytes) == sizeof(Vec3f));
        std::memcpy(bytes.data(), &chained, sizeof(chained));
        Require(NearlyEqual(bytes[0], 9.0F));
        Require(NearlyEqual(bytes[1], 13.0F));
        Require(NearlyEqual(bytes[2], 17.0F));

        const experimental::NativeVec3f nativeA(1.0F, 2.0F, 3.0F);
        const experimental::NativeVec3f nativeB(4.0F, 5.0F, 6.0F);
        const auto nativeResult = (nativeA + nativeB) * 2.0F;
        Require(NearlyEqual(nativeResult.X(), 10.0F));
        Require(NearlyEqual(nativeResult.Y(), 14.0F));
        Require(NearlyEqual(nativeResult.Z(), 18.0F));
    }

    void CheckStorageAndGpuLayouts()
    {
        using namespace mv::math;

        const PackedVec3f packed{1.0F, 2.0F, 3.0F};
        const Vec3f loaded = Load(packed);
        Require(loaded == Vec3f(1.0F, 2.0F, 3.0F));
        Require(Store(loaded) == packed);

        const gpu::Float3Slot16 slot = gpu::Store(loaded);
        Require(NearlyEqual(slot.X, 1.0F));
        Require(NearlyEqual(slot.Y, 2.0F));
        Require(NearlyEqual(slot.Z, 3.0F));
        Require(slot.Padding == 0U);

        const std::array<Vec3f, 2> compute = {Vec3f(1.0F, 2.0F, 3.0F),
                                              Vec3f(4.0F, 5.0F, 6.0F)};
        const auto gpuBytes = AsGpuBytes<GpuLayout::Float3Array16>(
            std::span<const Vec3f>(compute));
        Require(gpuBytes.size() == 32U);

        const std::array<PackedVec3f, 2> compact = {
            PackedVec3f{1.0F, 2.0F, 3.0F}, PackedVec3f{4.0F, 5.0F, 6.0F}};
        const auto packedBytes = AsGpuBytes<GpuLayout::Float3Packed12>(
            std::span<const PackedVec3f>(compact));
        Require(packedBytes.size() == 24U);
    }

    void CheckSemanticTypes()
    {
        using namespace mv::math;
        using namespace mv::math::literals;

        Require(!Direction3f::TryFrom(Vec3f(0.0F, 0.0F, 0.0F)));
        Require(!Direction3f::TryFrom(
            Vec3f(std::numeric_limits<float>::infinity(), 0.0F, 0.0F)));
        Require(!Direction3f::TryFrom(
            Vec3f(std::numeric_limits<float>::quiet_NaN(), 0.0F, 0.0F)));

        const auto tiny = Direction3f::TryFrom(Vec3f(1.0e-30F, 0.0F, 0.0F));
        Require(tiny.has_value());
        Require(tiny->Vector() == Vec3f(1.0F, 0.0F, 0.0F));

        const auto large = Direction3f::TryFrom(Vec3f(0.0F, 1.0e30F, 0.0F));
        Require(large.has_value());
        Require(large->Vector() == Vec3f(0.0F, 1.0F, 0.0F));

        const Radiansf halfTurn = ToRadians(180.0_deg);
        Require(NearlyEqual(halfTurn.Value(), 3.14159265F));

        const Rotation3f yaw = Rotation3f::FromAxisAngle(Direction3f::AxisY(),
                                                         ToRadians(90.0_deg));
        const Direction3f rotated = Rotate(yaw, Direction3f::Forward());
        Require(NearlyEqual(rotated.Vector().X(), 1.0F));
        Require(NearlyEqual(rotated.Vector().Y(), 0.0F));
        Require(NearlyEqual(rotated.Vector().Z(), 0.0F));
        Require(NearlyEqual(LengthSquared(rotated.Vector()), 1.0F));

        const Rotation3f halfTurnRotation = yaw * yaw;
        const Direction3f turnedAround =
            Rotate(halfTurnRotation, Direction3f::Forward());
        Require(NearlyEqual(turnedAround.Vector().X(), 0.0F));
        Require(NearlyEqual(turnedAround.Vector().Y(), 0.0F));
        Require(NearlyEqual(turnedAround.Vector().Z(), -1.0F));

        Require(!Rotation3f::TryFrom(Quatf(0.0F, 0.0F, 0.0F, 0.0F)));
        Require(!Rotation3f::TryFrom(
            Quatf(0.0F, 0.0F, 0.0F, std::numeric_limits<float>::infinity())));
        const auto normalized =
            Rotation3f::TryFrom(Quatf(0.0F, 0.0F, 0.0F, 2.0F));
        Require(normalized.has_value());
        Require(*normalized == Rotation3f::Identity());
    }

    void CheckTransforms()
    {
        using namespace mv::math;

        const AffineTransform3f transform(
            Vec3f(2.0F, 0.0F, 0.0F), Vec3f(0.0F, 3.0F, 0.0F),
            Vec3f(0.0F, 0.0F, 4.0F), Vec3f(10.0F, 20.0F, 30.0F));

        Require(TransformPoint(transform, Vec3f(1.0F, 2.0F, 3.0F)) ==
                Vec3f(12.0F, 26.0F, 42.0F));

        const std::array<PackedVec3f, 2> input = {
            PackedVec3f{1.0F, 2.0F, 3.0F}, PackedVec3f{-1.0F, 0.0F, 1.0F}};
        std::array<gpu::Float3Slot16, 2> output{};

        TransformPoints(std::span<const PackedVec3f>(input), transform,
                        std::span<gpu::Float3Slot16>(output));

        Require(gpu::Load(output[0]) == Vec3f(12.0F, 26.0F, 42.0F));
        Require(gpu::Load(output[1]) == Vec3f(8.0F, 20.0F, 34.0F));
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

        Require(Load(output[0].Position) == Vec3f(12.0F, 26.0F, 42.0F));
        Require(Load(output[1].Position) == Vec3f(8.0F, 20.0F, 34.0F));
    }
}  // namespace

int main()
{
    CheckVec2();
    CheckVec3();
    CheckStorageAndGpuLayouts();
    CheckSemanticTypes();
    CheckTransforms();
    CheckStridedTransforms();
    return 0;
}
