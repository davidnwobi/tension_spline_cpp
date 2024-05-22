#include <iostream>
#include <Eigen/Dense>


class TensionSpline{
    public:
    TensionSpline(Eigen::VectorXd, Eigen::VectorXd, double);
    TensionSpline(const TensionSpline&) = default;
    TensionSpline(TensionSpline&&) = default;
    TensionSpline operator=(const TensionSpline&) = delete;
    TensionSpline operator=(TensionSpline&&) = delete;
    void fit();
    double eval_spline(double, int);
    Eigen::VectorXd eval_vector(Eigen::VectorXd);

    private:
        Eigen::VectorXd t;
        Eigen::VectorXd y;
        double tau;
        Eigen::VectorXd z;
        Eigen::VectorXd h;

};