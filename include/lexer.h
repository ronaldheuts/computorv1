#pragma once

#include <sstream>
#include <stdexcept>

#include "exceptions.h"
#include "token.h"

class Lexer {
 public:
  Lexer();
  Lexer(const std::string &s);

  Token get(void);
  Token peek(void);
  void  putback(Token);
  void  stream(const std::string &);
  bool  isReady() const;

 private:
  Lexer(const Lexer &) = delete;
  Lexer &operator=(const Lexer &) = delete;

  Token number(void);

  std::istringstream scanner;
  Token              buffer;
  bool               ready;
  bool               full;
};
