#
# directory structure:
#.
#├── build
#├── CMakeLists.txt
#├── include
#|   └── *.h
#└── source
#    └── *.cpp
#
##################################################
# usage: execute "cmake .." from build directory
##################################################

cmake_minimum_required(VERSION 3.13)

#
### to use gcc-10 and g++-10 instead of default c / c++ (must come before project line)
#
#set(CMAKE_C_COMPILER "gcc-10")
#set(CMAKE_CXX_COMPILER "g++-10")

# set following lines to comment to use clang instead of default c / c++
#set(CMAKE_C_COMPILER "clang")
#set(CMAKE_CXX_COMPILER "clang++")

#
# project name (=identical to executable name)
#
project(sudoku)

#
# Compiler flags (since cmake 3.12.2 supported: 98, 11, 14, 17, and 20)
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -O3 -std=c++17 -Wall -pedantic -Wno-sign-compare -Wno-invalid-source-encoding")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -DGSL_THROW_ON_CONTRACT_VIOLATION")
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION")

#// GSL usage for error handling of pre-/post conditions (use via #include "gsl/gsl")
#//
#// There are three configuration options for this GSL implementation's behavior
#// when pre/post conditions on the GSL types are violated:
#//
#// 1. GSL_TERMINATE_ON_CONTRACT_VIOLATION: std::terminate will be called (default)
#// 2. GSL_THROW_ON_CONTRACT_VIOLATION: a gsl::fail_fast exception will be thrown
#// 3. GSL_UNENFORCED_ON_CONTRACT_VIOLATION: nothing happens

#
# configure qt
#
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)
if(CMAKE_VERSION VERSION_LESS "3.7.0")
    set(CMAKE_INCLUDE_CURRENT_DIR ON)
endif()
# Find the QtWidgets library
find_package(Qt5 COMPONENTS Widgets REQUIRED)
#find_package(Qt5Widgets)
#find_package(Qt5Core)
#find_package(Qt5Gui)
#get_target_property(QtCore_location Qt5::Core LOCATION)

#
# add include directories for header files)
#
# Find includes in corresponding build directories
include_directories(${PROJECT_SOURCE_DIR}/include ${PROJECT_SOURCE_DIR}/../../../include ${PROJECT_SOURCE_DIR}/../../../include/GSL/include ${PROJECT_SOURCE_DIR}/../../../include/range-v3/include /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include)

#Can manually add the sources using the set command as follows:
#set(SOURCES hello.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES source/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

#
#  add target_link_libraries AFTER definition of executable!
#

#
# fmt library
#
find_package(fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt)

#
# Use the Widgets module from Qt 5.
#
#target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)
target_link_libraries(${PROJECT_NAME} Qt5::Widgets)

