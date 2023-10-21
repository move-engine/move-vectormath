@echo off
mkdir build\ninja-debug
pushd build\ninja-debug
    cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DBUILD_TESTING=true -DMOVE_VECTORMATH_DEVELOPER_MODE=true -GNinja ../../
    copy compile_commands.json ..\..\compile_commands.json
popd