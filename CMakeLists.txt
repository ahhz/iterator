###############################################################################
#
# Require 3.0 for INTERFACE projects

cmake_minimum_required (VERSION 3.0)

###############################################################################
#
# Create an interface library for the header-only library
#

add_library(iterator INTERFACE)
target_include_directories(iterator INTERFACE "include/")
target_link_libraries(iterator INTERFACE boost_interface)

###############################################################################
#
# Find header files and put in separate target for IDE
#

file(GLOB_RECURSE MY_HDRS "*.h")
add_custom_target(iterator_headers SOURCES ${MY_HDRS})

###############################################################################
#
# Create an executable using the header only interface
#

add_executable(iterator_demo demo.cpp)
target_link_libraries(iterator_demo PRIVATE iterator)

###############################################################################