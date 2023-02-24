#pragma once

#include <variant>
#include <vector>

#include "parser.h"

struct BinaryExpr;
struct UnaryExpr;
struct Var;
struct Num;

struct PrintVisitor {
  void operator()(const BinaryExpr &expr) const;
  void operator()(const UnaryExpr &expr) const;
  void operator()(const Var &expr) const;
  void operator()(const Num &expr) const;
};

struct RpnVisitor {
  using val_t = Token::val_t;

  RpnVisitor(void);

  std::vector<val_t> operands;
  std::vector<Term> terms;
  std::vector<val_t> buffer;

  void evaluate(const BinaryExpr &expr);
  void evaluate(const UnaryExpr &expr);

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Var &expr);
  void operator()(const Num &expr);
};
