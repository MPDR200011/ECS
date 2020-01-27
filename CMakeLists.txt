cmake_minimum_required(VERSION 3.12)

project("ECS implementation")

set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

file(GLOB_RECURSE TEST_SRC src/*.cpp) 

add_executable(tests ${TEST_SRC})

target_compile_options(tests
    PUBLIC -std=c++17
)