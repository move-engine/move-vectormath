#!/bin/bash

./gcc-debug-compile-tests.sh && ./build/gcc-debug/test/move-vectormath-test
cp ./build/gcc-debug/compile_commands.json ./