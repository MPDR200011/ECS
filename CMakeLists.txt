cmake_minimum_required(VERSION 3.12)

project("ECS implementation")
set(CMAKE_EXPORT_COMPILE_COMMANDS TRUE)

file(GLOB_RECURSE ECS_SRC ecs/*.cpp)
add_library(ECS
    ${ECS_SRC}
)

target_compile_options(ECS
    PUBLIC -std=c++17
)

file(GLOB_RECURSE TEST_SRC tests-src/*.cpp)
message(STATUS )
add_executable(tests 
    ${TEST_SRC}
)

target_compile_options(tests
    PUBLIC -std=c++17
)

target_include_directories(tests
    PRIVATE ./ecs
)

target_link_libraries(tests ECS)
