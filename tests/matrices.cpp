#include <cmath>
#include <cstdlib>
#include <limits>
#include <type_traits>

#include <mv/math/Mat3.hpp>

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

    [[nodiscard]] bool NearlyEqual(const mv::math::Vec3f& left,
                                   const mv::math::Vec3f& right,
                                   float epsilon = 1.0e-5F)
    {
        return NearlyEqual(left.X(), right.X(), epsilon) &&
               NearlyEqual(left.Y(), right.Y(), epsilon) &&
               NearlyEqual(left.Z(), right.Z(), epsilon);
    }

    void CheckConstructionAndAccess()
    {
        using namespace mv::math;

        const Mat3f identity;
        Require(identity == Mat3f::Identity());
        Require(Vec3f(2.0F, 3.0F, 4.0F) * identity == Vec3f(2.0F, 3.0F, 4.0F));

        const Mat3f matrix(1.0F, 2.0F, 3.0F, 4.0F, 5.0F, 6.0F, 7.0F, 8.0F,
                           9.0F);
        Require(matrix.Row(1U) == Vec3f(4.0F, 5.0F, 6.0F));
        Require(matrix.Column(2U) == Vec3f(3.0F, 6.0F, 9.0F));
        Require(NearlyEqual(matrix.Element(2U, 1U), 8.0F));
        Require(Vec3f(1.0F, 2.0F, 3.0F) * matrix == Vec3f(30.0F, 36.0F, 42.0F));

        Require(Mat3f::Scale(Vec3f(2.0F, 3.0F, 4.0F)) * Mat3f::Zero() ==
                Mat3f::Zero());
        Require(Vec3f(1.0F, 2.0F, 3.0F) *
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
        Require(
            NearlyEqual((vector * first) * second, vector * (first * second)));

        const Mat3f transposed = first.Transposed();
        Require(transposed.Row(0U) == first.Column(0U));
        Require(transposed.Transposed() == first);
    }

    void CheckRotationConversion()
    {
        using namespace mv::math;

        Require(Mat3f::FromRotation(Rotation3f::Identity()) ==
                Mat3f::Identity());

        const Rotation3f rotation = Rotation3f::FromAxisAngle(
            *Direction3f::TryFrom(Vec3f(1.0F, 2.0F, -0.5F)),
            Radiansf(0.73F));
        const Vec3f vector(-2.0F, 0.25F, 4.0F);
        Require(NearlyEqual(vector * Mat3f::FromRotation(rotation),
                            Rotate(rotation, vector)));
    }

    void CheckInverse()
    {
        using namespace mv::math;

        const Mat3f matrix(1.0F, 2.0F, 3.0F, 0.0F, 1.0F, 4.0F, 5.0F, 6.0F,
                           0.0F);
        Require(NearlyEqual(matrix.Determinant(), 1.0F));

        const auto inverse = matrix.TryInverse();
        Require(inverse.has_value());
        Require(NearlyEqual(inverse->Row(0U), Vec3f(-24.0F, 18.0F, 5.0F)));
        Require(NearlyEqual(inverse->Row(1U), Vec3f(20.0F, -15.0F, -4.0F)));
        Require(NearlyEqual(inverse->Row(2U), Vec3f(-5.0F, 4.0F, 1.0F)));
        Require(NearlyEqual((matrix * *inverse).Row(0U),
                            Mat3f::Identity().Row(0U)));
        Require(NearlyEqual((matrix * *inverse).Row(1U),
                            Mat3f::Identity().Row(1U)));
        Require(NearlyEqual((matrix * *inverse).Row(2U),
                            Mat3f::Identity().Row(2U)));

        const Mat3f singular(1.0F, 2.0F, 3.0F, 2.0F, 4.0F, 6.0F, 0.0F, 1.0F,
                             0.0F);
        Require(!singular.TryInverse());

        const Mat3f nonFinite(std::numeric_limits<float>::infinity(), 0.0F,
                              0.0F, 0.0F, 1.0F, 0.0F, 0.0F, 0.0F, 1.0F);
        Require(!nonFinite.IsFinite());
        Require(!nonFinite.TryInverse());
    }
}  // namespace

int main()
{
    static_assert(sizeof(mv::math::Mat3f) == 48U);
    static_assert(alignof(mv::math::Mat3f) == 16U);
    static_assert(std::is_trivially_copyable_v<mv::math::Mat3f>);

    CheckConstructionAndAccess();
    CheckCompositionAndTranspose();
    CheckRotationConversion();
    CheckInverse();
    return 0;
}
