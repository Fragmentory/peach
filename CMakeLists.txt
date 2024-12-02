# Copyright (c) 2024, Roman Koch, koch.roman@gmail.com
# SPDX-License-Identifier: MIT

# ---------------------------------------------------------
# CMake, SDK, Compiler and Project config
# ---------------------------------------------------------

cmake_minimum_required(VERSION 3.22)

# ---------------------------------------------------------
# download pico sdk
# ---------------------------------------------------------

unset(ENV{PICO_SDK_PATH})

# set(ENV{PICO_SDK_PATH} "${CMAKE_SOURCE_DIR}/../pico-sdk")
set(ENV{PICO_SDK_FETCH_FROM_GIT_PATH} "${CMAKE_SOURCE_DIR}/../pico-sdk")
set(ENV{PICO_SDK_FETCH_FROM_GIT} ON)

if(NOT EXISTS $ENV{PICO_SDK_PATH})
        file(MAKE_DIRECTORY "${CMAKE_SOURCE_DIR}/../pico-sdk")
endif()

include(build/pico_sdk_import.cmake)
include(build/pico_extras_import_optional.cmake)

set(ENV{PICOTOOL_FETCH_FROM_GIT_PATH} "${CMAKE_SOURCE_DIR}/../picotool")

include(build/cppcheck.cmake)

include(build/install_hooks.cmake)
install_hooks("pre-commit")

# ---------------------------------------------------------
# project definitions
# ---------------------------------------------------------
project(peach-zero
        VERSION 1.0.0.0
        DESCRIPTION "Platform Elements And Component Handling"
        LANGUAGES C CXX ASM)

set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

add_compile_options(
        -Wall
        -Wextra

        # -Werror
        -Wno-format # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
        -Waddress
        -Wconversion
        -Wswitch-enum

        # -fpack-struct
        -g
        -O0
)

pico_sdk_init()

message(STATUS "Unit testing enabled")
add_compile_options(-DENABLE_UNIT_TESTS=1)

# add_link_options(-T ${CMAKE_CURRENT_LIST_DIR}/build/memmap_extended.ld)

# ---------------------------------------------------------
# Platform variants management / select hardware platform
# ---------------------------------------------------------
if(NOT HARDWARE_PLATFORM)
        message(WARNING "platform is not defined, default is PLATFORM_PROTO")

        # set default
        # available platforms are: PLATFORM_PROTO, PLATFORM_SERIE
        set(HARDWARE_PLATFORM "PLATFORM_SERIE")
endif()

if(${HARDWARE_PLATFORM} STREQUAL "PLATFORM_SERIE")
        set(HARDWARE_IDENTIFIER 2)
        set(HARDWARE_NUMBER 1)
        set(HARDWARE_VARIANT 0)
elseif(${HARDWARE_PLATFORM} STREQUAL "PLATFORM_PROTO")
        set(HARDWARE_IDENTIFIER 1)
        set(HARDWARE_NUMBER 1)
        set(HARDWARE_VARIANT 0)
else()
        set(HARDWARE_IDENTIFIER 1)
        set(HARDWARE_NUMBER 1)
        set(HARDWARE_VARIANT 0)
endif()

message(STATUS "platform is ${HARDWARE_PLATFORM} (internal ID:${HARDWARE_IDENTIFIER} NUM:${HARDWARE_NUMBER} VAR:${HARDWARE_VARIANT})")

# ---------------------------------------------------------
# Product variants management / select application
# ---------------------------------------------------------
if(NOT FIRMWARE_PRODUCT)
        message(WARNING "product is not defined, default is TEST_APPLICATION")

        # set default device name
        # available devices are: TEST_APPLICATION, APPLICATION
        set(FIRMWARE_PRODUCT "TEST_APPLICATION")
endif()

if(${FIRMWARE_PRODUCT} STREQUAL "APPLICATION")
        set(FIRMWARE_IDENTIFIER 2)
        set(FIRMWARE_VENDOR 0xCAFE)
elseif(${FIRMWARE_PRODUCT} STREQUAL "TEST_APPLICATION")
        set(FIRMWARE_IDENTIFIER 1)
        set(FIRMWARE_VENDOR 0xBEEF)
else()
        set(FIRMWARE_IDENTIFIER 1)
        set(FIRMWARE_VENDOR 0xBEEF)
endif()

message(STATUS "product is ${FIRMWARE_PRODUCT} (internal ID:${FIRMWARE_IDENTIFIER} REV:${PROJECT_VERSION})")

# ---------------------------------------------------------
# Check submodules
# ---------------------------------------------------------
if(NOT EXISTS "${CMAKE_SOURCE_DIR}/src/test/unity/.git")
        message(SEND_ERROR "The test submodule are not found. Please run git submodule update --init --recursive")
else()
        message(STATUS "Test submodule found.")
endif()

# ---------------------------------------------------------
# Source code structure / configure executable
# ---------------------------------------------------------
if(CMAKE_BUILD_TYPE STREQUAL "Release")
        set(EXECUTABLE_PREFIX "XXX_")
else()
        set(EXECUTABLE_PREFIX "")
endif()

set(EXECUTABLE_NAME "${EXECUTABLE_PREFIX}${PROJECT_NAME}_${FIRMWARE_PRODUCT}_${HARDWARE_PLATFORM}_${HARDWARE_NUMBER}_${HARDWARE_VARIANT}")

string(TOLOWER ${EXECUTABLE_NAME} EXECUTABLE_NAME)

# Attention: remove CMakeCache.txt after option update
option(USE_PROTOCOL_TEST "Use protocol test module" OFF)

add_subdirectory(${CMAKE_SOURCE_DIR}/src/peach)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/zero)
add_subdirectory(${CMAKE_SOURCE_DIR}/src/test/unity)

add_executable(${EXECUTABLE_NAME})

# ---------------------------------------------------------
# Revision description generator / create revision.h
# ---------------------------------------------------------
include(${PROJECT_SOURCE_DIR}/build/revision_header.cmake)
revision_header(${EXECUTABLE_NAME})

# ---------------------------------------------------------
# Build Number generator / generate build number info
# ---------------------------------------------------------
include(${CMAKE_SOURCE_DIR}/build/build_number_handler.cmake)
handle_build_number(${EXECUTABLE_NAME})

# ---------------------------------------------------------
# Nice grafiti / pretty project grafiti
# ---------------------------------------------------------
include("${CMAKE_SOURCE_DIR}/build/graffiti.cmake")
add_dependencies(${EXECUTABLE_NAME} ascii_art)

# ---------------------------------------------------------
# Executable management / project dependencies
# ---------------------------------------------------------
target_sources(${EXECUTABLE_NAME} PUBLIC
        ${CMAKE_CURRENT_LIST_DIR}/src/firmware.cpp
)

target_include_directories(${EXECUTABLE_NAME} PUBLIC
        ${PROJECT_SOURCE_DIR}/src/
)

# Uncomment this line to enable fix for Errata RP2040-E5 (the fix requires use of GPIO 15)
# target_compile_definitions(dev_hid_composite PUBLIC PICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)
target_link_libraries(${EXECUTABLE_NAME} PUBLIC
        pico_stdlib
        unity
        zero
        peach
)

target_link_options(${EXECUTABLE_NAME} PRIVATE
        -Wl,--gc-sections
        -Wl,-Map=output.map
)

# ---------------------------------------------------------
# Extended project configurations
# ---------------------------------------------------------

# add url via pico_set_program_url
# example_auto_set_url(${PROJECT_NAME})
pico_enable_stdio_usb(${EXECUTABLE_NAME} 0)
pico_enable_stdio_uart(${EXECUTABLE_NAME} 1)

pico_add_extra_outputs(${EXECUTABLE_NAME})

# ---------------------------------------------------------
# Configure the Doxyfile template
# ---------------------------------------------------------
configure_file(${CMAKE_SOURCE_DIR}/build/Doxyfile.in ${CMAKE_BINARY_DIR}/Doxyfile @ONLY)

find_package(Doxygen REQUIRED)

add_custom_target(doc
        COMMAND ${DOXYGEN_EXECUTABLE} ${CMAKE_BINARY_DIR}/Doxyfile
        WORKING_DIRECTORY ${CMAKE_BINARY_DIR}
        COMMENT "Generating API documentation with Doxygen"
        VERBATIM)

# for auto build documentaton enable next line
# add_dependencies(${EXECUTABLE_NAME} doc)

# ---------------------------------------------------------
# Testing
# ---------------------------------------------------------
enable_testing()

include(CTest)
list(APPEND CMAKE_CTEST_ARGUMENTS "--output-on-failure")
list(APPEND CMAKE_CTEST_ARGUMENTS "--extra-verbose")

set(TEST_LIST_FILE "${CMAKE_SOURCE_DIR}/src/test/collection/test_list.cmake")

if(EXISTS ${TEST_LIST_FILE})
        include(${TEST_LIST_FILE})
else()
        message(FATAL_ERROR "Test list file ${TEST_LIST_FILE} not exists.")
endif()

