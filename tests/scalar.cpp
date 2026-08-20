#include <limits>

#include <catch2/catch_test_macros.hpp>
#include <mv/math/Scalar.hpp>

TEST_CASE("mv::math scalar constants", "[mv][scalar][constants]")
{
    using namespace mv::math;

    STATIC_REQUIRE(Pi<float> > 3.14159F);
    STATIC_REQUIRE(HalfPi<double> == Pi<double> / 2.0);
    STATIC_REQUIRE(QuarterPi<float> == Pi<float> / 4.0F);
    STATIC_REQUIRE(Tau<double> == Pi<double> * 2.0);
}

TEST_CASE("mv::math scalar saturation", "[mv][scalar]")
{
    using namespace mv::math;

    STATIC_REQUIRE(Saturate(-2) == 0);
    STATIC_REQUIRE(Saturate(0.25) == 0.25);
    STATIC_REQUIRE(Saturate(4.0F) == 1.0F);
}

TEST_CASE("mv::math inverse lerp is explicit and overflow safe",
          "[mv][scalar][interpolation]")
{
    using namespace mv::math;

    REQUIRE(TryInverseLerp(10.0, 20.0, 12.5) == 0.25);
    REQUIRE(TryInverseLerp(20.0, 10.0, 12.5) == 0.75);
    REQUIRE(TryInverseLerpClamped(10.0, 20.0, 25.0) == 1.0);
    REQUIRE(!TryInverseLerp(1.0, 1.0, 1.0));
    REQUIRE(!TryInverseLerp(0.0, 1.0, std::numeric_limits<double>::infinity()));

    const double maximum = std::numeric_limits<double>::max();
    const auto wide = TryInverseLerp(-maximum, maximum, 0.0);
    REQUIRE(wide.has_value());
    REQUIRE(*wide == 0.5);
}

TEST_CASE("mv::math smoothing polynomials clamp their amount",
          "[mv][scalar][interpolation]")
{
    using namespace mv::math;

    STATIC_REQUIRE(SmoothStep(-1.0F) == 0.0F);
    STATIC_REQUIRE(SmoothStep(0.5F) == 0.5F);
    STATIC_REQUIRE(SmoothStep(2.0F) == 1.0F);
    STATIC_REQUIRE(SmootherStep(-1.0) == 0.0);
    STATIC_REQUIRE(SmootherStep(0.5) == 0.5);
    STATIC_REQUIRE(SmootherStep(2.0) == 1.0);
}
