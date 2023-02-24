#include "../include/lexer.h"

/* Scanner */

// Scanner::Scanner() : m_stream{} {}

// Scanner::Scanner(const std::string &s) : m_stream{s} {}

// char Scanner::peek(void) { return m_stream.peek(); }

// char Scanner::get(void) { return m_stream.get(); }

// void Scanner::unget(void) { m_stream.unget(); }

/* Lexer */

Lexer::Lexer() : m_scanner{} {}

Lexer::Lexer(const std::string &s) : m_scanner{s} {}

void Lexer::stream(const std::string &s) { m_scanner = std::istringstream{s}; }

Token Lexer::number() {
  double d{0};
  m_scanner.unget();
  m_scanner >> d;
  return Token{Token::Kind::NUMBER, d};
}

Token Lexer::variable(char ch) {
  std::string s{ch};
  while (std::isalpha(ch = m_scanner.get())) s += ch;
  if (m_scanner.good()) m_scanner.unget();  // don't unget when eof
  return Token{Token::Kind::VARIABLE, s};
}

Token Lexer::get(void) {
  if (m_full) {
    m_full = false;
    return m_buffer;
  }

  for (;;) {
    char ch = m_scanner.get();
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
          return variable(ch);
        } else if (m_scanner.eof()) {
          return Token{Token::Kind::END};
        } else {
          throw(std::invalid_argument("unknown token"));
        }
      }
    }
  }
}

void Lexer::putback(Token t) {
  if (m_full) {
    throw(std::invalid_argument("buffer full"));
  }
  m_buffer = t;
  m_full = true;
}

Token Lexer::peek(void) {
  if (m_full) {
    return m_buffer;
  }
  putback(get());
  return m_buffer;
}
