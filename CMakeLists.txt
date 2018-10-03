
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

cmake_minimum_required(VERSION 3.12.2)

#
### to use gcc-8 and g++-8 instead of clang  (must come before project line)
#
# set following lines to comment to use clang
#set(CMAKE_C_COMPILER "gcc-8")
#set(CMAKE_CXX_COMPILER "g++-8")

#
# project name (=identical to executable name)
#
project(qt-widget_array)

#
# configure qt
#
# Find includes in corresponding build directories
set(CMAKE_INCLUDE_CURRENT_DIR ON)
# Instruct CMake to run moc automatically when needed.
set(CMAKE_AUTOMOC ON)

# Find the QtWidgets library
find_package(Qt5Widgets)
find_package(Qt5Core)
find_package(Qt5Gui)

get_target_property(QtCore_location Qt5::Core LOCATION)

#
# Compiler flags (since cmake 3.12.2 supported standards are 98, 11, 14, 17, and 20)
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -O3 -Wall -pedantic")

#
# add include directories for header files, includes apple specific version of system includes)
#
include_directories(${PROJECT_SOURCE_DIR}/include /Users/daniel/Programming/cpp/include /Applications/Xcode.app/Contents/Developer/Toolchains/XcodeDefault.xctoolchain/usr/include)

#Can manually add the sources using the set command as follows:
#set(SOURCES xyz.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES source/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)

### compilation and code completion tools (currently must come after project() !
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

#
# verbose output in Makefile
#
#set(CMAKE_VERBOSE_MAKEFILE on)

#install(TARGETS hello)
