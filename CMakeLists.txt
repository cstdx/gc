cmake_minimum_required(VERSION 3.2)
project(gc)

add_compile_options(-Wall -Werror -pedantic)

set(GC_LIBRARY ${CMAKE_PROJECT_NAME})
set(GC_INCLUDE_DIR ${CMAKE_CURRENT_LIST_DIR})

option(STDX_GC_TEST "Enable tests" OFF)

if (STDX_GC_TEST)
    find_program(LCOV lcov)
    find_program(GCOV gcov)

    if (LCOV AND GCOV)
        set(covname cov.info)
        add_compile_options(-fprofile-arcs -ftest-coverage)
        add_link_options(--coverage)
        add_custom_target(cov DEPENDS ${covname})
        add_custom_command(
            OUTPUT  ${covname}
            COMMAND ${LCOV}
                --directory .
                --base-directory .
                --gcov-tool ${GCOV}
                --capture -o ${covname}
        )
        set_directory_properties(PROPERTIES
            ADDITIONAL_CLEAN_FILES ${covname}
        )

    else()
        message(WARNING "coverage tools were not found, target `cov` will not be generated")
    endif()

    enable_testing()
    add_definitions(-DSTDX_GC_TEST=1)
    add_subdirectory(tests)
endif()

add_library(${GC_LIBRARY} gc.c)
