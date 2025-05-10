#include "solver.hpp"
#include <iostream>

int main(int argc, char* argv[]) {
    if (argc != 3) {
        std::cerr << "Usage: " << argv[0]
                  << " input.csv output.csv\n";
        return 1;
    }
    Eigen::MatrixXd A;
    Eigen::VectorXd b;
    if (!readCSV(argv[1], A, b)) {
        std::cerr << "Error reading " << argv[1] << "\n";
        return 1;
    }
    Eigen::VectorXd x;
    try {
        x = solveGauss(A, b);
    } catch (std::exception& e) {
        std::cerr << "Solve failed: " << e.what() << "\n";
        return 1;
    }
    if (!writeCSV(argv[2], x)) {
        std::cerr << "Error writing " << argv[2] << "\n";
        return 1;
    }
    return 0;
}