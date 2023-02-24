#pragma once

#include <set>
#include <stdexcept>

struct Factor {
  Factor();
  Factor(const char v);
  Factor(const char v, const int e);

  char variable;
  int exponent;
};

bool operator==(const Factor &a, const Factor &b);
bool operator!=(const Factor &a, const Factor &b);
bool operator<(const Factor &a, const Factor &b);

struct Term {
  Term();
  Term(const double c, const std::set<Factor> &v);

  double coefficient;
  std::set<Factor> vars;
};

bool likeTerms(const Term &a, const Term &b);
bool unlikeTerms(const Term &a, const Term &b);
bool isConstant(const Term &a);

Term operator*(const Term &a, const Term &b);
Term operator+(const Term &a, const Term &b);
Term operator-(const Term &a, const Term &b);
