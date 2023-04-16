#include "move-vectormath/move-vectormath.hpp"

#include <catch2/catch_all.hpp>
#include <move/vectormath.hpp>

#include "catch2/catch_test_macros.hpp"

SCENARIO("Test move-vectormath", "[move-vectormath]")
{
  GIVEN("A transformation matrix")
  {
    move::matrix4 transformation = move::matrix4::Identity;
  }
}