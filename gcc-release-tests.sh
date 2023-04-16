#!/bin/bash

./gcc-release-compile-tests.sh && ./build/gcc-release/tests/move-tests
cp ./build/gcc-release/compile_commands.json ./