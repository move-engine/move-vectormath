#!/bin/bash

mkdir -p build
mkdir -p build/gcc-release

pushd build/gcc-release
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=true ../../ && cp compile_commands.json ../../
popd