#include "term.h"

/* Helper functions */

/// @brief check if two terms (e.g. X^2) share the same variable and exponent
bool likeTerms(const Term& lhs, const Term& rhs) {
  return lhs.getVar() == rhs.getVar() && lhs.getExp() == rhs.getExp();
}

bool isConstant(const Term& lhs) { return !lhs.getVar(); }

bool sameVars(const Term& lhs, const Term& rhs) {
  return lhs.getVar() == rhs.getVar();
}

/* Term */

Term::Term() : coe{0}, var{0}, exp{0} {}

Term::Term(const double c) : coe{c}, var{0}, exp{0} {}

Term::Term(const double c, const char v) : coe{c}, var{v}, exp{0} {
  if (v > 'Z' || 'A' > v) {
    throw grammarError("the variable in a term must be a capital letter");
  }
}

Term::Term(const double c, const char v, const int e) : coe{c}, var{v}, exp{e} {
  if (v > 'Z' || 'A' > v) {
    throw grammarError("the variable in a term must be a capital letter");
  } else if (e < 0) {
    throw grammarError("the exponent in a term must be positive");
  }
}

double Term::getCoe() const { return coe; }

void Term::setCoe(const double c) { coe = c; }

char Term::getVar() const { return var; }

void Term::setVar(const char v) { var = v; }

int Term::getExp() const { return exp; }

void Term::setExp(const int e) { exp = e; }

Term& Term::operator=(const Term& rhs) {
  coe = rhs.getCoe();
  var = rhs.getVar();
  exp = rhs.getExp();
  return *this;
}

Term& Term::operator-=(const Term& rhs) {
  *this = *this - rhs;
  return *this;
}

Term& Term::operator+=(const Term& rhs) {
  *this = *this + rhs;
  return *this;
}

/// @brief negate operator
Term Term::operator-() const {
  Term term{*this};

  if (!getCoe()) {
    throw std::runtime_error("can not negate 0");
  }
  term.setCoe(-getCoe());
  return term;
}

std::ostream& operator<<(std::ostream& os, const Term& lhs) {
  os << lhs.getCoe();
  if (std::isalpha(lhs.getVar())) {
    os << " * " << lhs.getVar() << "^" << lhs.getExp();
  }
  return os;
}

Term Term::operator+(const Term& rhs) {
  if (likeTerms(*this, rhs)) {
    return Term{getCoe() + rhs.getCoe(), getVar(), getExp()};
  }
  throw std::runtime_error("can not add unlike terms");
}

Term Term::operator-(const Term& rhs) {
  if (likeTerms(*this, rhs)) {
    return Term{getCoe() - rhs.getCoe(), getVar(), getExp()};
  }
  throw std::runtime_error("can not subtract unlike terms");
}

Term Term::operator*(const Term& rhs) {
  if (isConstant(*this)) {
    return Term{getCoe() * rhs.getCoe(), rhs.getVar(), rhs.getExp()};
  } else if (isConstant(rhs)) {
    return Term{getCoe() * rhs.getCoe(), getVar(), getExp()};
  } else if (sameVars(*this, rhs)) {
    return Term{getCoe() * rhs.getCoe(), getVar(), getExp() + rhs.getExp()};
  }
  throw std::runtime_error("can not factor unlike terms");
}

Term Term::operator/(const Term& rhs) {
  if (!getCoe()) {
    throw std::runtime_error("can not divide zero");
  } else if (!rhs.getCoe()) {
    throw std::runtime_error("can not divide by zero");
  } else if (isConstant(*this)) {
    return Term{getCoe() / rhs.getCoe(), rhs.getVar(), rhs.getExp()};
  } else if (isConstant(rhs)) {
    return Term{getCoe() / rhs.getCoe(), getVar(), getExp()};
  } else if (sameVars(*this, rhs)) {
    return Term{getCoe() / rhs.getCoe(), getVar(), getExp() + rhs.getExp()};
  }
  throw std::runtime_error("can not divide unlike terms");
}

bool operator<(const Term& lhs, const Term& rhs) {
  return lhs.getVar() < rhs.getVar() || lhs.getExp() < rhs.getExp();
}

bool operator==(const Term& lhs, const Term& rhs) {
  return lhs.getCoe() == rhs.getCoe() && lhs.getVar() == rhs.getVar() &&
         lhs.getExp() == rhs.getExp();
}

bool operator!=(const Term& lhs, const Term& rhs) { return !(lhs == rhs); }

bool operator<(const Term& lhs, const double& rhs) {
  return lhs.getCoe() < rhs;
}

bool operator>(const Term& lhs, const double& rhs) {
  return lhs.getCoe() > rhs;
}

bool operator!(const Term& lhs) { return !lhs.getCoe(); }
