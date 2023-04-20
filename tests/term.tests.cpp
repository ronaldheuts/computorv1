#include "term.h"

#include <gtest/gtest.h>

/* constructors */

TEST(term, defaultConstructor) {
  Term actual{};

  EXPECT_EQ(actual.getCoe(), 0);
  EXPECT_EQ(actual.getVar(), 0);
  EXPECT_EQ(actual.getExp(), 0);
}

TEST(term, coefConstructor) {
  Term actual{42};

  EXPECT_EQ(actual.getCoe(), 42);
  EXPECT_EQ(actual.getVar(), 0);
  EXPECT_EQ(actual.getExp(), 0);
}

TEST(term, coefVarConstructor) {
  Term actual{42, 'X'};

  EXPECT_EQ(actual.getCoe(), 42);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 0);
}

TEST(term, WrongVarConstructor) {
  EXPECT_THROW((Term{42, 'x'}), std::runtime_error);
}

TEST(term, coefVarExpConstructor) {
  Term actual{42, 'X', 2};

  EXPECT_EQ(actual.getCoe(), 42);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);
}

TEST(term, WrongExpConstructor) {
  EXPECT_THROW((Term{42, 'X', -2}), std::runtime_error);
}

/* assignment operators */

TEST(term, assignment) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'Y', 0};

  lhs = rhs;
  EXPECT_EQ(lhs.getCoe(), 21);
  EXPECT_EQ(lhs.getVar(), 'Y');
  EXPECT_EQ(lhs.getExp(), 0);
}

TEST(term, wrongVarSubtractionAssigment) {
  Term lhs{42, 'X', 2};
  Term rhs{7, 'Y', 2};

  EXPECT_THROW(lhs -= rhs, std::runtime_error);
}

TEST(term, wrongExpSubtractionAssigment) {
  Term lhs{42, 'X', 2};
  Term rhs{7, 'X', 13};

  EXPECT_THROW(lhs -= rhs, std::runtime_error);
}

TEST(term, goodSubtractionAssigment) {
  Term lhs{42, 'X', 2};
  Term rhs{7, 'X', 2};

  lhs -= rhs;
  EXPECT_EQ(lhs.getCoe(), 35);
  EXPECT_EQ(lhs.getVar(), 'X');
  EXPECT_EQ(lhs.getExp(), 2);
}

TEST(term, wrongVarAdditionAssignment) {
  Term actual{42, 'X', 2};
  Term expected{7, 'Y', 2};

  EXPECT_THROW(actual += expected, std::runtime_error);
}

TEST(term, wrongExpAdditionAssignment) {
  Term actual{42, 'X', 2};
  Term expected{7, 'X', 13};

  EXPECT_THROW(actual += expected, std::runtime_error);
}

TEST(term, goodAdditionAssignment) {
  Term actual{42, 'X', 2};
  Term expected{7, 'X', 2};

  actual += expected;
  EXPECT_EQ(actual.getCoe(), 49);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);
}

TEST(term, negateZero) {
  Term actual{0, 'X', 2};

  EXPECT_THROW(-actual, std::runtime_error);
}

TEST(term, negateGood) {
  Term actual{42, 'X', 2};

  actual = -actual;
  EXPECT_EQ(actual.getCoe(), -42);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);

  actual = -actual;
  EXPECT_EQ(actual.getCoe(), 42);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);
}

/* addition */

TEST(term, wrongVarAddition) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'Y', 2};

  EXPECT_THROW(lhs + rhs, std::runtime_error);
}

TEST(term, wrongExpAddition) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'X', 0};

  EXPECT_THROW(lhs + rhs, std::runtime_error);
}

TEST(term, goodAddition) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'X', 2};

  Term actual{lhs + rhs};
  EXPECT_EQ(actual.getCoe(), 63);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);
}

/* subtraction */

TEST(term, wrongVarSubtraction) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'Y', 2};

  EXPECT_THROW(lhs - rhs, std::runtime_error);
}

TEST(term, wrongExpSubtraction) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'X', 0};

  EXPECT_THROW(lhs - rhs, std::runtime_error);
}

TEST(term, goodSubtraction) {
  Term lhs{42, 'X', 2};
  Term rhs{21, 'X', 2};

  Term actual{lhs - rhs};
  EXPECT_EQ(actual.getCoe(), 21);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 2);
}

/* multiplication */

TEST(term, zeroMultiplication) {
  Term lhs{4, 'X', 2};
  Term rhs{0, 'X', 2};

  Term actual{lhs * rhs};
  EXPECT_EQ(actual.getCoe(), 0);
}

TEST(term, wrongVarMultiplication) {
  Term lhs{4, 'X', 2};
  Term rhs{3, 'Y', 2};

  EXPECT_THROW(lhs * rhs, std::runtime_error);
}

TEST(term, goodMultiplication) {
  Term lhs{4, 'X', 2};
  Term rhs{3, 'X', 2};

  Term actual{lhs * rhs};
  EXPECT_EQ(actual.getCoe(), 12);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 4);
}

TEST(term, goodExpMultiplication) {
  Term lhs{4, 'X', 2};
  Term rhs{3, 'X', 8};

  Term actual{lhs * rhs};
  EXPECT_EQ(actual.getCoe(), 12);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 10);
}

/* division */

TEST(term, zeroDivision) {
  Term lhs{4, 'X', 2};
  Term rhs{0, 'X', 2};

  EXPECT_THROW(lhs / rhs, std::runtime_error);
  EXPECT_THROW(rhs / lhs, std::runtime_error);
}

TEST(term, wrongVarDivision) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'Y', 2};

  EXPECT_THROW(lhs / rhs, std::runtime_error);
}

TEST(term, goodDivision) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'X', 2};

  Term actual{lhs / rhs};
  EXPECT_EQ(actual.getCoe(), 3);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 0);
}

TEST(term, goodExpDivision) {
  Term lhs{12, 'X', 9};
  Term rhs{4, 'X', 1};

  Term actual{lhs / rhs};
  EXPECT_EQ(actual.getCoe(), 3);
  EXPECT_EQ(actual.getVar(), 'X');
  EXPECT_EQ(actual.getExp(), 8);
}

/* likeTerms */

TEST(likeTerms, differentVar) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'Y', 2};

  EXPECT_FALSE(likeTerms(lhs, rhs));
}

TEST(likeTerms, differentExp) {
  Term lhs{12, 'X', 5};
  Term rhs{4, 'X', 2};

  EXPECT_FALSE(likeTerms(lhs, rhs));
}

TEST(likeTerms, sameVarExp) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'X', 2};

  EXPECT_TRUE(likeTerms(lhs, rhs));
}

/* sameVars */

TEST(sameVars, differentVar) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'Y', 4};

  EXPECT_FALSE(sameVars(lhs, rhs));
}

TEST(sameVars, sameVar) {
  Term lhs{12, 'X', 2};
  Term rhs{4, 'X', 4};

  EXPECT_TRUE(sameVars(lhs, rhs));
}

/* isConstant */

TEST(isConstant, withVar) {
  Term lhs{12, 'X'};

  EXPECT_FALSE(isConstant(lhs));
}

TEST(isConstant, withExp) {
  Term lhs{12};

  lhs.setExp(4);
  EXPECT_FALSE(isConstant(lhs));
}

TEST(isConstant, withoutVar) {
  Term lhs{12};

  EXPECT_TRUE(isConstant(lhs));
}
