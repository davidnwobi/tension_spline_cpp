#include <Eigen/Dense>
#include "tension_spline.h"
#include <cmath>
#include <stdexcept>

TensionSpline::TensionSpline(Eigen::VectorXd t, Eigen::VectorXd y, double tau) : t(t), y(y), tau(tau) {fit();};


void TensionSpline::fit(){
    int n = t.size()-1;

    Eigen::VectorXd a_(n);
    Eigen::VectorXd b_(n);
    Eigen::VectorXd g_(n);

    if (t.size() != y.size())
        throw std::invalid_argument("t and y must have the same length");
    if (tau <= 0)
        throw std::invalid_argument("tau must be positive");
    if ((t.tail(n) - t.head(n)).minCoeff() < 0 )
        throw std::invalid_argument("t must be strictly increasing");
    if (t.size() < 3)
        throw std::invalid_argument("t and y must have at least 3 elements");

    h = t.tail(n) - t.head(n) ;
    g_ = std::pow(tau,2) * (y.tail(n) - y.head(n));
    a_ = 1 / h.array() - tau / (tau * h).array().sinh();
    b_ = tau * (tau * h).array().cosh() / (tau * h).array().sinh() - 1 / h.array();
  
    Eigen::MatrixXd A(n + 1, n + 1);
    for(int row = 1; row < n; row++ ){
        A(row, row-1) = a_(row-1);
        A(row, row) = b_(row-1) + b_(row);
        A(row, row+1) = a_(row);
        A(0,0) = 1;
        A(n,n) = 1;   
    }
    Eigen::VectorXd Y(n+1);
    Y.segment(1,n-1) = g_.tail(n-1) - g_.head(n-1);
    
    z = A.colPivHouseholderQr().solve(Y);
}


double TensionSpline::eval_spline(double x, int i){
    double sinh_tau_h = std::sinh(tau * h[i]);
    double sinh_tau_h_i = std::sinh(tau * (t[i + 1] - x));
    double sinh_tau_h_ip1 = std::sinh(tau * (x - t[i]));

    double t1 = (z[i] * sinh_tau_h_i + z[i + 1] * sinh_tau_h_ip1) / (tau * tau * sinh_tau_h);
    double t2 = (y[i] - z[i] / (tau * tau)) * (t[i + 1] - x) / h[i];
    double t3  = (y[i + 1] - z[i + 1] / (tau * tau)) * (x - t[i]) / h[i];

    return t1+t2+t3;
}

Eigen::VectorXd TensionSpline::eval_vector(Eigen::VectorXd t){
    int n = t.size()-1;
    if ((t.tail(n) - t.head(n)).minCoeff() < 0 )
        throw std::invalid_argument("t must be strictly increasing");

    Eigen::VectorXd y(t.size());
    int i=0;
    int j=0;

    while(j < t.size()){
        if ((i < this->t.size() - 2) && (t(j) > this->t(i+1)))  {
            i+=1;
        }
        y(j) = eval_spline(t(j), i);
        j +=1;
    }
    return y;
}