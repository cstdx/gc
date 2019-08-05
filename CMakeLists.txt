cmake_minimum_required(VERSION 3.2)
project(gc)

set(CMAKE_MODULE_PATH "${CMAKE_MODULE_PATH};${CMAKE_CURRENT_SOURCE_DIR}/cmake")

add_compile_options(-Wall -Werror -pedantic)

set(GC_LIBRARY ${CMAKE_PROJECT_NAME})
set(GC_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

option(STDX_GC_TEST "Enable tests" OFF)

include(Coverage)

if (STDX_GC_TEST)
    enable_testing()
    add_coverage_target("*/tests/*")
    add_definitions(-DSTDX_GC_TEST=1)
    add_subdirectory(tests)
endif()

add_library(${GC_LIBRARY} gc.c)
