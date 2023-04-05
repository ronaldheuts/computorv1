#pragma once

#include <stdexcept>
#include <vector>

namespace utils {
double              absval(const double val);
double              exponentiation(const double b, const int n);
double              squareroot(const double num);
double              linear_equation_solver(const double a, const double b);
std::vector<double> quadratic_equation_solver(const double a, const double b,
                                              const double c);
}  // namespace utils
