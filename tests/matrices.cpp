#include <cmath>
#include <limits>
#include <type_traits>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Mat3.hpp>
#include <mv/math/Mat4.hpp>
#include <mv/math/Tolerance.hpp>
#include <mv/math/transforms/Projection.hpp>
#include <mv/math/transforms/View.hpp>

namespace
{
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

    [[nodiscard]] bool NearlyEqual(const mv::math::Vec4f& left,
                                   const mv::math::Vec4f& right,
                                   float epsilon = 1.0e-5F)
    {
        return NearlyEqual(left.X(), right.X(), epsilon) &&
               NearlyEqual(left.Y(), right.Y(), epsilon) &&
               NearlyEqual(left.Z(), right.Z(), epsilon) &&
               NearlyEqual(left.W(), right.W(), epsilon);
    }

    void CheckConstructionAndAccess()
    {
        using namespace mv::math;

        const Mat3f identity;
        REQUIRE(identity == Mat3f::Identity());
        REQUIRE(Vec3f(2.0F, 3.0F, 4.0F) * identity == Vec3f(2.0F, 3.0F, 4.0F));

        const Mat3f matrix(1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F,
                           9.0F);
        REQUIRE(matrix.Row(1U) == Vec3f(4.0F, 5.0F, 6.0F));
        REQUIRE(matrix.Column(2U) == Vec3f(3.0F, 6.0F, 9.0F));
        REQUIRE(NearlyEqual(matrix.Element(2U, 1U), 8.0F));
        REQUIRE(Vec3f(1.0F, 2.0F, 3.0F) * matrix == Vec3f(30.0F, 36.0F, 42.0F));

        REQUIRE(Mat3f::Scale(Vec3f(2.0F, 3.0F, 4.0F)) * Mat3f::Zero() ==
                Mat3f::Zero());
        REQUIRE(Vec3f(1.0F, 2.0F, 3.0F) *
                    Mat3f::Scale(Vec3f(2.0F, 3.0F, 4.0F)) ==
                Vec3f(2.0F, 6.0F, 12.0F));
    }

    void CheckCompositionAndTranspose()
    {
        using namespace mv::math;

        const Mat3f first(1.0F, 2.0F, 0.0F, -1.0F, 1.0F, 3.0F, 2.0F, 0.0F,
                          1.0F);
        const Mat3f second(2.0F, 0.0F, 1.0F, 1.0F, 3.0F, 0.0F, 0.0F, -2.0F,
                           1.0F);
        const Vec3f vector(0.25F, -2.0F, 4.0F);
        REQUIRE(
            NearlyEqual((vector * first) * second, vector * (first * second)));

        const Mat3f transposed = first.Transposed();
        REQUIRE(transposed.Row(0U) == first.Column(0U));
        REQUIRE(transposed.Transposed() == first);
    }

    void CheckRotationConversion()
    {
        using namespace mv::math;

        REQUIRE(Mat3f::FromRotation(Rotation3f::Identity()) ==
                Mat3f::Identity());

        const Rotation3f rotation = Rotation3f::FromAxisAngle(
            *Direction3f::TryFrom(Vec3f(1.0F, 2.0F, -0.5F)), Radiansf(0.73F));
        const Vec3f vector(-2.0F, 0.25F, 4.0F);
        REQUIRE(NearlyEqual(vector * Mat3f::FromRotation(rotation),
                            Rotate(rotation, vector)));
    }

    void CheckInverse()
    {
        using namespace mv::math;

        const Mat3f matrix(1.0F, 2.0F, 3.0F, 0.0F, 1.0F, 4.0F, 5.0F, 6.0F,
                           0.0F);
        REQUIRE(NearlyEqual(matrix.Determinant(), 1.0F));

        const auto inverse = matrix.TryInverse();
        REQUIRE(inverse.has_value());
        REQUIRE(NearlyEqual(inverse->Row(0U), Vec3f(-24.0F, 18.0F, 5.0F)));
        REQUIRE(NearlyEqual(inverse->Row(1U), Vec3f(20.0F, -15.0F, -4.0F)));
        REQUIRE(NearlyEqual(inverse->Row(2U), Vec3f(-5.0F, 4.0F, 1.0F)));
        REQUIRE(NearlyEqual((matrix * *inverse).Row(0U),
                            Mat3f::Identity().Row(0U)));
        REQUIRE(NearlyEqual((matrix * *inverse).Row(1U),
                            Mat3f::Identity().Row(1U)));
        REQUIRE(NearlyEqual((matrix * *inverse).Row(2U),
                            Mat3f::Identity().Row(2U)));

        const Mat3f singular(1.0F, 2.0F, 3.0F, 2.0F, 4.0F, 6.0F, 0.0F, 1.0F,
                             0.0F);
        REQUIRE(!singular.TryInverse());

        const Mat3f nonFinite(std::numeric_limits<float>::infinity(), 0.0F,
                              0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F);
        REQUIRE(!nonFinite.IsFinite());
        REQUIRE(!nonFinite.TryInverse());
    }

    void CheckMat4ConstructionAndComposition()
    {
        using namespace mv::math;

        const Mat4f identity;
        REQUIRE(identity == Mat4f::Identity());
        REQUIRE(Vec4f(1.0F, 2.0F, 3.0F, 4.0F) * identity ==
                Vec4f(1.0F, 2.0F, 3.0F, 4.0F));

        const Mat4f matrix(1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F, 9.0F,
                           10.0F, 11.0F, 12.0F, 13.0F, 14.0F, 15.0F, 16.0F);
        REQUIRE(matrix.Row(2U) == Vec4f(9.0F, 10.0F, 11.0F, 12.0F));
        REQUIRE(matrix.Column(1U) == Vec4f(2.0F, 6.0F, 10.0F, 14.0F));
        REQUIRE(NearlyEqual(matrix.Element(3U, 2U), 15.0F));
        REQUIRE(NearlyEqual(Vec4f(1.0F, 2.0F, 3.0F, 4.0F) * matrix,
                            Vec4f(90.0F, 100.0F, 110.0F, 120.0F)));

        const Mat4f first(1.0F, 2.0F, 0.0F, 1.0F, -1.0F, 1.0F, 3.0F, 0.0F, 2.0F,
                          0.0F, 1.0F, 1.0F, 0.5F, -2.0F, 0.0F, 1.0F);
        const Mat4f second(2.0F, 0.0F, 1.0F, 0.0F, 1.0F, 3.0F, 0.0F, -1.0F,
                           0.0F, -2.0F, 1.0F, 0.0F, 4.0F, 0.0F, 2.0F, 1.0F);
        const Vec4f value(0.25F, -2.0F, 4.0F, 1.0F);
        REQUIRE(
            NearlyEqual((value * first) * second, value * (first * second)));
        REQUIRE(first.Transposed().Row(0U) == first.Column(0U));
        REQUIRE(first.Transposed().Transposed() == first);
    }

    void CheckMat4Inverse()
    {
        using namespace mv::math;

        const Mat4f matrix(2.0F, 0.0F, 0.0F, 0.0F, 0.0F, 3.0F, 0.0F, 0.0F, 0.0F,
                           0.0F, 4.0F, 0.0F, 5.0F, -6.0F, 7.0F, 1.0F);
        REQUIRE(NearlyEqual(matrix.Determinant(), 24.0F));
        const auto inverse = matrix.TryInverse();
        REQUIRE(inverse.has_value());
        REQUIRE(NearlyEqual(inverse->Row(0U), Vec4f(0.5F, 0.0F, 0.0F, 0.0F)));
        REQUIRE(NearlyEqual(inverse->Row(1U),
                            Vec4f(0.0F, 1.0F / 3.0F, 0.0F, 0.0F)));
        REQUIRE(NearlyEqual(inverse->Row(2U), Vec4f(0.0F, 0.0F, 0.25F, 0.0F)));
        REQUIRE(
            NearlyEqual(inverse->Row(3U), Vec4f(-2.5F, 2.0F, -1.75F, 1.0F)));
        for (std::size_t row = 0U; row < 4U; ++row)
        {
            REQUIRE(NearlyEqual((matrix * *inverse).Row(row),
                                Mat4f::Identity().Row(row)));
        }

        const Mat4f singular(1.0F, 2.0F, 3.0F, 4.0F, 2.0F, 4.0F, 6.0F, 8.0F,
                             0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F);
        REQUIRE(!singular.TryInverse());

        const Mat4f nonFinite(std::numeric_limits<float>::infinity(), 0.0F,
                              0.0F, 0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 0.0F,
                              1.0F, 0.0F, 0.0F, 0.0F, 0.0F, 1.0F);
        REQUIRE(!nonFinite.IsFinite());
        REQUIRE(!nonFinite.TryInverse());
    }

    template <typename T>
    [[nodiscard]] T ProjectedDepth(const mv::math::Mat4<T>& matrix,
                                   T cameraSpaceZ)
    {
        const mv::math::Vec4<T> clip =
            mv::math::Vec4<T>(T(0), T(0), cameraSpaceZ, T(1)) * matrix;
        return clip.Z() / clip.W();
    }

    void CheckProjectionConventions()
    {
        using namespace mv::math;

        for (const Handedness handedness :
             {Handedness::Left, Handedness::Right})
        {
            for (const ClipDepth clipDepth :
                 {ClipDepth::ZeroToOne, ClipDepth::NegativeOneToOne})
            {
                for (const DepthDirection depthDirection :
                     {DepthDirection::Forward, DepthDirection::Reverse})
                {
                    const auto projection = TryPerspectiveFov(
                        Radiansf(1.1F), 16.0F / 9.0F, 0.25F, 1000.0F,
                        handedness, clipDepth, depthDirection);
                    REQUIRE(projection.has_value());
                    const float sign =
                        handedness == Handedness::Left ? 1.0F : -1.0F;
                    const float expectedNear =
                        depthDirection == DepthDirection::Reverse ? 1.0F
                        : clipDepth == ClipDepth::ZeroToOne       ? 0.0F
                                                                  : -1.0F;
                    const float expectedFar =
                        depthDirection == DepthDirection::Forward ? 1.0F
                        : clipDepth == ClipDepth::ZeroToOne       ? 0.0F
                                                                  : -1.0F;
                    REQUIRE(
                        NearlyEqual(ProjectedDepth(*projection, sign * 0.25F),
                                    expectedNear, 2.0e-5F));
                    REQUIRE(
                        NearlyEqual(ProjectedDepth(*projection, sign * 1000.0F),
                                    expectedFar, 2.0e-5F));

                    const auto infinite = TryPerspectiveFovInfinite(
                        Radiansf(1.1F), 16.0F / 9.0F, 0.25F, handedness,
                        clipDepth, depthDirection);
                    REQUIRE(infinite.has_value());
                    REQUIRE(NearlyEqual(ProjectedDepth(*infinite, sign * 0.25F),
                                        expectedNear, 2.0e-5F));
                    REQUIRE(
                        NearlyEqual(ProjectedDepth(*infinite, sign * 1.0e7F),
                                    expectedFar, 2.0e-5F));

                    const auto orthographic = TryOrthographicOffCenter(
                        -4.0F, 8.0F, -3.0F, 5.0F, 0.25F, 1000.0F, handedness,
                        clipDepth, depthDirection);
                    REQUIRE(orthographic.has_value());
                    REQUIRE(
                        NearlyEqual(ProjectedDepth(*orthographic, sign * 0.25F),
                                    expectedNear, 2.0e-5F));
                    REQUIRE(NearlyEqual(
                        ProjectedDepth(*orthographic, sign * 1000.0F),
                        expectedFar, 2.0e-5F));
                }
            }
        }

        REQUIRE(!TryPerspectiveFov(Radiansf(0.0F), 1.0F, 0.1F, 100.0F,
                                   Handedness::Left, ClipDepth::ZeroToOne,
                                   DepthDirection::Forward));
        REQUIRE(!TryPerspectiveFov(Radiansf(1.0F), 0.0F, 0.1F, 100.0F,
                                   Handedness::Left, ClipDepth::ZeroToOne,
                                   DepthDirection::Forward));
        REQUIRE(!TryOrthographicOffCenter(
            1.0F, 1.0F, -1.0F, 1.0F, 0.1F, 100.0F, Handedness::Left,
            ClipDepth::ZeroToOne, DepthDirection::Forward));

        const auto doubleProjection = TryPerspectiveFov(
            Radiansd(0.9), 1.5, 0.01, 1.0e8, Handedness::Right,
            ClipDepth::ZeroToOne, DepthDirection::Reverse);
        REQUIRE(doubleProjection.has_value());
        REQUIRE(IsNearlyEqual(ProjectedDepth(*doubleProjection, -0.01), 1.0));
        REQUIRE(IsNearlyEqual(ProjectedDepth(*doubleProjection, -1.0e8), 0.0));
    }

    void CheckViewConventions()
    {
        using namespace mv::math;

        const Point3f eye(3.0F, 4.0F, 5.0F);
        const auto left =
            TryLookAtLH(eye, Point3f(3.0F, 4.0F, 9.0F), Direction3f::AxisY());
        REQUIRE(left.has_value());
        REQUIRE(NearlyEqual(Vec4f(eye.X(), eye.Y(), eye.Z(), 1.0F) * *left,
                            Vec4f(0.0F, 0.0F, 0.0F, 1.0F)));
        REQUIRE(NearlyEqual(Vec4f(3.0F, 4.0F, 9.0F, 1.0F) * *left,
                            Vec4f(0.0F, 0.0F, 4.0F, 1.0F)));

        const auto right =
            TryLookAtRH(eye, Point3f(3.0F, 4.0F, 1.0F), Direction3f::AxisY());
        REQUIRE(right.has_value());
        REQUIRE(NearlyEqual(Vec4f(3.0F, 4.0F, 1.0F, 1.0F) * *right,
                            Vec4f(0.0F, 0.0F, -4.0F, 1.0F)));

        REQUIRE(!TryLookAtLH(eye, eye, Direction3f::AxisY()));
        REQUIRE(
            !TryLookAtLH(eye, Point3f(3.0F, 8.0F, 5.0F), Direction3f::AxisY()));

        const auto doubleView =
            TryLookAtRH(Point3d(1.0, 2.0, 3.0), Point3d(1.0, 2.0, 2.0),
                        Direction3d::AxisY());
        REQUIRE(doubleView.has_value());
    }
}  // namespace

TEST_CASE("mv::math matrix layouts", "[mv][matrix][layout]")
{
    static_assert(sizeof(mv::math::Mat3f) == 48U);
    static_assert(alignof(mv::math::Mat3f) == 16U);
    static_assert(std::is_trivially_copyable_v<mv::math::Mat3f>);
    static_assert(sizeof(mv::math::Mat4f) == 64U);
    static_assert(alignof(mv::math::Mat4f) == 16U);
    static_assert(std::is_trivially_copyable_v<mv::math::Mat4f>);
    static_assert(sizeof(mv::math::Mat4d) == 128U);
    static_assert(alignof(mv::math::Mat4d) == 32U);
    static_assert(std::is_trivially_copyable_v<mv::math::Mat4d>);
}

TEST_CASE("mv::math Mat3 construction and composition", "[mv][matrix][mat3]")
{
    CheckConstructionAndAccess();
    CheckCompositionAndTranspose();
    CheckRotationConversion();
    CheckInverse();
}

TEST_CASE("mv::math Mat4 construction and composition", "[mv][matrix][mat4]")
{
    CheckMat4ConstructionAndComposition();
    CheckMat4Inverse();
}

TEST_CASE("mv::math projection conventions", "[mv][matrix][projection]")
{
    CheckProjectionConventions();
}

TEST_CASE("mv::math view conventions", "[mv][matrix][view]")
{
    CheckViewConventions();
}
