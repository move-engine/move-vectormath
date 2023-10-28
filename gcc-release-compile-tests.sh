#!/bin/bash

[[ -d build/gcc-release ]] || ./gcc-release-init.sh
cd build/gcc-release
(cmake --build . --target move-vectormath-test -j24 && cp compile_commands.json ../../) || (cp compile_commands.json ../../; exit 1)