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

struct BinaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num>;
  BinaryExpr(Token::Kind, std::unique_ptr<node_t> &, std::unique_ptr<node_t> &);

  Token::Kind oper;
  std::unique_ptr<node_t> left;
  std::unique_ptr<node_t> right;
};

struct UnaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num>;
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
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num>;

 public:
  Tree(void) : m_root{} {}

  void root(std::unique_ptr<node_t> &);

 private:
  Tree(const Tree &);
  Tree &operator=(const Tree &);

  std::unique_ptr<node_t> m_root;
};

/* Parser */

class Parser {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num>;

 public:
  Parser();

  void parse(void);
  void stream(const std::string &s);
  std::string prompt(void);

 private:
  Parser(const Parser &);
  Parser &operator=(const Parser &);

  [[nodiscard]] bool match(const Token &token, Token::Kind kind);
  [[nodiscard]] bool check(const Token &token, Token::Kind kind);
  [[nodiscard]] Token peek(void);
  Token advance(void);

  [[nodiscard]] std::unique_ptr<node_t> primary(void);
  [[nodiscard]] std::unique_ptr<node_t> unary(void);
  [[nodiscard]] std::unique_ptr<node_t> factor(void);
  [[nodiscard]] std::unique_ptr<node_t> power(void);
  [[nodiscard]] std::unique_ptr<node_t> expression(void);
  [[nodiscard]] std::unique_ptr<node_t> equation(void);

  Lexer m_lexer;
  Tree m_tree;
};
