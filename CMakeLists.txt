cmake_minimum_required(VERSION 3.27)
project(aoc2024)

set(CMAKE_CXX_STANDARD 20)
add_executable(aoc2024 main.cpp
        days/day14/day14.cpp
        utils/file_utils.cpp
        )
set_target_properties(aoc2024 PROPERTIES CXX_STANDARD 20)
target_compile_definitions(aoc2024 PRIVATE BASE_DIR="${CMAKE_CURRENT_SOURCE_DIR}")