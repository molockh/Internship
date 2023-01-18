cmake_minimum_required(VERSION 3.22)
project(Task_11_UnitTesting)

set(CMAKE_CXX_STANDARD 17)

enable_testing()
find_package(GTest REQUIRED)

add_executable(Task_11_UnitTesting main.cpp GetProductAmount.cpp IsLeapYear.cpp)
target_link_libraries(Task_11_UnitTesting GTest::gtest GTest::gtest_main)
