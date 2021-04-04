#
# directory structure: . ├── build ├── CMakeLists.txt ├── include |   └── *.h
# └── src └── *.cpp
#
# ##############################################################################
# usage: execute "cmake .." from build directory
# ##############################################################################

cmake_minimum_required(VERSION 3.13)

#
# to use gcc-10 and g++-10 instead of default c / c++ (must come before project
# line)
#
# set(CMAKE_C_COMPILER "gcc-10") set(CMAKE_CXX_COMPILER "g++-10")

# set following lines to comment to use clang instead of default c / c++
# set(CMAKE_C_COMPILER "clang") set(CMAKE_CXX_COMPILER "clang++")

# find_program(CLANGTIDY clang-tidy) if(CLANGTIDY) set(CMAKE_CXX_CLANG_TIDY
# ${CLANGTIDY}) else() message(SEND_ERROR "clang-tidy requested, but executable
# not found") endif()

# find_program(CPPCHECK cppcheck) if(CPPCHECK) set(CMAKE_CXX_CPPCHECK
# ${CPPCHECK}) else() message(SEND_ERROR "cppcheck requested, but executable not
# found") endif()

#
# project name (=identical to executable name)
#
project(sudoku)

# globally define the required C++ Standard; supported: 98, 11, 14, 17, and 20
# (since cmake 3.12.2)
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

#
# Compiler flags (since cmake 3.12.2 )
#
set(CMAKE_CXX_FLAGS
    "${CMAKE_CXX_FLAGS} -O3 -Wall -pedantic -Wno-sign-compare -Wno-invalid-source-encoding"
)
# set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -std=c++17 -Wall
# -DGSL_THROW_ON_CONTRACT_VIOLATION") set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g
# -std=c++17 -Wall -DGSL_UNENFORCED_ON_CONTRACT_VIOLATION")

# // GSL usage for error handling of pre-/post conditions (use via #include
# "gsl/gsl") // // There are three configuration options for this GSL
# implementation's behavior // when pre/post conditions on the GSL types are
# violated: // // 1. GSL_TERMINATE_ON_CONTRACT_VIOLATION: std::terminate will be
# called (default) // 2. GSL_THROW_ON_CONTRACT_VIOLATION: a gsl::fail_fast
# exception will be thrown // 3. GSL_UNENFORCED_ON_CONTRACT_VIOLATION: nothing
# happens

#
# configure qt
#
# Instruct CMake to run moc et.al. automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

#
# add include directories for header files)
#
# Find includes in corresponding build directories
include_directories(${PROJECT_SOURCE_DIR}/include)
include_directories(${PROJECT_SOURCE_DIR}/../../../include)
include_directories(
  /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include
)

# Can manually add the sources using the set command as follows: set(SOURCES
# hello.cpp)

# However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES src/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

#
# add target_link_libraries AFTER definition of executable!
#

#
# fmt library
#
find_package(fmt)
target_link_libraries(${PROJECT_NAME} fmt::fmt)

#
# Use the Widgets module from Qt 6.
#
# Find the QtWidgets library
find_package(
  Qt6
  COMPONENTS Widgets
  REQUIRED)
target_link_libraries(${PROJECT_NAME} Qt6::Widgets)
