#include "lexer.h"

#include <gtest/gtest.h>

TEST(lexer, defaultConstructor) {
  Lexer lexer{};
  EXPECT_FALSE(lexer.isready());
}

TEST(lexer, setStream) {
  Lexer lexer{};
  EXPECT_FALSE(lexer.isready());
  lexer.stream(std::string{"hello world"});
  EXPECT_TRUE(lexer.isready());
}

TEST(lexer, paramConstructor) {
  Lexer lexer{std::string("hello world")};
  EXPECT_TRUE(lexer.isready());
}

TEST(lexer, getEmpty) {
  Lexer lexer{""};

  Token token = lexer.get();
  EXPECT_EQ(token.kind, Token::Kind::kEnd);
}

TEST(lexer, getVariableToken) {
  std::string input("hello");
  Lexer       lexer{input};
  Token       token;

  for (size_t i = 0; i < input.size(); ++i) {
    token = lexer.get();
    EXPECT_EQ(std::get<char>(token.value), input.at(i));
    EXPECT_EQ(token.kind, Token::Kind::kVariable);
  }
  token = lexer.get();
  EXPECT_EQ(token.kind, Token::Kind::kEnd);
  EXPECT_EQ(std::get<std::monostate>(token.value), std::monostate{});
}

TEST(lexer, getNumberToken) {
  Lexer lexer{"123456"};
  Token token = lexer.get();

  EXPECT_EQ(std::get<double>(token.value), 123456);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
}

TEST(lexer, getAllTokenKinds) {
  Lexer lexer{"3 * X ^ 2 + 3 / 5 - 7 = 0"};

  Token token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 3);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '*');
  EXPECT_EQ(token.kind, Token::Kind::kAsterisk);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'X');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '^');
  EXPECT_EQ(token.kind, Token::Kind::kCaret);
  token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 2);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '+');
  EXPECT_EQ(token.kind, Token::Kind::kPlus);
  token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 3);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '/');
  EXPECT_EQ(token.kind, Token::Kind::kSlash);
  token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 5);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '-');
  EXPECT_EQ(token.kind, Token::Kind::kMinus);
  token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 7);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), '=');
  EXPECT_EQ(token.kind, Token::Kind::kEqual);
  token = lexer.get();
  EXPECT_EQ(std::get<double>(token.value), 0);
  EXPECT_EQ(token.kind, Token::Kind::kNumber);
  token = lexer.get();
  EXPECT_EQ(std::get<std::monostate>(token.value), std::monostate{});
  EXPECT_EQ(token.kind, Token::Kind::kEnd);
}

TEST(lexer, peek) {
  Lexer lexer{"hello"};

  Token token = lexer.peek();
  EXPECT_EQ(std::get<char>(token.value), 'h');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'h');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.peek();
  EXPECT_EQ(std::get<char>(token.value), 'e');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'e');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.peek();
  EXPECT_EQ(std::get<char>(token.value), 'l');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'l');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.peek();
  EXPECT_EQ(std::get<char>(token.value), 'l');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'l');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.peek();
  EXPECT_EQ(std::get<char>(token.value), 'o');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.get();
  EXPECT_EQ(std::get<char>(token.value), 'o');
  EXPECT_EQ(token.kind, Token::Kind::kVariable);
  token = lexer.peek();
  EXPECT_EQ(std::get<std::monostate>(token.value), std::monostate{});
  EXPECT_EQ(token.kind, Token::Kind::kEnd);
  token = lexer.get();
  EXPECT_EQ(std::get<std::monostate>(token.value), std::monostate{});
  EXPECT_EQ(token.kind, Token::Kind::kEnd);
}
