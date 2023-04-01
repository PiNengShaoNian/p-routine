cmake_minimum_required(VERSION 3.10)
project(p_routine C)

set(CMAKE_C_STANDARD 99)

add_executable(p_routine.out main.c common.h coroutine.h 
                             coroutine.c sched.h sched.c init.c)