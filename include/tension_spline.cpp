#include <Eigen/Dense>
#include "tension_spline.h"
#include <cmath>
#include <stdexcept>
#include <limits>

TensionSpline::TensionSpline(Eigen::VectorXd t, Eigen::VectorXd y, double tau) : t(t), y(y), tau(tau) {fit();};


void TensionSpline::fit(){
    /**
     * Fit the tension spline to the input data.
     * @throws std::invalid_argument: If the input data does not meet the requirements.
    */
    int n = t.size()-1;

    Eigen::VectorXd a_ = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd b_ = Eigen::VectorXd::Zero(n);
    Eigen::VectorXd g_ = Eigen::VectorXd::Zero(n);


    if (t.size() != y.size())
        error_handler(T_AND_Y_MUST_HAVE_SAME_LENGTH);
    if (tau <= 0)
        error_handler(TAU_MUST_BE_POSITIVE);
    if ((t.tail(n) - t.head(n)).minCoeff() < 0 )
        error_handler(T_MUST_BE_STRICTLY_INCREASING);
    if (t.size() < 3)
        error_handler(T_AND_Y_MUST_HAVE_AT_LEAST_3_ELEMENTS);

    h = t.tail(n) - t.head(n) ;
    g_ = std::pow(tau,2) * (y.tail(n) - y.head(n)).array()/h.array();
    a_ = 1 / h.array() - tau / (tau * h).array().sinh();
    b_ = tau * (tau * h).array().cosh() / (tau * h).array().sinh() - 1 / h.array();

    if (g_.array().isNaN().any() || a_.array().isNaN().any() || b_.array().isNaN().any())
        error_handler(TAU_IS_TOO_SMALL_OR_TOO_LARGE);

    Eigen::MatrixXd A = Eigen::MatrixXd::Zero(n + 1, n + 1);
    A(0,0) = 1;
    A(n,n) = 1;  
    for(int row = 1; row < n; row++ ){
        A(row, row-1) = a_(row-1);
        A(row, row) = b_(row-1) + b_(row);
        A(row, row+1) = a_(row);
 
    }
    Eigen::VectorXd Y =  Eigen::VectorXd::Zero(n+1);
    Y.segment(1,n-1) = g_.tail(n-1) - g_.head(n-1);
    z = A.inverse()*(Y);
    is_fitted = true;
}


double TensionSpline::eval_spline(double x, int i){
    /**
     * Evaluate the tension spline at a point.
     * @param x: Point at which to evaluate the spline.
     * @param i: Index of the interval in which x lies.
     * @return Value of the spline at the given point.
     * @throws std::invalid_argument: If the result is NaN.
    */
    double sinh_tau_h = std::sinh(tau * h[i]);
    double sinh_tau_h_i = std::sinh(tau * (t[i + 1] - x));
    double sinh_tau_h_ip1 = std::sinh(tau * (x - t[i]));

    double t1 = (z[i] * sinh_tau_h_i + z[i + 1] * sinh_tau_h_ip1) / (tau * tau * sinh_tau_h);
    double t2 = (y[i] - z[i] / (tau * tau)) * (t[i + 1] - x) / h[i];
    double t3  = (y[i + 1] - z[i + 1] / (tau * tau)) * (x - t[i]) / h[i];


    if (std::isnan(t1) || std::isnan(t2) || std::isnan(t3))
        error_handler(TAU_IS_TOO_SMALL_OR_TOO_LARGE);

    return t1+t2+t3;
}

Eigen::VectorXd TensionSpline::eval_vector(Eigen::VectorXd t){
    int n = t.size()-1;

    if ((t.tail(n) - t.head(n)).minCoeff() < 0 )
        error_handler(T_MUST_BE_STRICTLY_INCREASING);
    if (!is_fitted)
        error_handler(SPLINE_HAS_NOT_BEEN_FITTED);

    Eigen::VectorXd y(t.size());
    int i=0;
    int j=0;

    while(j < t.size()){
        if ((i < this->t.size() - 2) && (t(j) > this->t(i+1)))  {
            i+=1;
        }
        try{
            y(j) = eval_spline(t(j), i);
            j +=1;
        }
        catch(std::invalid_argument& e){
            error_handler(TAU_IS_TOO_SMALL_OR_TOO_LARGE);
        }
    }
    return y;
}

void TensionSpline::error_handler(ERRORS err){
    /**
     * Error handler for the TensionSpline class.
     * @param err: Error code.
     * @throws std::invalid_argument: If the error code is not recognized.
    */
    switch (err)
    {
    case T_AND_Y_MUST_HAVE_SAME_LENGTH:
        throw std::invalid_argument("T_AND_Y_MUST_HAVE_SAME_LENGTH");
        break;
    case TAU_MUST_BE_POSITIVE:
        throw std::invalid_argument("TAU_MUST_BE_POSITIVE");
        break;
    case T_MUST_BE_STRICTLY_INCREASING:
        throw std::invalid_argument("T_MUST_BE_STRICTLY_INCREASING");
        break;
    case T_AND_Y_MUST_HAVE_AT_LEAST_3_ELEMENTS:
        throw std::invalid_argument("T_AND_Y_MUST_HAVE_AT_LEAST_3_ELEMENTS");
        break;
    case TAU_IS_TOO_SMALL_OR_TOO_LARGE:
        throw std::invalid_argument("TAU_IS_TOO_SMALL_OR_TOO_LARGE");
        break;
    case SPLINE_HAS_NOT_BEEN_FITTED:
        throw std::invalid_argument("SPLINE_HAS_NOT_BEEN_FITTED");
        break;
    default:
        break;
    }
}

