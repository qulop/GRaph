cmake_minimum_required(VERSION 3.2)
project("TEST_dict")

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

file(GLOB SRC *.cpp)

add_executable(${PROJECT_NAME} ${SRC})