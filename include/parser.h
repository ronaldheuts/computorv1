#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "lexer.h"
#include "term.h"
#include "tree.h"
#include "visitors.h"

/* Parser */

class Parser {
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;
  Parser();

  void                      parse();
  void                      stream(const std::string &s);
  [[nodiscard]] Tree       &getTree();
  [[nodiscard]] std::string prompt();

 private:
  Lexer lexer;
  Tree  tree;

  Parser(const Parser &) = delete;
  Parser &operator=(const Parser &) = delete;

  Token advance();
  void  putback(Token);

  [[nodiscard]] bool  match(const Token &token, Token::Kind kind);
  [[nodiscard]] bool  check(const Token &token, Token::Kind kind);
  [[nodiscard]] Token peek();
  [[nodiscard]] std::unique_ptr<node_t> primary();
  [[nodiscard]] std::unique_ptr<node_t> term();
  [[nodiscard]] std::unique_ptr<node_t> unary();
  [[nodiscard]] std::unique_ptr<node_t> factor();
  [[nodiscard]] std::unique_ptr<node_t> power();
  [[nodiscard]] std::unique_ptr<node_t> expression();
  [[nodiscard]] std::unique_ptr<node_t> equation();
};
