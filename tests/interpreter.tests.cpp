#include "interpreter.h"

#include <gtest/gtest.h>

#include <cmath>

TEST(parser, goodMonomial) {
  Parser par{"4 * X^2 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(std::get<double>(interp.getSolutions().at(0)), 0);
}

TEST(parser, goodBinomial) {
  Parser par{"4 * X^2 + 8 * X^1 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
}

TEST(parser, goodTrinomial) {
  constexpr double expectedSolutionOne{-1};
  constexpr double expectedSolutionTwo{4};

  Parser par{"1 * X^2 - 3 * X^1 - 4 * X^0 = 0"};
  par.parse();
  Interpreter interp{par.getTree()};
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 2);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(1)) -
                       expectedSolutionOne) < std::pow(10, -8));
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolutionTwo) < std::pow(10, -8));
}

TEST(parser, linearEquation) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"42 * X^1 - 10 * X^0 = 0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -8));
}

TEST(parser, transposeOneEntity) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"42 * X^1 - 20 * X^0 = -10 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -8));
}

TEST(parser, transposeTwoEntities) {
  constexpr double expectedSolution{-0.23809523809523808};

  Parser par{"84 * X^1 - 20 * X^0 = 42 * X^1 - 10 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 1);
  EXPECT_TRUE(std::abs(std::get<double>(interp.getSolutions().at(0)) -
                       expectedSolution) < std::pow(10, -8));
}

TEST(parser, identityEquation) {
  Parser par{"42 * X^0 = 42 * X^0"};
  par.parse();
  Interpreter interp(par.getTree());
  interp.evaluate();

  EXPECT_EQ(interp.getSolutions().size(), 0);
}

TEST(parser, missingEquation) {
  Parser par{"1 * X^2"};
  par.parse();
  Interpreter interp(par.getTree());

  EXPECT_THROW(par.parse(), grammarError);
}
