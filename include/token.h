#pragma once

#include <cstdint>
#include <iostream>
#include <map>
#include <string>
#include <variant>

// std::istream& operator>>(std::istream& is, Term& t) {
//   is >> t.coefficient >> t.variable >> t.exponent;
//   return is;
// }

struct Token {
  using val_t = std::variant<std::monostate, double, std::string>;

  enum class Kind : char {
    END = 0,
    NUMBER,
    VARIABLE,
    PLUS = '+',
    MINUS = '-',
    ASTERISK = '*',
    SLASH = '/',
    CARET = '^',
    LPAREN = '(',
    RPAREN = ')',
    EQUAL = '=',
  };

  Token();
  Token(Kind k);
  Token(Kind k, val_t v);

  val_t value;
  Kind kind;
};
