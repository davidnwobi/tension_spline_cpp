cmake_minimum_required(VERSION 3.20)
project(TensionSpline)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(EIGEN_DIR C:/Users/dnwob/GitHub/eigen)
# set(PYBIND11_DIR C:/Users/dnwob/GitHub/pybind11)

# set(Python3_ROOT_DIR C:/Users/dnwob/AppData/Roaming/Python/Python312)
# set(PYTHON_LIBRARY ${Python3_ROOT_DIR}/libs/python3.lib)
# find_package (Python3 COMPONENTS Interpreter Development)


add_subdirectory(src)
add_subdirectory(pybind11)

include_directories(pybind11/include)
include_directories(${EIGEN_DIR})
include_directories(src)

pybind11_add_module(tension_spline_cpp pybind_module.cpp)
target_link_libraries(tension_spline_cpp PUBLIC tension_spline)


# add_executable(TensionSpline main.cpp pybind_module.cpp)
# target_link_libraries(TensionSpline PRIVATE tension_spline)