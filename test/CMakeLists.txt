cmake_minimum_required(VERSION 3.14)

project(move-vectormathTests LANGUAGES CXX)

include(../cmake/project-is-top-level.cmake)
include(../cmake/folders.cmake)
include(../cmake/windows-set-path.cmake)

# ---- Dependencies ----
CPMAddPackage("gh:catchorg/Catch2@3.2.0")

# Uses DXM to validate some functions
CPMAddPackage("gh:microsoft/DirectXMath#dec2022")
if(PROJECT_IS_TOP_LEVEL)
  find_package(move-vectormath REQUIRED)
endif()

# ---- Tests ----
# Get all cpp files in the source directory
file(GLOB_RECURSE TEST_SOURCES CONFIGURE_DEPENDS "source/*.cpp")
add_executable(move-vectormath-test ${TEST_SOURCES})

# Uses DXM to validate some functions
target_link_libraries(move-vectormath-test PUBLIC Catch2WithMain move::vectormath DirectXMath)
target_compile_features(move-vectormath-test PRIVATE cxx_std_17)

add_test(NAME move-vectormath-test COMMAND move-vectormath-test)

# ---- End-of-file commands ----

add_folders(Test)
