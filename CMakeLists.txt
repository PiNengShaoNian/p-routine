cmake_minimum_required(VERSION 3.10)
project(p_routine C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -g")

add_executable(p_routine.out main.c coroutine.c sched.c init_idle.c timer.c)