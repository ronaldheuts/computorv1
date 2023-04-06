#pragma once

#include <map>
#include <queue>  // may remove
#include <variant>

#include "parser.h"
#include "utils.h"

struct BinaryExpr;
struct UnaryExpr;
struct Term;

struct PrintVisitor {
  int         height;
  int         levels;
  std::string prefix;

  PrintVisitor();

  void operator()(const BinaryExpr &expr);
  void operator()(const UnaryExpr &expr);
  void operator()(const Term &expr);
};

struct QueueElem {
  enum class Orientation { kLeft, kRight, kCenter };

  std::string value;
  int         level;
  int         pos;
  Orientation orientation;
};

struct PrettyPrint {
  int levels;
  int level;
  int width;
  // QueueElem             buffer;
  std::queue<QueueElem> q;

  PrettyPrint(int levels);
  void buildQueue();

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
