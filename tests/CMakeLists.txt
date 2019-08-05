find_package(cmocka REQUIRED)

set(name gc_test)

add_executable(${name}
    tests.c
    utils.c
)

add_test(${name} ${name})

target_link_libraries(${name}
    ${CMOCKA_LIBRARY}
    ${GC_LIBRARY}
)

target_include_directories(${name}
    PRIVATE ${CMOCKA_INCLUDE_DIR}
    PRIVATE ${GC_INCLUDE_DIR}
)
