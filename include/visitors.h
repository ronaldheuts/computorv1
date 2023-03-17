#pragma once

#include <cassert>
#include <map>
#include <utility>
#include <variant>
#include <vector>

#include "parser.h"
#include "utils.h"

struct BinaryExpr;
struct UnaryExpr;
struct Var;
struct Num;
struct Term;

struct PrintVisitor {
  int height;
  std::string prefix;

  PrintVisitor();

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Term &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};

struct TransposeVisitor {
  void operator()(BinaryExpr &expr);
  void operator()(UnaryExpr &expr);
  void operator()(Term &expr);
  void operator()(Var &expr);
  void operator()(Num &expr);
};

struct RpnVisitor {
  RpnVisitor();

  std::vector<Term> terms;
  std::map<std::pair<char, int>, Term> reduced;

  void evaluate(const BinaryExpr &expr, Term term);

  Term operator()(const BinaryExpr &expr);
  Term operator()(const UnaryExpr &expr);
  Term operator()(const Term &expr);
  Term operator()(const Var &expr);
  Term operator()(const Num &expr);
};
