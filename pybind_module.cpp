#include <pybind11/pybind11.h>
#include <pybind11/eigen.h>
#include "src/tension_spline.cpp"

namespace py = pybind11;

PYBIND11_MODULE(tension_spline_cpp, m){
    py::class_<TensionSpline>(m, "TensionSpline")
        .def(py::init<Eigen::VectorXd, Eigen::VectorXd, double>())
        .def("eval_vector", &TensionSpline::eval_vector);
}