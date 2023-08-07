cmake_minimum_required(VERSION 3.4)

set(APP_NAME timer)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

project(AsyncTimer)

add_executable(${APP_NAME} main.cpp)
