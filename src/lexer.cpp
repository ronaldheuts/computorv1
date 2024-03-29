#include "lexer.h"

/* Lexer */

Lexer::Lexer() : scanner{}, buffer{}, ready{false}, full{false} {}

Lexer::Lexer(const std::string &s)
    : scanner{s}, buffer{}, ready{true}, full{false} {}

void Lexer::stream(const std::string &s) {
  scanner = std::istringstream{s};
  full = false;
  ready = true;
}

Token Lexer::number() {
  double d{0};
  scanner.unget();
  scanner >> d;
  return Token{Token::Kind::kNumber, d};
}

bool Lexer::isReady() const { return ready; }

Token Lexer::get(void) {
  if (!isReady()) {
    throw std::invalid_argument("can not tokenize empty input string");
  }
  if (full) {
    full = false;
    return buffer;
  }

  for (;;) {
    char ch = scanner.get();
    switch (ch) {
      case ' ':
        continue;
      case 'q':
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
          ready = false;
          throw grammarError(std::string{"character not supported: "} +
                             std::string{ch});
        }
      }
    }
  }
}

void Lexer::putback(Token token) {
  if (full) {
    ready = false;
    throw(std::runtime_error("buffer full"));
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
