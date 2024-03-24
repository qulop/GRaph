cmake_minimum_required(VERSION 3.4 FATAL_ERROR)
project("RubyUtility")

option(RTOOLS_STATIC_BUILD "" ON)

set(RTOOLS_SRC "")

if (RTOOLS_STATIC_BUILD)
    add_library(${CMAKE_PROJECT_NAME} STATIC ${RTOOLS_SRC})
endif()

target_include_directories(rtools/include)
