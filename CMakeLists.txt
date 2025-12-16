cmake_minimum_required(VERSION 3.20)
project(mersenne LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

option(MERSENNE_BUILD_STATIC "Try to build statically (may fail with GMP on some systems)" OFF)
option(GMP_PATH "Path to GMP installation (optional)" "")

list(APPEND CMAKE_MODULE_PATH "${CMAKE_CURRENT_SOURCE_DIR}/cmake")

# Try to find GMP
if(GMP_PATH)
  set(GMP_INCLUDE_DIR "${GMP_PATH}/include")
  find_library(GMP_LIBRARY gmp PATHS "${GMP_PATH}/lib" NO_DEFAULT_PATH)
  find_library(GMPXX_LIBRARY gmpxx PATHS "${GMP_PATH}/lib" NO_DEFAULT_PATH)
endif()

find_package(GMP)
if(NOT GMP_FOUND)
  message(FATAL_ERROR "GMP not found. Install it with:\n"
    "  Ubuntu/Debian: sudo apt-get install libgmp-dev\n"
    "  macOS: brew install gmp\n"
    "  Windows: vcpkg install gmp:x64-windows\n"
    "Or specify GMP_PATH: cmake .. -DGMP_PATH=/path/to/gmp")
endif()

add_executable(mersenne
  src/main.cpp
  src/ll.cpp
  src/io.cpp
  src/cli.cpp
)

target_include_directories(mersenne PRIVATE include)
target_link_libraries(mersenne PRIVATE GMP::gmp GMP::gmpxx)

if(MERSENNE_BUILD_STATIC)
  set_target_properties(mersenne PROPERTIES LINK_SEARCH_START_STATIC ON LINK_SEARCH_END_STATIC ON)
endif()

# Nice warnings
if (MSVC)
  target_compile_options(mersenne PRIVATE /W4)
else()
  target_compile_options(mersenne PRIVATE -Wall -Wextra -Wpedantic)
endif()
