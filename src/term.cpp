#include "term.h"

/* Helper functions */

/// @brief check if two terms (e.g. X^2) share the same variables and exponents
bool likeTerms(const Term& lhs, const Term& rhs) {
  return lhs.var == rhs.var && lhs.exp == rhs.exp;
}

bool unlikeTerms(const Term& lhs, const Term& rhs) {
  return !(likeTerms(lhs, rhs));
}

bool likeVars(const Term& lhs, const Term& rhs) { return lhs.var == rhs.var; }

bool isConstant(const Term& lhs) { return !lhs.var; }

bool sameVars(const Term& lhs, const Term& rhs) { return lhs.var == rhs.var; }

/* Term */

Term::Term() : coe{0}, var{0}, exp{0} {}

Term::Term(const double c) : coe{c}, var{0}, exp{0} {}

Term::Term(const double c, const char v) : coe{c}, var{v}, exp{0} {}

Term::Term(const double c, const char v, const int e)
    : coe{c}, var{v}, exp{e} {}

Term& Term::operator=(const Term& rhs) {
  coe = rhs.coe;
  var = rhs.var;
  exp = rhs.exp;
  return *this;
}

Term& Term::operator-=(const Term& rhs) {
  *this - rhs;
  return *this;
}

Term& Term::operator+=(const Term& rhs) {
  coe += rhs.coe;
  return *this;
}

Term Term::operator-() const {
  Term term{*this};
  term.coe = -term.coe;
  return term;
}

std::ostream& operator<<(std::ostream& os, const Term& lhs) {
  os << lhs.coe;
  if (std::isalpha(lhs.var)) {
    os << " * " << lhs.var << "^" << lhs.exp;
  }
  return os;
}

Term operator+(const Term& lhs, const Term& rhs) {
  if (likeTerms(lhs, rhs)) {
    return Term{lhs.coe + rhs.coe, lhs.var, lhs.exp};
  }
  throw(std::runtime_error("can not add unlike terms"));
}

Term operator-(const Term& lhs, const Term& rhs) {
  if (likeTerms(lhs, rhs)) {
    return Term{lhs.coe - rhs.coe, lhs.var, lhs.exp};
  }
  throw(std::runtime_error("can not subtract unlike terms"));
}

Term operator*(const Term& lhs, const Term& rhs) {
  if (isConstant(lhs)) {
    return Term{lhs.coe * rhs.coe, rhs.var, rhs.exp};
  } else if (isConstant(rhs)) {
    return Term{lhs.coe * rhs.coe, lhs.var, lhs.exp};
  } else if (sameVars(lhs, rhs)) {
    return Term{lhs.coe * rhs.coe, lhs.var, lhs.exp + rhs.exp};
  }
  throw(std::runtime_error("can not factor unlike terms"));
}

Term operator/(const Term& lhs, const Term& rhs) {
  if (isConstant(lhs)) {
    return Term{lhs.coe / rhs.coe, rhs.var, rhs.exp};
  } else if (isConstant(rhs)) {
    return Term{lhs.coe / rhs.coe, lhs.var, lhs.exp};
  } else if (sameVars(lhs, rhs)) {
    return Term{lhs.coe / rhs.coe, lhs.var, lhs.exp + rhs.exp};
  }
  throw(std::runtime_error("can not divide unlike terms"));
}

bool operator<(const Term& lhs, const Term& rhs) {
  return lhs.var < rhs.var || lhs.exp < rhs.exp;
}

bool operator==(const Term& lhs, const Term& rhs) {
  return lhs.coe == rhs.coe && lhs.var == rhs.var && lhs.exp == rhs.exp;
}

bool operator!=(const Term& lhs, const Term& rhs) { return !(lhs == rhs); }

bool operator<(const Term& lhs, const double& rhs) { return lhs.coe < rhs; }

bool operator>(const Term& lhs, const double& rhs) { return lhs.coe > rhs; }

bool operator!(const Term& lhs) { return !lhs.coe; }
