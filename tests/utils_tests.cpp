#include <gtest/gtest.h>

#include <cmath>

#include "utils.h"

/* exponentiation */

TEST(Exponentiation, baseNegativeExponentZero) {
  EXPECT_EQ(utils::exponentiation(-1, 0), std::pow(-1, 0));
}

TEST(Exponentiation, baseNegativeExponentOne) {
  EXPECT_EQ(utils::exponentiation(-1, 1), std::pow(-1, 1));
}

TEST(Exponentiation, baseNegativeExponentTwo) {
  EXPECT_EQ(utils::exponentiation(-1, 2), std::pow(-1, 2));
}

TEST(Exponentiation, baseZeroExponentZero) {
  EXPECT_EQ(utils::exponentiation(0, 0), std::pow(0, 0));
}

TEST(Exponentiation, baseZeroExponentOne) {
  EXPECT_EQ(utils::exponentiation(0, 1), std::pow(0, 1));
}

TEST(Exponentiation, baseZeroExponentNegative) {
  EXPECT_THROW(utils::exponentiation(0, -1), std::invalid_argument);
}

TEST(Exponentiation, baseTwoExponentZero) {
  EXPECT_EQ(utils::exponentiation(2, 0), std::pow(2, 0));
}

TEST(Exponentiation, baseTwoExponentOne) {
  EXPECT_EQ(utils::exponentiation(2, 1), std::pow(2, 1));
}

TEST(Exponentiation, baseTwoExponentTwo) {
  EXPECT_EQ(utils::exponentiation(2, 2), std::pow(2, 2));
}

TEST(Exponentiation, baseTwoExponentThree) {
  EXPECT_EQ(utils::exponentiation(2, 3), std::pow(2, 3));
}

TEST(Exponentiation, baseTwoExponentNegative) {
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
  EXPECT_EQ(utils::squareroot(9), std::sqrt(9));
}

TEST(squareroot, bigPerfectSquare) {
  EXPECT_EQ(utils::squareroot(1048576), std::sqrt(1048576));
}

// https://stackoverflow.com/questions/47906197/how-to-compare-double-values-limiting-the-decimal-places
TEST(squareroot, smallNonPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(2)) - std::abs(std::sqrt(2))) <
              std::pow(10, -8));
}

TEST(squareroot, bigNonPerfectSquare) {
  EXPECT_TRUE((std::abs(utils::squareroot(218792)) -
               std::abs(std::sqrt(218792))) < std::pow(10, -8));
}

/* quadratic formula */

TEST(quadratic_equation_solver, zeroZeroZero) {
  EXPECT_THROW(utils::quadratic_equation_solver(0, 0, 0),
               std::invalid_argument);
}

// discriminant is 0
TEST(quadratic_equation_solver, oneZeroZero) {
  auto result = utils::quadratic_equation_solver(1, 0, 0);
  EXPECT_TRUE(result->size() == 1);
  EXPECT_EQ(result->at(0), 0);
}

// discriminant is positive
TEST(quadratic_equation_solver, oneOneZero) {
  auto result = utils::quadratic_equation_solver(1, 1, 0);
  EXPECT_TRUE(result->size() == 2);
  EXPECT_EQ(result->at(0), 0);
  EXPECT_EQ(result->at(1), -1);
}

// from subject: 4 * X^0 + 4 * X^1 - 9.3 * X^2 = 0
// TEST(quadratic_equation_solver, oneOneZero) {
//   auto result = utils::quadratic_equation_solver(9.3, 4, 4);
//   EXPECT_TRUE(result->size() == 2);
//   EXPECT_EQ(result->at(0), 0);
//   EXPECT_EQ(result->at(1), -1);
// }
