#pragma once

#include <iostream>
#include <variant>

#include "parser.h"
#include "visitors.h"

class Interpreter {
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;
  Interpreter(Tree& t);

  void transpose();
  void evaluate();

 private:
  Tree tree;
};
