project(qs C)
set(CMAKE_C_STANDARD 99)
set (CMAKE_C_FLAGS "-Wall -Wextra -pedantic -g -O3 -std=c99")
add_executable(qs main.c)
