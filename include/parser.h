#pragma once

#include "lexer.h"
#include "term.h"
#include "tree.h"
#include "utils.h"
#include "visitors.h"

class Parser {
 public:
  using node_t = Tree::node_t;
  Parser();
  Parser(const std::string &s);

  void                      stream(const std::string &s);
  bool                      parse();
  [[nodiscard]] Tree       &getTree();
  [[nodiscard]] std::string prompt();

 private:
  Lexer lexer;
  Tree  tree;

  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;

  Token               advance();
  [[nodiscard]] bool  check(const Token &token, Token::Kind kind);
  [[nodiscard]] Token peek();
  [[nodiscard]] std::unique_ptr<node_t> term();
  [[nodiscard]] std::unique_ptr<node_t> unary();
  [[nodiscard]] std::unique_ptr<node_t> factor();
  [[nodiscard]] std::unique_ptr<node_t> power();
  [[nodiscard]] std::unique_ptr<node_t> expression();
  [[nodiscard]] std::unique_ptr<node_t> equation();
};
