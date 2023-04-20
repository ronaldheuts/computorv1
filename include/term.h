#pragma once

#include <iostream>

#include "exceptions.h"

class Term {
 public:
  Term();
  Term(const double c);
  Term(const double c, const char v);
  Term(const double c, const char v, const int e);

  Term &operator=(const Term &rhs);
  Term &operator-=(const Term &rhs);
  Term &operator+=(const Term &rhs);
  Term  operator-() const;

  Term operator*(const Term &rhs);
  Term operator/(const Term &rhs);
  Term operator+(const Term &rhs);
  Term operator-(const Term &rhs);

  double getCoe() const;
  void   setCoe(const double c);
  char   getVar() const;
  void   setVar(const char v);
  int    getExp() const;
  void   setExp(const int e);

 private:
  double coe;
  char   var;
  int    exp;
};

std::ostream &operator<<(std::ostream &os, const Term &lhs);

bool likeTerms(const Term &lhs, const Term &rhs);
bool sameVars(const Term &lhs, const Term &rhs);
bool isConstant(const Term &lhs);

bool operator<(const Term &lhs, const Term &rhs);
bool operator==(const Term &lhs, const Term &rhs);
bool operator!=(const Term &lhs, const Term &rhs);

bool operator<(const Term &lhs, const double &rhs);
bool operator>(const Term &lhs, const double &rhs);
bool operator!(const Term &lhs);
