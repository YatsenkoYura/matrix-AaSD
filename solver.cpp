#include "solver.hpp"
#include "lazycsv.hpp"

#include <fstream>
#include <sstream>
#include <stdexcept>
#include <vector>
#include <cmath>

bool readCSV(const std::string& fname,
             Eigen::MatrixXd& A,
             Eigen::VectorXd& b)
{
    std::ifstream in(fname);
    if (!in) return false;
    std::vector<std::vector<double>> rows;
    std::string line;
    while (std::getline(in, line)) {
        if (line.empty()) continue;
        std::stringstream ss(line);
        std::string cell;
        std::vector<double> tmp;
        while (std::getline(ss, cell, ',')) {
            try {
                tmp.push_back(std::stod(cell));
            } catch (...) {
                return false;
            }
        }
        if (tmp.size() < 2) return false;
        rows.push_back(tmp);
    }
    size_t n = rows.size();
    if (n == 0 || rows[0].size() != n+1) return false;
    A.resize(n,n);
    b.resize(n);
    for (size_t i = 0; i < n; ++i) {
        for (size_t j = 0; j < n; ++j)
            A(i,j) = rows[i][j];
        b(i) = rows[i][n];
    }
    return true;
}

bool writeCSV(const std::string& fname,
              const Eigen::VectorXd& x)
{
    std::ofstream out(fname);
    if (!out) return false;
    for (int i = 0; i < x.size(); ++i)
        out << x(i) << "\n";
    return true;
}

Eigen::VectorXd solveGauss(const Eigen::MatrixXd& A_,
                           const Eigen::VectorXd& b_)
{
    using Index = Eigen::Index;
    Index n = A_.rows();
    if (A_.cols() != n || b_.size() != n)
        throw std::invalid_argument("Dimension mismatch");

    Eigen::MatrixXd Ab(n, n+1);
    Ab.block(0,0,n,n) = A_;
    Ab.col(n) = b_;

    for (Index k = 0; k < n; ++k) {
        Index piv = k;
        double maxv = std::abs(Ab(k,k));
        for (Index i = k+1; i < n; ++i) {
            double v = std::abs(Ab(i,k));
            if (v > maxv) { maxv = v; piv = i; }
        }
        if (maxv < 1e-14)
            throw std::runtime_error("Matrix is singular");

        Ab.row(k).swap(Ab.row(piv));
        for (Index i = k+1; i < n; ++i) {
            double f = Ab(i,k) / Ab(k,k);
            Ab.row(i).segment(k, n+1-k)
               -= f * Ab.row(k).segment(k, n+1-k);
        }
    }

    Eigen::VectorXd x(n);
    for (Index i = n-1; i >= 0; --i) {
        double sum = Ab(i,n);
        for (Index j = i+1; j < n; ++j)
            sum -= Ab(i,j) * x(j);
        x(i) = sum / Ab(i,i);
    }
    return x;
}