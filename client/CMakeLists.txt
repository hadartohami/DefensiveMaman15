cmake_minimum_required(VERSION 3.18.4)
project(main_cpp)

set(CMAKE_CXX_STANDARD 17)

add_executable(main_cpp main.cpp Protocol.h RequestResponseHandler.cpp)

set(Boost_INCLUDE_DIR 3rd-party/boost_1_81_0)
set(Boost_LIBRARY_DIR 3rd-party/boost_1_81_0)
find_package(Boost 1.81.0)
include_directories(${Boost_INCLUDE_DIR})