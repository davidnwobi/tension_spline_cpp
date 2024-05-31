/**
 * \class TensionSpline
 * Tension Spline Interpolation
 *
 * <p>This code implements tension spline interpolation based on the paper:
 * Tension Spline
 * Mahdi Boulila, Meherab Hossain, Cat Mai, Daniel Wright
 *
 * <p>The tension spline interpolation method aims to interpolate a smooth curve through a given set of data points
 * while incorporating tension parameters to control the smoothness of the curve and minimize oscillations.
 *
 * <p>The TensionSpline class provides methods for fitting the tension spline to the given data points and
 * evaluating the spline at specific points.
 *
 * <p>Usage:
 * <pre>
 * {@code
 * // Initialize a TensionSpline object with input data points and tension parameter:
 * Eigen::VectorXd t1{{-1, 1, 8, 14, 20, 25, 30, 32, 34, 36, 40, 43, 46, 47, 47.75, 48, 49, 52, 55, 59, 63, 70, 73,
 *         77, 79, 83, 84, 85, 86, 87, 88}};
 * Eigen::VectorXd y1{{0, 2, 5, 6.5, 8, 8.5, 9, 10, 12, 15, 21, 25, 28, 22, 16, 12, 10, 9.5, 9, 8.5, 7.5, 5, 4, 5.5,
 *         8, 13, 15, 13, 10, 5, 1}};
 * double tau = 10;
 * // Evaluate the tension spline at a set of points:   
 * Eigen::VectorXd t_spline = Eigen::VectorXd::LinSpaced(1000, -3, 89);
 * TensionSpline spline = TensionSpline(t1, y1, tau);
 * Eigen::VectorXd y_spline = spline.eval_vector(t_spline);
 * }
 * </pre>
 *
 * <p>References:
 * <ul>
 *     <li>Tension Spline</li>
 *     <li>Mahdi Boulila, Meherab Hossain, Cat Mai, Daniel Wright</li>
 *     <li><a href="https://catxmai.github.io/pdfs/Math212_ProjectReport.pdf">https://catxmai.github.io/pdfs/Math212_ProjectReport.pdf</a></li>
 * </ul>
 */

#include <iostream>
#include <Eigen/Dense>



class TensionSpline{
    public:
    /**
     * Constructor for the TensionSpline class.
     * @param t: Vector of input data points (x-values).
     * @param y: Vector of input data points (y-values).
     * @param tau: Tension parameter (positive value).
     * @return TensionSpline object.
    */
    TensionSpline(Eigen::VectorXd, Eigen::VectorXd, double);
    /**
     * Copy constructor for the TensionSpline class.
     * @param TensionSpline: TensionSpline object to be copied.
     * @return TensionSpline object.
    */
    TensionSpline(const TensionSpline&) = default;
    /**
     * Move constructor for the TensionSpline class.
     * @param TensionSpline: TensionSpline object to be moved.
     * @return TensionSpline object.
    */
    TensionSpline(TensionSpline&&) = default;
    TensionSpline operator=(const TensionSpline&) = delete;
    TensionSpline operator=(TensionSpline&&) = delete;
    /**
     * Evaluate the tension spline at a set of points.
     * @param tValues: Vector of points at which to evaluate the spline.
     * @return Vector of spline values at the given points.
     * @throws std::invalid_argument: If tValues is empty.
    */
    Eigen::VectorXd eval_vector(Eigen::VectorXd);
    
    /**
     * Check if the spline has been fitted.
     * @return True if the spline has been fitted, false otherwise.
    */
    inline bool fitted(){
       return is_fitted;
    }

    /**
     * Error codes for the TensionSpline class.
    */

    enum ERRORS{
        T_AND_Y_MUST_HAVE_SAME_LENGTH =1,/* T and Y must have the same length */
        TAU_MUST_BE_POSITIVE, /* Tau must be positive */
        T_MUST_BE_STRICTLY_INCREASING, /* T must be strictly increasing */
        T_AND_Y_MUST_HAVE_AT_LEAST_3_ELEMENTS, /* T and Y must have at least 3 elements */
        TAU_IS_TOO_SMALL_OR_TOO_LARGE, /* Tau is too small or too large */
        SPLINE_HAS_NOT_BEEN_FITTED /* Spline has not been fitted */
    };

    private:
        void error_handler(ERRORS);
        Eigen::VectorXd t;
        Eigen::VectorXd y;
        double tau;
        Eigen::VectorXd z;
        Eigen::VectorXd h;
        double eval_spline(double, int);
        void fit();
        bool is_fitted = false;
};