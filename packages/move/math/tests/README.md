# Tests for move/math
This directory contains tests for move/math.  Tests are written using the Catch2 testing framework.

To add tests, simply add a new .cpp file to this directory and it will be automatically included in the build
and write the test cases for your code.  The test executable will be named move-vectormath_tests, and can
be invoked by the CLI using the following:

```
move-cli test move-vectormath
```