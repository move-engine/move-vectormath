cmake_minimum_required(VERSION 3.14)

project(move-vectormath-consumer LANGUAGES CXX)

if(NOT MOVE_VECTORMATH_SOURCE_DIR)
    message(FATAL_ERROR "MOVE_VECTORMATH_SOURCE_DIR must name the source tree")
endif()

include(FetchContent)
FetchContent_Declare(
    move_vectormath
    SOURCE_DIR "${MOVE_VECTORMATH_SOURCE_DIR}"
)
FetchContent_MakeAvailable(move_vectormath)

enable_testing()
add_executable(move-vectormath-consumer main.cpp)
target_link_libraries(move-vectormath-consumer PRIVATE move::vectormath)
add_test(NAME move-vectormath-consumer COMMAND move-vectormath-consumer)
