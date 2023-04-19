#include "parser.h"

#include <gtest/gtest.h>

TEST(parser, noTokens) {
  Parser par{};
  EXPECT_THROW(par.parse(), std::invalid_argument);
}

TEST(parser, missingAsterisk) {
  Parser par{"1"};
  EXPECT_THROW(par.parse(), std::invalid_argument);
}

TEST(parser, missingVariable) {
  Parser par{"1 *"};
  EXPECT_THROW(par.parse(), std::invalid_argument);
}

TEST(parser, missingCaret) {
  Parser par{"1 * X"};
  EXPECT_THROW(par.parse(), std::invalid_argument);
}

TEST(parser, missingExponent) {
  Parser par{"1 * X^"};
  EXPECT_THROW(par.parse(), std::invalid_argument);
}
