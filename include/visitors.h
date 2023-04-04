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
struct Term;

struct PrintVisitor {
  int         height;
  std::string prefix;

  PrintVisitor();

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Term &expr);
};

struct TransposeVisitor {
  void operator()(BinaryExpr &expr);
  void operator()(UnaryExpr &expr);
  void operator()(Term &expr);
};

struct RpnVisitor {
  std::map<std::pair<char, int>, Term> terms;

  RpnVisitor();

  void evaluate(const BinaryExpr &expr, Term term);
  Term operator()(const BinaryExpr &expr);
  Term operator()(const UnaryExpr &expr);
  Term operator()(const Term &expr);
};
