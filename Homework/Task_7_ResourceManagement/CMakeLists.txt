cmake_minimum_required(VERSION 3.22)
project(Task_7_ResourceManagement)

set(CMAKE_CXX_STANDARD 17)
enable_testing()
find_package(GTest REQUIRED)
#target_link_libraries(GoogleTest gtest gtest_main)
add_executable(Task_7_ResourceManagement shared_ptr.cpp shared_ptr_cb.h shared_ptr.h)
target_link_libraries(Task_7_ResourceManagement GTest::gtest GTest::gtest_main)
