@echo off

if not exist build\ninja-release (
    call ninja-release-init.bat
)

pushd build\ninja-release
    cmake --build . --target move-vectormath-test || (
        copy compile_commands.json ..\..\compile_commands.json
        exit /B 1
    )
    copy compile_commands.json ..\..\compile_commands.json
    
    @REM mkdir ..\..\bin
    @REM copy bin\move-tests* ..\..\bin\
    @REM copy bin\*.dll ..\..\bin\
popd