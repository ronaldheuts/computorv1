#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

namespace utils {
double exponentiation(double n, int e);
double squareroot(double num);
double linear_equation_solver(double a, double b);
std::optional<std::vector<double>> quadratic_equation_solver(double a, double b,
                                                             double c);
}  // namespace utils
