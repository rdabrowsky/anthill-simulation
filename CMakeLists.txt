cmake_minimum_required(VERSION 3.28)
project(anthill_simulation C)

set(SRC_DIR ${CMAKE_SOURCE_DIR}/src)

set(CMAKE_C_STANDARD 11)

add_executable(main
        ${SRC_DIR}/main.c
)
