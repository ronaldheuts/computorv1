#include "utils.h"

#include <gtest/gtest.h>

#include <cmath>

/* absolute value */

TEST(absval, zero) { EXPECT_EQ(utils::absval(0), 0); }

TEST(absval, negativeNum) { EXPECT_EQ(utils::absval(-42), 42); }

TEST(absval, negativeFracNum) {
  EXPECT_EQ(utils::absval(-42.42424242), 42.42424242);
}

TEST(absval, positiveNum) { EXPECT_EQ(utils::absval(1337), 1337); }

TEST(absval, positiveFracNum) {
  EXPECT_EQ(utils::absval(1337.13371337), 1337.13371337);
}

/* exponentiation */

TEST(exponentiation, baseNegativeExponentZero) {
  EXPECT_EQ(utils::exponentiation(-1, 0), std::pow(-1, 0));
}

TEST(exponentiation, baseNegativeExponentOne) {
  EXPECT_EQ(utils::exponentiation(-1, 1), std::pow(-1, 1));
}

TEST(exponentiation, baseNegativeExponentTwo) {
  EXPECT_EQ(utils::exponentiation(-1, 2), std::pow(-1, 2));
}

TEST(exponentiation, baseZeroExponentZero) {
  EXPECT_EQ(utils::exponentiation(0, 0), std::pow(0, 0));
}

TEST(exponentiation, baseZeroExponentOne) {
  EXPECT_EQ(utils::exponentiation(0, 1), std::pow(0, 1));
}

TEST(exponentiation, baseZeroExponentNegative) {
  EXPECT_THROW(utils::exponentiation(0, -1), std::invalid_argument);
}

TEST(exponentiation, baseTwoExponentZero) {
  EXPECT_EQ(utils::exponentiation(2, 0), std::pow(2, 0));
}

TEST(exponentiation, baseTwoExponentOne) {
  EXPECT_EQ(utils::exponentiation(2, 1), std::pow(2, 1));
}

TEST(exponentiation, baseTwoExponentTwo) {
  EXPECT_EQ(utils::exponentiation(2, 2), std::pow(2, 2));
}

TEST(exponentiation, baseTwoExponentThree) {
  EXPECT_EQ(utils::exponentiation(2, 3), std::pow(2, 3));
}

TEST(exponentiation, baseTwoExponentNegative) {
  EXPECT_THROW(utils::exponentiation(2, -1), std::invalid_argument);
}

/* square root */

TEST(squareroot, Zero) {
  EXPECT_THROW(utils::squareroot(0), std::invalid_argument);
}

TEST(squareroot, Negative) {
  EXPECT_THROW(utils::squareroot(-1), std::invalid_argument);
}

TEST(squareroot, smallPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(9) - std::sqrt(9))) <
              std::pow(10, -8));
}

TEST(squareroot, bigPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(1048576) - std::sqrt(1048576))) <
              std::pow(10, -8));
}

// https://stackoverflow.com/questions/47906197/how-to-compare-double-values-limiting-the-decimal-places
TEST(squareroot, smallNonPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(2) - std::sqrt(2))) <
              std::pow(10, -8));
}

TEST(squareroot, bigNonPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(218792) - std::sqrt(218792))) <
              std::pow(10, -8));
}

/* quadratic equation solver */

TEST(quadratic_equation_solver, zeroZeroZero) {
  EXPECT_THROW(utils::quadratic_equation_solver(0, 0, 0),
               std::invalid_argument);
}

/* discriminant is 0 */
TEST(quadratic_equation_solver, oneZeroZero) {
  auto actual = utils::quadratic_equation_solver(1, 0, 0);
  EXPECT_EQ(actual.size(), 1);
  EXPECT_EQ(std::get<double>(actual.at(0)), 0);
}

/* discriminant is positive */
TEST(quadratic_equation_solver, doubleSolution) {
  auto actual = utils::quadratic_equation_solver(1, 1, 0);
  EXPECT_EQ(actual.size(), 2);
  EXPECT_EQ(std::get<double>(actual.at(0)), 0);
  EXPECT_EQ(std::get<double>(actual.at(1)), -1);
}

TEST(quadratic_equation_solver, singleSolution) {
  auto actual = utils::quadratic_equation_solver(1, 4, 4);
  EXPECT_EQ(actual.size(), 1);
  EXPECT_EQ(std::get<double>(actual.at(0)), -2);
}

/* discriminant is negative ergo complex solution */
TEST(quadratic_equation_solver, complexSolution) {
  auto actual = utils::quadratic_equation_solver(3, 3, 4);
  EXPECT_EQ(actual.size(), 2);
  EXPECT_EQ(std::get<utils::Complex>(actual.at(0)).real, -0.5);
  EXPECT_EQ(std::get<utils::Complex>(actual.at(0)).imag, -1.0408330019191296);
  EXPECT_EQ(std::get<utils::Complex>(actual.at(1)).real, -0.5);
  EXPECT_EQ(std::get<utils::Complex>(actual.at(1)).imag, 1.0408330019191296);
}

/* linear equation solver */

TEST(linear_equation_solver, zeroZero) {
  EXPECT_THROW(utils::linear_equation_solver(0, 0), std::invalid_argument);
}

TEST(linear_equation_solver, zeroOne) {
  EXPECT_THROW(utils::linear_equation_solver(0, 1), std::invalid_argument);
}

TEST(linear_equation_solver, oneZero) {
  EXPECT_EQ(utils::linear_equation_solver(1, 0), 0);
}

TEST(linear_equation_solver, twoZero) {
  EXPECT_EQ(utils::linear_equation_solver(2, 0), 0);
}

TEST(linear_equation_solver, bigNumZero) {
  EXPECT_EQ(utils::linear_equation_solver(223901283, 0), 0);
}

TEST(linear_equation_solver, oneOne) {
  EXPECT_EQ(utils::linear_equation_solver(1, 1), 1);
}

TEST(linear_equation_solver, twoOne) {
  EXPECT_EQ(utils::linear_equation_solver(2, 1), 0.5);
}

TEST(linear_equation_solver, twoTwo) {
  EXPECT_EQ(utils::linear_equation_solver(2, 2), 1);
}

TEST(linear_equation_solver, bigNumBigNum) {
  EXPECT_EQ(utils::linear_equation_solver(223901283, 1241283721832),
            5543.8883833104255);
}
