#!/bin/bash

./gcc-release-compile-tests.sh && ./build/gcc-release/test/move-vectormath-test
cp ./build/gcc-release/compile_commands.json ./