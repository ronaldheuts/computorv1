#include "lexer.h"

/* Lexer */

Lexer::Lexer() : scanner{}, buffer{}, full{false} {}

Lexer::Lexer(const std::string &s) : scanner{s}, buffer{}, full{false} {}

void Lexer::stream(const std::string &s) { scanner = std::istringstream{s}; }

Token Lexer::number() {
  double d{0};
  scanner.unget();
  scanner >> d;

  if (d > std::numeric_limits<int>::max() ||
      d < std::numeric_limits<int>::min()) {
    throw std::invalid_argument(
        "input number is too big, permitted range is: [" +
        std::to_string(std::numeric_limits<int>::min()) + ", " +
        std::to_string(std::numeric_limits<int>::max()) + "]");
  }
  return Token{Token::Kind::kNumber, d};
}

Token Lexer::get(void) {
  if (full) {
    full = false;
    return buffer;
  }

  for (;;) {
    char ch = scanner.get();
    switch (ch) {
      case ' ':
        continue;
      case '+':
      case '-':
      case '*':
      case '/':
      case '^':
      case '=':
        return Token{Token::Kind{ch}, {ch}};
      default: {
        if (std::isdigit(ch)) {
          return number();
        } else if (std::isalpha(ch)) {
          return Token{Token::Kind::kVariable, ch};
        } else if (scanner.eof()) {
          return Token{Token::Kind::kEnd};
        } else {
          throw(std::invalid_argument("unknown token"));
        }
      }
    }
  }
}

void Lexer::putback(Token token) {
  if (full) {
    throw(std::invalid_argument("buffer full"));
  }
  buffer = token;
  full = true;
}

Token Lexer::peek(void) {
  if (full) {
    return buffer;
  }
  putback(get());
  return buffer;
}
