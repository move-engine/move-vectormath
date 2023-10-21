@echo off

ninja-debug-compile-example.bat %1 && (
    @REM pushd bin\Ninja_MSVC\Debug\
    @REM @REM robocopy ..\..\..\samples\sample\built-data\ .\data\assets /e /MT /NDL /NJH /NJS /nc /ns /PURGE
    @REM move64.exe --assetdir "..\..\..\samples\sample\built-data\"
    @REM popd

    @REM pushd samples\sample\
    "build\ninja-debug\examples\example_%1.exe"
)