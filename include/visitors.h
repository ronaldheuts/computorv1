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

struct PrintVisitor {
  int height;
  std::string prefix;

  PrintVisitor();

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};

struct RpnVisitor {
  using val_t = Token::val_t;

  RpnVisitor();

  std::vector<Term> terms;
  std::map<std::tuple<char, int>, Term> reduced;

  void evaluate(const BinaryExpr &expr);
  void evaluate(const UnaryExpr &expr);

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};
