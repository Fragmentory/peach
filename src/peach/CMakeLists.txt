# Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
# SPDX-License-Identifier: MIT

add_library(peach)

target_sources(peach PUBLIC
    ${CMAKE_CURRENT_LIST_DIR}/pulp/peach_application.cpp
    ${CMAKE_SOURCE_DIR}/src/test/collection/agency/test.cpp
)

set(TEST_COLLECTION_DIRECTORY "${CMAKE_SOURCE_DIR}/src/test/collection")

include(${CMAKE_SOURCE_DIR}/build/subdirectories.cmake)
get_subdirectories(PROJECT_SOURCES_SUBDIRS ${CMAKE_CURRENT_LIST_DIR})
get_subdirectories(TEST_COLLECTION_SUBDIRS ${TEST_COLLECTION_DIRECTORY})

target_include_directories(peach PUBLIC
    ${CMAKE_SOURCE_DIR}/bin
    ${CMAKE_CURRENT_LIST_DIR}
    ${PROJECT_SOURCES_SUBDIRS}
    ${TEST_COLLECTION_DIRECTORY}
    ${TEST_COLLECTION_SUBDIRS}
)

# target_compile_options(tinyusb_board INTERFACE -Wno-switch-enum)
# target_compile_options(tinyusb_device INTERFACE -Wno-switch-enum)
target_link_libraries(peach
    hardware_adc
    hardware_clocks
    hardware_exception
    hardware_flash
    hardware_i2c
    hardware_pio
    hardware_pwm
    hardware_spi
    hardware_sync
    hardware_uart

    pico_stdlib
    pico_i2c_slave

    tinyusb_board
    tinyusb_device

    zero
    unity
)
