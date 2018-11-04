# Disable in-source builds to prevent source tree corruption.
if(" ${CMAKE_SOURCE_DIR}" STREQUAL " ${CMAKE_BINARY_DIR}")
    message(FATAL_ERROR "
    FATAL: In-source builds are not allowed.
       You should create a separate directory for build files.
    ")
endif()

cmake_minimum_required (VERSION 2.8)
project(utils)

set(CMAKE_MODULE_PATH ${PROJECT_SOURCE_DIR}/cmake)

set (Tutorial_VERSION_MAJOR 1)
set (Tutorial_VERSION_MINOR 0)

ADD_DEFINITIONS("-Wall -g -DDEBUG")
ADD_SUBDIRECTORY(package)
#ADD_SUBDIRECTORY(src)
ADD_SUBDIRECTORY(test)

set(CMAKE_BUILD_TYPE Debug CACHE STRING "set build type to debug")
set(CMAKE_VERBOSE_MAKEFILE on)
