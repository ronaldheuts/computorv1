#pragma once

#include <iostream>
#include <variant>

#include "parser.h"
#include "utils.h"
#include "visitors.h"

class Interpreter {
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;
  Interpreter(Tree& t);

  char   findVar() const;
  double findCoef(const char var, const int exp) const;
  void   transpose();
  void   evaluate();

 private:
  RpnVisitor rpn;
  Tree       tree;
};
