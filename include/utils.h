#pragma once

#include <iostream>
#include <stdexcept>
#include <variant>
#include <vector>

namespace utils {

struct Complex {
  double real;
  double imag;
};

struct ComplexVisitor {
  void operator()(const double& num);
  void operator()(const Complex& num);
};

double absval(const double val);
double exponentiation(const double b, const int n);
double squareroot(const double num);
double linear_equation_solver(const double a, const double b);
std::vector<std::variant<double, Complex>> quadratic_equation_solver(
    const double a, const double b, const double c);

std::ostream& operator<<(std::ostream& os, const Complex& num);

}  // namespace utils
