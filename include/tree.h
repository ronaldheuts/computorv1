#pragma once

#include <memory>
#include <variant>

#include "term.h"
#include "token.h"

/* Nodes */

struct UnaryExpr;
struct Term;

struct BinaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;

  BinaryExpr(Token::Kind, std::unique_ptr<node_t> &, std::unique_ptr<node_t> &);

  Token::Kind             oper;
  std::unique_ptr<node_t> left;
  std::unique_ptr<node_t> right;
};

struct UnaryExpr {
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;

  UnaryExpr(Token::Kind, std::unique_ptr<node_t> &);

  Token::Kind             oper;
  std::unique_ptr<node_t> child;
};

/* Tree */

class Tree {
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;

  Tree();

  void                     setRoot(std::unique_ptr<node_t> expr);
  std::unique_ptr<node_t> &getRoot();

 private:
  Tree(const Tree &) = delete;
  Tree &operator=(const Tree &) = delete;

  std::unique_ptr<node_t> root;
};
