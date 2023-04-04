#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

namespace utils {
double absval(const double val);
double exponentiation(const double n, const int e);
double squareroot(const double num);
double linear_equation_solver(const double a, const double b);
std::optional<std::vector<double>> quadratic_equation_solver(const double a,
                                                             const double b,
                                                             const double c);
}  // namespace utils
