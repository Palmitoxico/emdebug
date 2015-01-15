# file CMakeLists.txt

cmake_minimum_required(VERSION 2.8)

project(EmDebug)

find_package(wxWidgets 3.0.0 REQUIRED COMPONENTS gl aui adv core net base xml)

include( "${wxWidgets_USE_FILE}" )

add_library(gdbmi src/gdbmi.cpp)

add_executable(emdebug src/main.cpp)

target_link_libraries (emdebug gdbmi  ${wxWidgets_LIBRARIES})
