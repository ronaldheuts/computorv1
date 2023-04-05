#pragma once

#include <variant>

struct Token {
  using val_t = std::variant<std::monostate, double, char>;

  enum class Kind : char {
    kEnd = 0,
    kNumber,
    kVariable,
    kPlus = '+',
    kMinus = '-',
    kAsterisk = '*',
    kSlash = '/',
    kCaret = '^',
    kLeftParen = '(',
    kRightParen = ')',
    kEqual = '=',
  };

  Token();
  Token(Kind k);
  Token(Kind k, val_t v);

  val_t value;
  Kind  kind;
};
