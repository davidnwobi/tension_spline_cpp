#include <gtest/gtest.h>
#include <Eigen/Dense>
#include <stdexcept>
#include <fstream>
#include <filesystem>
#include "tension_spline.h"

void saveData(std::string fileName, Eigen::MatrixXd  matrix)
{
    const static Eigen::IOFormat CSVFormat(Eigen::FullPrecision, Eigen::DontAlignCols, ", ", "\n");
 
    std::ofstream file(fileName);
    if (file.is_open())
    {
        file << matrix.format(CSVFormat);
        file.close();
    }
}

Eigen::MatrixXd openData(std::string fileToOpen)
{
 
    // the inspiration for creating this function was drawn from here (I did NOT copy and paste the code)
    // https://stackoverflow.com/questions/34247057/how-to-read-csv-file-and-assign-to-eigen-matrix
     
    // the input is the file: "fileToOpen.csv":
    // a,b,c
    // d,e,f
    // This function converts input file data into the Eigen matrix format
 
    // the matrix entries are stored in this variable row-wise. For example if we have the matrix:
    // M=[a b c 
    //    d e f]
    // the entries are stored as matrixEntries=[a,b,c,d,e,f], that is the variable "matrixEntries" is a row vector
    // later on, this vector is mapped into the Eigen matrix format

    std::vector<double> matrixEntries;
    std::ifstream matrixDataFile(fileToOpen);
    if (!matrixDataFile.is_open())
    {
        std::cerr << "Error reading from file: " << fileToOpen << std::endl;
        exit(1);
    }
 
    std::string matrixRowString;
    std::string matrixEntry;
    int matrixRowNumber = 0;
 
    // Read the file line by line and store all entries in the vector variable matrixEntries
    while (getline(matrixDataFile, matrixRowString))
    {
        std::stringstream matrixRowStringStream(matrixRowString);
        while (getline(matrixRowStringStream, matrixEntry, ','))
        {
            matrixEntries.push_back(stod(matrixEntry));
        }
        matrixRowNumber++;
    }
    //Convert the vector variable matrixEntries into the Eigen matrix format

    return Eigen::Map<Eigen::Matrix<double, Eigen::Dynamic, Eigen::Dynamic, Eigen::RowMajor>>(
                matrixEntries.data(), 
                matrixRowNumber, 
                matrixEntries.size() / matrixRowNumber
);

 
}

TEST(TensionSpline, Constructor) {
    Eigen::VectorXd t1{{1,2,3,4,5}};
    Eigen::VectorXd y1 = Eigen::VectorXd::Random(5);
    double tau = 1;
    TensionSpline spline = TensionSpline(t1, y1, tau);
    EXPECT_EQ(spline.fitted(), true);
}

TEST(TensionSpline, ConstructorFail_Tau) {
    Eigen::VectorXd t1{{1,2,3,4,5}};
    Eigen::VectorXd y1 = Eigen::VectorXd::Random(5);
    double tau = 1000;
    EXPECT_THROW({TensionSpline spline = TensionSpline(t1, y1, tau);}, std::invalid_argument);
}

TEST(TensionSpline, ConstructorFail_Incompatible_Vectors) {
    Eigen::VectorXd t1{{1,2,3,4}};
    Eigen::VectorXd y1 = Eigen::VectorXd::Random(5);
    double tau = 1;
    EXPECT_THROW({TensionSpline spline = TensionSpline(t1, y1, tau);}, std::invalid_argument);
}

TEST(TensionSpline, ConstructorFail_T_not_increasing) {
    Eigen::VectorXd t1{{1,2,3,4,1}};
    Eigen::VectorXd y1 = Eigen::VectorXd::Random(5);
    double tau = 1;
    EXPECT_THROW({TensionSpline spline = TensionSpline(t1, y1, tau);}, std::invalid_argument);
}

TEST(TensionSpline, ConstructorFail_T_less_3_elements) {
    Eigen::VectorXd t1{{1,2}};
    Eigen::VectorXd y1 = Eigen::VectorXd::Random(2);
    double tau = 1;
    EXPECT_THROW({TensionSpline spline = TensionSpline(t1, y1, tau);}, std::invalid_argument);
}

TEST(TensionSpline, EvalVector_Parabola_from_Python) {
    Eigen::MatrixXd matrix_test;
    std::vector<std::string> tau_vec = {"0.01", "0.1", "1", "10"};
 
    matrix_test = openData("../tests/test_data/trial_data_1.csv");
    Eigen::VectorXd t1 = matrix_test.col(0);
    Eigen::VectorXd y1 = matrix_test.col(1);

    for (std::string tau : tau_vec){
        double tau_val = std::stod(tau);
        TensionSpline spline = TensionSpline(t1, y1, tau_val);
        Eigen::MatrixXd test_data = openData("../tests/test_data/trial_data_1_tau_" + tau + ".csv");
        Eigen::VectorXd t_spline = test_data.col(0);
        Eigen::VectorXd y_spline = spline.eval_vector(t_spline);
        for (int i = 0; i < y_spline.size(); i++)
        {
            EXPECT_NEAR(y_spline(i), test_data(i, 1), 1e-6);
        }
    }

}

TEST(TensionSpline, EvalVector_RandomDistribution_from_Python) {
    Eigen::MatrixXd matrix_test;
    std::vector<std::string> tau_vec = {"0.01", "0.1", "1", "10"};
 
    matrix_test = openData("../tests/test_data/trial_data_2.csv");
    Eigen::VectorXd t1 = matrix_test.col(0);
    Eigen::VectorXd y1 = matrix_test.col(1);

    for (std::string tau : tau_vec){
        double tau_val = std::stod(tau);
        TensionSpline spline = TensionSpline(t1, y1, tau_val);
        Eigen::MatrixXd test_data = openData("../tests/test_data/trial_data_2_tau_" + tau + ".csv");
        Eigen::VectorXd t_spline = test_data.col(0);
        Eigen::VectorXd y_spline = spline.eval_vector(t_spline);
        for (int i = 0; i < y_spline.size(); i++)
        {
            EXPECT_NEAR(y_spline(i), test_data(i, 1), 1e-6);
        }
    }

}