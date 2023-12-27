#[[
# directory structure:
# .
# ├── build
# ├── CMakeLists.txt
# ├── include
# |   └── *.[h|hpp]
# └── source
#     └── *.[cpp|cxx]
#
# ##############################################################################
# to configure execute from build directory:
#                cmake ..
#             or cmake .. -G "Ninja"
#             or cmake .. -G "Visual Studio 17 2022" -T host=x64 -A x64
#
# to build execute from build directory:
#
#                cmake --build .
#             or ninja
#
# ##############################################################################
#]]

cmake_minimum_required(VERSION 3.25)

# to use gcc-13 and g++-13 (must come before project line)
#[[
# set(CMAKE_C_COMPILER "gcc-13")
# set(CMAKE_CXX_COMPILER "g++-13")
#]]

project(
  sudoku
  VERSION 0.1
  LANGUAGES CXX)

# set(CMAKE_VERBOSE_MAKEFILE ON)  // alternative: make VERBOSE=1
# add_definitions(-Dxxx)

# globally define the required C++ Standard; supported: 98, 11, 14, 17, and 20
# (since cmake 3.12.2)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

#
# configure qt
#
# Instruct CMake to run moc et.al. automatically when needed.
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

# set a default build type: Debug | RelWithDebInfo | Release | MinSizeRel
if(NOT CMAKE_BUILD_TYPE)
  set(CMAKE_BUILD_TYPE "Debug")
  message(STATUS "Build type not specified: use Debug mode by default.")
endif()

if(CMAKE_BUILD_TYPE EQUAL "Debug")
  if(MSVC)
    # warning level 4 and all warnings as errors
    add_compile_options(/W4 /WX)
  else()
    # lots of warnings and all warnings as errors
    add_compile_options(-Wall -Wextra -pedantic -Werror)
  endif()
endif()

set(SOURCES
    src/main.cpp
    src/dyn_assert.cpp
    src/sudoku_class.cpp
    src/sudoku_print.cpp
    src/sudoku_solve.cpp
    src/sudoku_solve_helper.cpp
    src/w_sudoku.cpp
    src/w_sudoku_view.cpp)

set(HEADERS
    include/dyn_assert.h
    include/PlainTextEditIODevice.h
    include/sudoku_class.h
    include/sudoku_print.h
    include/sudoku_read.h
    include/sudoku_solve.h
    include/sudoku_solve_helper.h
    include/w_sudoku.h
    include/w_sudoku_view.h)

set(EXEC_NAME ${PROJECT_NAME})
add_executable(${EXEC_NAME} ${HEADERS} ${SOURCES})

# target link libraries have to be added AFTER add_executable or add_library!
target_include_directories(${EXEC_NAME} PRIVATE include)
target_include_directories(${EXEC_NAME} PRIVATE ${CMAKE_SOURCE_DIR}/include)
target_include_directories(${EXEC_NAME}
                           PRIVATE ${CMAKE_SOURCE_DIR}/../../include)

#
# add target_link_libraries AFTER definition of executable!
#
find_package(
  Qt6
  COMPONENTS Widgets
  REQUIRED)

target_link_libraries(${EXEC_NAME} PRIVATE Qt6::Widgets)

find_package(fmt CONFIG REQUIRED)
target_link_libraries(${EXEC_NAME} PRIVATE fmt::fmt-header-only)
