# The following five lines of boilerplate have to be in your project's
# CMakeLists in this exact order for cmake to work correctly
cmake_minimum_required(VERSION 3.5)

if (NOT UNIX)
    message(FATAL_ERROR "Unsupported OS")
endif ()

include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(espiot)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra -Wpedantic")

list(APPEND CMAKE_MODULE_PATH ${CMAKE_SOURCE_DIR}/cmake)
include(address-sanitizer)
include(undefined-sanitizer)
include(clang-tidy)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_custom_target(lint)
