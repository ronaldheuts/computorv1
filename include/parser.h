#pragma once

#include <map>
#include <memory>
#include <string_view>
#include <vector>

#include "lexer.h"

struct Factor {
  char variable;
  int exponent;
};

struct Term {
  Term(const double c, const std::string v, const int e);
  Term(const double c, const std::string &v);
  Term(const std::string &v);

  double coefficient;
  std::string variable;
  int exponent;

  std::vector<Factor> vars;  // X^2
};

Term operator*(const Term &a, const Term &b);
Term operator+(const Term &a, const Term &b);
Term operator-(const Term &a, const Term &b);

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
  std::string value;
};

struct Num {
  double value;
};

/* Visitors */

struct PrintVisitor {
  void operator()(const BinaryExpr &expr) const;
  void operator()(const UnaryExpr &expr) const;
  void operator()(const Var &expr) const;
  void operator()(const Num &expr) const;
};

struct CalcVisitor {
  using val_t = Token::val_t;

  val_t operator()(const BinaryExpr &expr) const;
  val_t operator()(const UnaryExpr &expr) const;
  val_t operator()(const Var &expr) const;
  val_t operator()(const Num &expr) const;
};

struct RpnVisitor {
  using val_t = Token::val_t;

  RpnVisitor(void);

  std::vector<val_t> operands;
  std::vector<Term> terms;

  void evaluate(const BinaryExpr &expr);
  void evaluate(const UnaryExpr &expr);

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
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
