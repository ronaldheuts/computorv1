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
  char var;
  int exp;

  Term &operator=(const Term &a);
  Term &operator-=(const Term &a);
  Term &operator+=(const Term &a);
};

bool likeTerms(const Term &a, const Term &b);
bool unlikeTerms(const Term &a, const Term &b);
bool isConstant(const Term &a);

Term operator*(const Term &a, const Term &b);
Term operator/(const Term &a, const Term &b);
Term operator+(const Term &a, const Term &b);
Term operator-(const Term &a, const Term &b);

bool operator<(const Term &a, const Term &b);
