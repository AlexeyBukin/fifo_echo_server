cmake_minimum_required(VERSION 3.15)
project(fifo_echo_server C)

set(CMAKE_C_STANDARD 99)

set(SRC
        src/main.c)
add_executable(fifo_echo_server ${SRC})