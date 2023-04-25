#include "interpreter.h"

#include <gtest/gtest.h>

#include <cmath>

TEST(interpreter, goodMonomial) {
  Parser par{"4 * X^2 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(std::get<double>(interp.getSolutions().at(0)), 0);
}

TEST(interpreter, goodBinomial) {
  Parser par{"4 * X^2 + 8 * X^1 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
}

TEST(interpreter, goodTrinomial) {
  constexpr double expectedSolutionOne{-1};
  constexpr double expectedSolutionTwo{4};

  Parser par{"1 * X^2 - 3 * X^1 - 4 * X^0 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 2);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(1)) -
                       expectedSolutionOne) < std::pow(10, -6));
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolutionTwo) < std::pow(10, -6));
}

TEST(interpreter, codamExampleOne) {
  constexpr double expectedSolutionOne{-0.475131};
  constexpr double expectedSolutionTwo{0.905239};

  Parser par{"5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 2);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolutionOne) <= std::pow(10, -6));
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(1)) -
                       expectedSolutionTwo) <= std::pow(10, -6));
}

TEST(interpreter, codamExampleTwo) {
  constexpr double expectedSolutionOne{0.25};

  Parser par{"5 * X^0 + 4 * X^1 = 4 * X^0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_EQ(std::get<double>(interp.getSolutions().at(0)), 0.25);
}

TEST(interpreter, codamExampleThree) {
  Parser par{"8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"};
  par.parse();
  Interpreter interp{par.getTree()};

  EXPECT_THROW(interp.evaluate(), std::invalid_argument);
}

TEST(interpreter, linearEquation) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"42 * X^1 - 10 * X^0 = 0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -6));
}

TEST(interpreter, transposeOneEntity) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"42 * X^1 - 20 * X^0 = -10 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -6));
}

TEST(interpreter, transposeTwoEntities) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"84 * X^1 - 20 * X^0 = 42 * X^1 - 10 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -6));
}

TEST(interpreter, identityEquation) {
  Parser par{"42 * X^0 = 42 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 0);
}

TEST(interpreter, missingEquation) {
  Parser par{"1 * X^2"};
  par.parse();
  Interpreter interp(par.getTree());

  EXPECT_THROW(par.parse(), grammarError);
}
