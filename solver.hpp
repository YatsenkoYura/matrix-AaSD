#pragma once
#include <Eigen/Dense>
#include <string>

bool readCSV(const std::string& fname,
             Eigen::MatrixXd& A,
             Eigen::VectorXd& b);

bool writeCSV(const std::string& fname,
              const Eigen::VectorXd& x);

Eigen::VectorXd solveGauss(const Eigen::MatrixXd& A,
                           const Eigen::VectorXd& b);