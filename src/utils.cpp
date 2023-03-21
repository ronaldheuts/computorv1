#include "utils.h"

namespace utils {

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
/// @param num for number
/// @return square root of num
double squareroot(double num) {
  if (num <= 0) {
    throw(std::invalid_argument("can not get the root of 0 or less"));
  }
  constexpr int max_iter = 100;
  double        result = num;

  for (int i = 0; i < max_iter; ++i) {
    result = 0.5 * (result + num / result);
  }
  return result;
}

double linear_equation_solver(double a, double b) {
  if (a > 1 || a < -1) {
    return b / a;
  } else if ((a > 0 && a < 1) || (a < 0 && a > -1)) {
    return (1 / a) * b;
  }
}

/// @brief get the roots of a quadratic equation
/// @param a the coefficient of the variable raised to power of 2
/// @param b the coefficient of the variable raised to power of 1
/// @param c the constant
/// @return the roots of the equation
std::optional<std::vector<double>> quadratic_equation_solver(double a, double b,
                                                             double c) {
  if (!a) {
    throw(std::invalid_argument("'a' can not be 0"));
  }
  // check input
  double discriminant{utils::exponentiation(b, 2) - (4 * a * c)};

  if (!discriminant) {
    return std::vector<double>{-b / (2 * a)};
  } else if (discriminant > 0) {
    return std::vector<double>{
        (-b + utils::squareroot(discriminant)) / (2 * a),
        (-b - utils::squareroot(discriminant)) / (2 * a)};
  } else {
    return std::nullopt;
  }
}

}  // namespace utils
