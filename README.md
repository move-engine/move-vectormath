# Move Vectormath

This is the vectormath library for the Move Engine.  It is a somewhat high-level wrapper for [RTM](https://github.com/nfrechette/rtm/) with a number of extensions, presenting a more familiar interface for game programmers [while maintaining most of RTM's performance](https://github.com/RasterDuck/vectormathbench).

# Status

Move Vectormath is still fairly WIP.  It has fairly robust tests, though it does not currently achieve full coverage.  While it is not nearly as feature complete as more mature libraries such as DirectXMath or GLM, it currently has more functionality than RTM (with a number of functions ported from GLM and DXM).

It is currently tested and benchmarked on Ubuntu and Windows.

# Documentation

Documentation is generated with Doxygen and can be found [here](https://move-engine.github.io/move-vectormath/).

# Building and installing

This is a header-only library, but it depends on RTM.  If your project uses CMake, you can use FetchContent, CPM, or add this repository as a submodule, then simply link against `move-vectormath`, which will automatically set up the RTM dependency using CPM.

If your project does not use CMake, make sure the RTM headers are available in your project's include path.

# Contributing

See the [CONTRIBUTING](CONTRIBUTING.md) document.

# Licensing

<!--
Please go to https://choosealicense.com/licenses/ and choose a license that
fits your needs. The recommended license for a project of this type is the
Boost Software License 1.0.
-->
