#include "solver.hpp"
#include <gtest/gtest.h>
#include <random>

static double residual(const Eigen::MatrixXd& A,
                       const Eigen::VectorXd& b,
                       const Eigen::VectorXd& x)
{
    return (A*x - b).norm();
}

TEST(Gauss, Known) {
    Eigen::MatrixXd A(3,3);
    A << 2, -1,  1,
         1,  0, -1,
         3,  1,  1;
    Eigen::VectorXd b(3);
    b << 3, -2, 8;
    auto x = solveGauss(A,b);
    EXPECT_NEAR(x(0), 1.0, 1e-9);
    EXPECT_NEAR(x(1), 2.0, 1e-9);
    EXPECT_NEAR(x(2), 3.0, 1e-9);
}

TEST(Gauss, Random) {
    std::mt19937_64 gen(123);
    std::uniform_real_distribution<double> dist(-10,10);
    for (int seed = 0; seed < 10; ++seed) {
        gen.seed(seed);
        int n = 5 + seed%5;
        Eigen::MatrixXd A(n,n);
        Eigen::VectorXd b(n);
        for (int i = 0; i < n; ++i) {
            double sum = 0;
            for (int j = 0; j < n; ++j) {
                if (i == j) continue;
                A(i,j) = dist(gen);
                sum += std::abs(A(i,j));
            }
            A(i,i) = sum + 1;
            b(i)   = dist(gen);
        }
        auto x = solveGauss(A,b);
        //дебааааг
        EXPECT_LT(residual(A,b,x), 1e-8) << "seed="<<seed;
    }
}