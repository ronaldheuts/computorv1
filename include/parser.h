#pragma once

#include <algorithm>
#include <map>
#include <memory>
#include <set>
#include <string_view>
#include <vector>

#include "lexer.h"
#include "term.h"
#include "visitors.h"

/* Nodes */

struct UnaryExpr;  // forward declaration
struct Var;        // forward declaration
struct Num;        // forward declaration
struct Term;

struct BinaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

  BinaryExpr(Token::Kind, std::unique_ptr<node_t> &, std::unique_ptr<node_t> &);

  Token::Kind oper;
  std::unique_ptr<node_t> left;
  std::unique_ptr<node_t> right;
};

struct UnaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

  UnaryExpr(Token::Kind, std::unique_ptr<node_t> &);

  Token::Kind oper;
  std::unique_ptr<node_t> child;
};

struct Var {
  char value;
};

struct Num {
  double value;
};

/* Tree */

class Tree {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

 public:
  Tree(void) : root{} {}

  void setRoot(std::unique_ptr<node_t> &);

 private:
  Tree(const Tree &);
  Tree &operator=(const Tree &);

  std::unique_ptr<node_t> root;
};

/* Parser */

class Parser {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

 public:
  Parser();

  void parse();
  void stream(const std::string &s);
  std::string prompt();

 private:
  Parser(const Parser &);
  Parser &operator=(const Parser &);

  [[nodiscard]] bool match(const Token &token, Token::Kind kind);
  [[nodiscard]] bool check(const Token &token, Token::Kind kind);
  [[nodiscard]] Token peek();
  Token advance();
  void putback(Token);

  [[nodiscard]] std::unique_ptr<node_t> primary();
  [[nodiscard]] std::unique_ptr<node_t> term();
  [[nodiscard]] std::unique_ptr<node_t> unary();
  [[nodiscard]] std::unique_ptr<node_t> factor();
  [[nodiscard]] std::unique_ptr<node_t> power();
  [[nodiscard]] std::unique_ptr<node_t> expression();
  [[nodiscard]] std::unique_ptr<node_t> equation();

  Lexer lexer;
  Tree tree;
};
