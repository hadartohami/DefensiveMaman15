cmake_minimum_required(VERSION 3.24)
project(main_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(main_cpp main.cpp Protocol.h RequestResponseHandler.cpp)

set(Boost_INCLUDE_DIR /Users/htohami/boost_1_81_0)
set(Boost_LIBRARY_DIR /Users/htohami/boost_1_81_0)
find_package(Boost 1.81.0)
include_directories(${Boost_INCLUDE_DIR})