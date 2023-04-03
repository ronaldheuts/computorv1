#include "utils.h"

#include <iostream>
namespace utils {

double absval(double val) { return val < 0 ? -val : val; }

/// @brief exponentiate number
/// @param b for base
/// @param n for power
/// @return b raised to the power of n
double exponentiation(double b, int n) {
  if (n < 0) {
    throw(std::invalid_argument("can not raise to negative power"));
  } else if (!n) {
    return 1;
  } else if (!b) {
    return 0;
  }
  double result{b};

  while (--n) {
    result *= b;
  }
  return result;
}

/// @brief get the square root of a number
double squareroot(double num) {
  if (num < 0) {
    throw std::runtime_error("square root of negative number is not defined");
  }
  const double epsilon = 1e-6;
  double       guess = num;

  while (utils::absval(num - guess * guess) > epsilon) {
    guess = (guess + num / guess) * 0.5;
  }
  return guess;
}

/// @brief
/// @param a the coefficient of the variable raised to power of 1
/// @param b the constant
/// @return
double linear_equation_solver(double slope, double intercept) {
  if (slope > 1 || slope < -1) {
    return intercept / slope;
  }
  return (1 / slope) * intercept;
}

/// @brief get the roots of a quadratic equation
/// @param a the coefficient of the variable raised to power of 2
/// @param b the coefficient of the variable raised to power of 1
/// @param c the constant
/// @return the roots of the equation
std::optional<std::vector<double>> quadratic_equation_solver(double a, double b,
                                                             double c) {
  if (!a) {
    // throw(std::invalid_argument(
    //     "coefficient of the term raised to the power of 2 can not be 0"));
    return std::vector<double>{linear_equation_solver(b, c)};
  }
  // check input
  double discriminant{utils::exponentiation(b, 2) - (4 * a * c)};

  // [result] + 0.0 to prevent negative 0
  if (!discriminant) {
    return std::vector<double>{-b / (2 * a) + 0.0};
  } else if (discriminant > 0) {
    return std::vector<double>{
        (-b + utils::squareroot(discriminant)) / (2 * a) + 0.0,
        (-b - utils::squareroot(discriminant)) / (2 * a) + 0.0};
  } else {
    return std::nullopt;
  }
}

}  // namespace utils
