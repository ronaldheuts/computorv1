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

struct PostFixPrintVisitor {
  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Term &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};

struct RpnVisitor {
  RpnVisitor();

  std::vector<Term> terms;
  std::map<std::pair<char, int>, Term> reduced;

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Term &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};
