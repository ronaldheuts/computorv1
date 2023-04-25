#pragma once

#include <iostream>
#include <stdexcept>
#include <variant>

#include "exceptions.h"
#include "parser.h"
#include "utils.h"
#include "visitors.h"

class Interpreter {
 public:
  using node_t = std::variant<BinaryExpr, UnaryExpr, Term>;
  using solutions_t = std::vector<std::variant<double, utils::Complex>>;

  Interpreter(Tree& t);

  solutions_t getSolutions() const;
  char        findVar() const;
  double      findCoef(const char var, const int exp) const;
  void        transpose();
  void        evaluate();

 private:
  Interpreter() = delete;
  Interpreter(const Interpreter&) = delete;
  Interpreter& operator=(const Interpreter&) = delete;

  solutions_t solutions;
  RpnVisitor  rpn;
  Tree        tree;
};
