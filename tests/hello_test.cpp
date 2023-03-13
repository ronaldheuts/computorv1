#include <gtest/gtest.h>

#include "utils.h"

// Demonstrate some basic assertions.
TEST(HelloTest, BasicAssertions) {
  // Expect two strings not to be equal.
  EXPECT_STRNE("hello", "world");
  // Expect equality.
  EXPECT_EQ(7 * 6, 42);
}

TEST(Exponentiation, baseZero) { EXPECT_EQ(utils::exponentiation(0, 2), 0); }
