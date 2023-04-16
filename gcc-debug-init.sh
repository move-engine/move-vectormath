#!/bin/bash

mkdir -p build
mkdir -p build/gcc-debug

pushd build/gcc-debug
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DBUILD_TESTING=true -DMOVE_VECTORMATH_DEVELOPER_MODE=true ../../ && cp compile_commands.json ../../
popd