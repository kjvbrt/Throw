cmake_minimum_required(VERSION 3.0 FATAL_ERROR)
project(Throw)

# Saving compile commands in .json file
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

# Find and setup ROOT
find_package(ROOT)
include(${ROOT_USE_FILE})

# Create Throw static library
add_library(Throw STATIC Throw.cxx)
target_link_libraries(Throw ${ROOT_LIBRARIES})

# Create Test executable
add_executable(Test Test.cxx)
target_link_libraries(Test Throw)
