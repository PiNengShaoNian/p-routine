cmake_minimum_required(VERSION 3.10)
project(p_routine C)

set(CMAKE_C_STANDARD 99)

set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -pthread -g")

add_executable(p_routine.out main.c common.h coroutine.h 
                             coroutine.c sched.h sched.c init_idle.c
                             timer.h timer.c)