#pragma once

#include <memory>

#include "term.h"
#include "token.h"

/* Nodes */

struct UnaryExpr;  // forward declaration
struct Var;        // forward declaration
struct Num;        // forward declaration
struct Term;

struct BinaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

  BinaryExpr(Token::Kind, std::unique_ptr<node_t> &, std::unique_ptr<node_t> &);

  Token::Kind             oper;
  std::unique_ptr<node_t> left;
  std::unique_ptr<node_t> right;
};

struct UnaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;

  UnaryExpr(Token::Kind, std::unique_ptr<node_t> &);

  Token::Kind             oper;
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
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Var, Num, Term>;
  Tree(void);

  void    setRoot(std::unique_ptr<node_t> expr);
  node_t &getRoot() const;

  std::unique_ptr<node_t> root;

 private:
  Tree(const Tree &);
  Tree &operator=(const Tree &);
};
