cmake_minimum_required(VERSION 3.5)

project(dz_03 LANGUAGES CXX)


add_executable(print_ip print_ip.cpp)
set_target_properties(print_ip PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)

add_executable(meta_error meta_error.cpp)
set_target_properties(meta_error PROPERTIES CXX_STANDARD 17 CXX_STANDARD_REQUIRED ON)
