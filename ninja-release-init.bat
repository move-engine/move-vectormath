@echo off
mkdir build\ninja-release
pushd build\ninja-release
    cmake -DCMAKE_BUILD_TYPE=Release -DCMAKE_EXPORT_COMPILE_COMMANDS=true -DBUILD_TESTING=true -DMOVE_VECTORMATH_DEVELOPER_MODE=true -GNinja ../../
    copy compile_commands.json ..\..\compile_commands.json
popd