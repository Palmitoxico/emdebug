# file CMakeLists.txt

cmake_minimum_required(VERSION 3.1.0 FATAL_ERROR)

project(EmDebug)

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(wxWidgets 3.0.0 REQUIRED COMPONENTS stc gl aui adv core net base xml)

include( "${wxWidgets_USE_FILE}" )

add_library(gdbmi src/gdbmi.cpp)

add_library(ctrlexe src/ctrlexe.cpp)

add_library(simple_exception src/simple_exception.cpp)

add_executable(emdebug src/main.cpp)

set_property(TARGET emdebug ctrlexe gdbmi simple_exception PROPERTY CXX_STANDARD 11)

target_link_libraries (emdebug gdbmi simple_exception ctrlexe ${wxWidgets_LIBRARIES})
