cmake_minimum_required(VERSION 3.1)
project(test)

set(CMAKE_CXX_STANDARD 11)

add_executable(test_RsmSource test_RsmSource.cpp RsmSource.cpp)
