cmake_minimum_required(VERSION 3.0)

project(ProgVM64Assembler)

set(SOURCE main.cpp)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -fwrapv -g")

add_executable(${PROJECT_NAME} ${SOURCE})