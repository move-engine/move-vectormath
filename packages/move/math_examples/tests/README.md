# Tests for move/math_examples

The maintained example smoke coverage lives in `tests/examples.cpp`. Configure
the root CMake project with `MOVE_VECTORMATH_BUILD_EXAMPLES=ON` to build the ray
tracer and register `move-vectormath-examples` with CTest. Normal hosted CI does
this on every supported compiler and operating system.
