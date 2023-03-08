#pragma once

#include <sstream>
#include <stdexcept>

#include "token.h"

// class Scanner {
//  public:
//   Scanner();
//   Scanner(const std::string &);

//   char peek(void);
//   char get(void);
//   void unget(void);

//   //  private:
//   std::istringstream m_stream;
// };

// // std::istringstream &operator>>(std::istringstream &is, Scanner &s) {
// //   return s.m_stream;
// // }

class Lexer {
 public:
  Lexer();
  Lexer(const std::string &s);

  // void stream(const std::string &s);
  // void stream(const std::ifstream &ifs);
  // void stream(const std::istream &is);

  Token get(void);
  Token peek(void);
  void putback(Token);
  void stream(const std::string &);

 private:
  Lexer(const Lexer &);

  Token number(void);
  Token variable(char ch);

  std::istringstream m_scanner;
  Token m_buffer;
  bool m_full;
};