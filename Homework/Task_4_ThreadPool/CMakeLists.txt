cmake_minimum_required(VERSION 3.22)
project(Task_4_ThreadPool)

set(CMAKE_CXX_STANDARD 20)

add_executable(Task_4_ThreadPool main.cpp thread_pool.cpp thread_pool.h blocking_queue.h)
