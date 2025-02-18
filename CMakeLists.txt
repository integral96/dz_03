cmake_minimum_required(VERSION 3.5)

project(task_03 LANGUAGES CXX)


add_executable(task_2047 task_2047.cpp)
set_target_properties(task_2047 PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

add_executable(print_ip print_ip.cpp my_tuple.hpp)
set_target_properties(print_ip PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)


