cmake_minimum_required(VERSION 3.22)
project(Task_9_QtBasicConcepts)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS
        Core
        Widgets
        REQUIRED)

add_executable(Task_9_QtBasicConcepts main.cpp counter.cpp counter.h)
target_link_libraries(Task_9_QtBasicConcepts
        Qt6::Core
        Qt6::Widgets
        )

