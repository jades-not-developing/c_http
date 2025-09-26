cmake_minimum_required(VERSION 3.20)

project(CHttp)

add_library(chttp STATIC
    src/chttp.c
)
target_include_directories(chttp PRIVATE include)
target_link_libraries(chttp PRIVATE ws2_32)

add_executable(chttp_test
    src/chttp_test.c
)
target_include_directories(chttp_test PRIVATE include)
target_link_libraries(chttp_test PRIVATE chttp)