
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

cmake_minimum_required(VERSION 3.5.2)

#
# project name (=identical to executable name)
#
project(qt-basic)

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
# Compiler flags
#
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -g -Wall")

#
# add include directories for header files)
#
include_directories(${PROJECT_SOURCE_DIR}/include /Users/daniel/Programming/cpp/include)

#Can manually add the sources using the set command as follows:
#set(SOURCES xyz.cpp)

#However, the file(GLOB...) allows for wildcard additions:
file(GLOB SOURCES source/*.cpp include/*.h)

add_executable(${PROJECT_NAME} ${SOURCES})

# Use the Widgets module from Qt 5.
target_link_libraries(${PROJECT_NAME} Qt5::Widgets Qt5::Core Qt5::Gui)

#
# verbose output in Makefile
#
#set(CMAKE_VERBOSE_MAKEFILE on)

#install(TARGETS hello)
