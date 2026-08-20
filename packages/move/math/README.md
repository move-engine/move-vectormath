# move/math

This package provides Move's C++20 game and graphics math API in `mv::math`.
Include `<mv/math/Math.hpp>` for the full surface or use the focused headers for
vectors, transforms, geometry, queries, culling, graphics, and GPU layouts.

The CMake consumer target is `mv::math`. The implementation is header-only and
uses RTM 2.3.1 when SIMD is available, with a portable scalar backend selected
by `MV_MATH_FORCE_SCALAR=1`.
