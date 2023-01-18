cmake_minimum_required(VERSION 3.22)
project(Task_6_IPC)

set(CMAKE_CXX_STANDARD 17)

find_package(Boost 1.80.0 COMPONENTS system filesystem REQUIRED)

include_directories(${Boost_INCLUDE_DIR})

if (NOT Boost_FOUND)
    message(FATAL_ERROR "Could not find boost!")
endif ()

file(MAKE_DIRECTORY ./build/downloads)

add_executable(client client.cpp)

add_executable(server server.cpp)

