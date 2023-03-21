#pragma once

#include <variant>

// #include <iostream>
// std::istream& operator>>(std::istream& is, Term& t) {
//   is >> t.coe >> t.variable >> t.exponent;
//   return is;
// }

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
