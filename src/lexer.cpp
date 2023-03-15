#include "lexer.h"

/* Scanner */

// Scanner::Scanner() : m_stream{} {}

// Scanner::Scanner(const std::string &s) : m_stream{s} {}

// char Scanner::peek(void) { return m_stream.peek(); }

// char Scanner::get(void) { return m_stream.get(); }

// void Scanner::unget(void) { m_stream.unget(); }

/* Lexer */

Lexer::Lexer() : scanner{}, buffer{}, full{false} {}

Lexer::Lexer(const std::string &s) : scanner{s}, buffer{}, full{false} {}

void Lexer::stream(const std::string &s) { scanner = std::istringstream{s}; }

Token Lexer::number() {
  double d{0};
  scanner.unget();
  scanner >> d;
  return Token{Token::Kind::NUMBER, d};
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
      case '(':
      case ')':
        return Token{Token::Kind{ch}, {ch}};
      default: {
        if (std::isdigit(ch)) {
          return number();
        } else if (std::isalpha(ch)) {
          return Token{Token::Kind::VARIABLE, ch};
        } else if (scanner.eof()) {
          return Token{Token::Kind::END};
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
