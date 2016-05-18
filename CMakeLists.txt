cmake_minimum_required (VERSION 2.8)
project(iterator)
include_directories("include/")
file(GLOB_RECURSE MY_HDRS "*.h")
add_executable(iterator_demo demo.cpp)
add_library(iterator STATIC dummy.cpp ${MY_HDRS})

###############################################################################
#
# Search for dependencies
# Boost C++ Libraries support - required
find_package(Boost 1.55 REQUIRED)
include_directories(${Boost_INCLUDE_DIRS})

###############################################################################