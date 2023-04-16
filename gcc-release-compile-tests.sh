#!/bin/bash

[[ -d build/gcc-release ]] || ./gcc-release-init.sh
cd build/gcc-release
(cmake --build . --target move-tests -j24 && cp compile_commands.json ../../) || (cp compile_commands.json ../../; exit 1)