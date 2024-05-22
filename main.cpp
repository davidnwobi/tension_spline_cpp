#include <iostream>
#include <Eigen/Dense>
#include "src/tension_spline.h"
 
 
int main()
{   
    Eigen::VectorXd t1{{-1, 1, 8, 14, 20, 25, 30, 32, 34, 36, 40, 43, 46, 47, 47.75, 48, 49, 52, 55, 59, 63, 70, 73,
          77, 79, 83, 84, 85, 86, 87, 88}};
    Eigen::VectorXd y1{{0, 2, 5, 6.5, 8, 8.5, 9, 10, 12, 15, 21, 25, 28, 22, 16, 12, 10, 9.5, 9, 8.5, 7.5, 5, 4, 5.5,
          8, 13, 15, 13, 10, 5, 1}};

    double tau = 10;
    
    Eigen::VectorXd t_spline = Eigen::VectorXd::LinSpaced(1000, -3, 89);
    TensionSpline spline = TensionSpline(t1, y1, tau);
    Eigen::VectorXd y_spline = spline.eval_vector(t_spline);

    std::cout << "y_spline =" << std::endl << y_spline << std::endl;
}