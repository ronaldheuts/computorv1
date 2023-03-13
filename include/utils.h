#pragma once

#include <optional>
#include <stdexcept>
#include <vector>

namespace utils {
double exponentiation(double n, int e);
double squareroot(double num);
std::optional<std::vector<double>> quadratic_formula(double a, double b,
                                                     double c);
}  // namespace utils
