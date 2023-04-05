#include "utils.h"

#include <iostream>
namespace utils {

double absval(const double val) { return val < 0 ? -val : val; }

/// @brief exponentiate number
/// @param b for base
/// @param n for power
/// @return b raised to the power of n
double exponentiation(const double b, const int n) {
  if (n < 0) {
    throw(std::invalid_argument("can not raise to negative power"));
  } else if (!n) {
    return 1;
  } else if (!b) {
    return 0;
  }
  double result{b};

  for (int i = 1; i < n; ++i) {
    result *= b;
  }
  return result;
}

/// @brief get the square root of a number
double squareroot(const double num) {
  if (num <= 0) {
    throw std::invalid_argument(
        "square root of negative number is not defined");
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
double linear_equation_solver(const double slope, const double intercept) {
  if (!slope) {
    throw std::invalid_argument("'slope' can not be 0");
  } else if (!intercept) {
    return 0;
  } else if (slope > 1 || slope < -1) {
    return intercept / slope;
  }
  return (1 / slope) * intercept;
}

/// @brief get the roots of a quadratic equation
/// @param a the coefficient of the variable raised to power of 2
/// @param b the coefficient of the variable raised to power of 1
/// @param c the constant
/// @return the roots of the equation
std::vector<double> quadratic_equation_solver(const double a, const double b,
                                              const double c) {
  if (!a) {
    throw(std::invalid_argument("'a' can not be 0"));
  }
  double discriminant{utils::exponentiation(b, 2) - (4 * a * c)};

  if (!discriminant) {
    return std::vector<double>{-b / (2 * a) + 0.0};
  } else if (discriminant > 0) {
    return std::vector<double>{
        (-b + utils::squareroot(discriminant)) / (2 * a) + 0.0,
        (-b - utils::squareroot(discriminant)) / (2 * a) + 0.0};
  }
  throw std::invalid_argument("complex solutions are not supported");
}

}  // namespace utils
