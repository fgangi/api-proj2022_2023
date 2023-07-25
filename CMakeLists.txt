cmake_minimum_required(VERSION 3.25)
project(api2223 C)

set(CMAKE_C_STANDARD 11)

add_executable(api2223 main.c)

set(CFLAGS += -Wall -Werror -std=gnu11 -O2)
set(LDFLAGS += -lm)