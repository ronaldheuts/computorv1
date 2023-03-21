#pragma once

#include <iostream>
#include <set>
#include <stdexcept>

struct Term {
  Term();
  Term(const double c);
  Term(const double c, const char v);
  Term(const double c, const char v, const int e);

  double coe;
  char   var;
  int    exp;

  Term &operator=(const Term &rhs);
  Term &operator-=(const Term &rhs);
  Term &operator+=(const Term &rhs);

  Term operator-() const;
};

std::ostream &operator<<(std::ostream &os, const Term &lhs);

bool likeTerms(const Term &lhs, const Term &rhs);
bool unlikeTerms(const Term &lhs, const Term &rhs);
bool likeVars(const Term &lhs, const Term &rhs);
bool isConstant(const Term &lhs);

Term operator*(const Term &lhs, const Term &rhs);
Term operator/(const Term &lhs, const Term &rhs);
Term operator+(const Term &lhs, const Term &rhs);
Term operator-(const Term &lhs, const Term &rhs);

bool operator<(const Term &lhs, const Term &rhs);
bool operator==(const Term &lhs, const Term &rhs);
bool operator!=(const Term &lhs, const Term &rhs);

bool operator<(const Term &lhs, const double &rhs);
bool operator>(const Term &lhs, const double &rhs);
bool operator!(const Term &lhs);
