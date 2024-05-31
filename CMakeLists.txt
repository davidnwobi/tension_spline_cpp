cmake_minimum_required(VERSION 3.20)
project(TensionSpline)

# SET C++ STANDARD
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# CHECK FOR EIGEN3
IF( NOT EIGEN3_INCLUDE_DIR )
  MESSAGE( FATAL_ERROR "Please point the environment variable EIGEN3_INCLUDE_DIR to the include directory of your Eigen3 installation.")
ENDIF()

include_directories(${EIGEN3_INCLUDE_DIR})
message(STATUS "EIGEN3_INCLUDE_DIR: ${EIGEN3_INCLUDE_DIR}")
include_directories(src)

# BUILD TensionSpline C++ LIBRARY
set(SOURCES include/tension_spline.cpp include/tension_spline.h)
add_library(TensionSpline ${SOURCES})
target_include_directories(TensionSpline PUBLIC include)
target_link_libraries(TensionSpline PUBLIC)
# target_compile_options(TensionSpline PRIVATE -O3)

# GOOGLETEST
include(FetchContent)
FetchContent_Declare(
  googletest
  URL https://github.com/google/googletest/archive/03597a01ee50ed33e9dfd640b249b4be3799d395.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

#BUILD C++ TESTS
add_executable(
  tension_spline_test
  tests/tension_spline_tests.cpp
)

# Link the test executable against gtest_main and TensionSpline
target_link_libraries(
  tension_spline_test
  PRIVATE
  GTest::gtest_main
  TensionSpline
)



