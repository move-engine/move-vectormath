# Tests for move/math_examples
This directory contains tests for move/math_examples.  Tests are written using the Catch2 testing framework.

To add tests, simply add a new .cpp file to this directory and it will be automatically included in the build
and write the test cases for your code.  The test executable will be named move-math_examples_tests, and can
be invoked by the CLI using the following:

```
move-cli test move-math_examples
move-cli test move-math-examples
```