#include "term.h"

/* Term helper functions */

/// @brief check if two terms (e.g. X^2) share the same variables and exponents
bool likeTerms(const Term& a, const Term& b) {
  return a.var == b.var && a.exp == b.exp;
}

bool unlikeTerms(const Term& a, const Term& b) { return !(likeTerms(a, b)); }

bool isConstant(const Term& a) { return !a.var; }

bool sameVars(const Term& a, const Term& b) { return a.var == b.var; }

/* Term */

Term::Term() : coe{0}, var{0}, exp{0} {}

Term::Term(const double c) : coe{c}, var{0}, exp{0} {}

Term::Term(const double c, const char v) : coe{c}, var{v}, exp{0} {}

Term::Term(const double c, const char v, const int e)
    : coe{c}, var{v}, exp{e} {}

Term& Term::operator=(const Term& a) {
  coe = a.coe;
  var = a.var;
  exp = a.exp;
  return *this;
}

Term& Term::operator-=(const Term& a) {
  *this - a;
  return *this;
}

Term& Term::operator+=(const Term& a) {
  coe += a.coe;
  return *this;
}

Term operator+(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coe + b.coe, a.var, a.exp};
  }
  throw(std::runtime_error("can not add unlike terms"));
}

Term operator-(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coe - b.coe, a.var, a.exp};
  }
  throw(std::runtime_error("can not subtract unlike terms"));
}

Term operator*(const Term& a, const Term& b) {
  if (isConstant(a)) {
    return Term{a.coe * b.coe, b.var, b.exp};
  } else if (isConstant(b)) {
    return Term{a.coe * b.coe, a.var, a.exp};
  } else if (sameVars(a, b)) {
    return Term{a.coe * b.coe, a.var, a.exp + b.exp};
  }
  throw(std::runtime_error("can not factor unlike terms"));
}

Term operator/(const Term& a, const Term& b) {
  if (isConstant(a)) {
    return Term{a.coe / b.coe, b.var, b.exp};
  } else if (isConstant(b)) {
    return Term{a.coe / b.coe, a.var, a.exp};
  } else if (sameVars(a, b)) {
    return Term{a.coe / b.coe, a.var, a.exp + b.exp};
  }
  throw(std::runtime_error("can not divide unlike terms"));
}

bool operator<(const Term& a, const Term& b) {
  return a.var < b.var || a.exp < b.exp;
}