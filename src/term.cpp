#include "../include/term.h"

Factor::Factor() : variable{}, exponent{1} {}

Factor::Factor(const char v) : variable{v}, exponent{1} {}

Factor::Factor(const char v, const int e) : variable{v}, exponent{e} {}

bool operator==(const Factor& a, const Factor& b) {
  return a.variable == b.variable && a.exponent == b.exponent;
}

bool operator!=(const Factor& a, const Factor& b) { return !(a == b); }

bool operator<(const Factor& a, const Factor& b) {
  if (a.variable == b.variable) {
    return a.exponent < b.exponent;
  }
  return a.variable < b.variable;
}

bool likeTerms(const Term& a, const Term& b) { return a.vars == b.vars; }

bool unlikeTerms(const Term& a, const Term& b) { return !(likeTerms(a, b)); }

bool isConstant(const Term& a) { return a.vars.empty(); }

Term::Term() : coefficient{}, vars{} {}

Term::Term(const double c, const std::set<Factor>& v)
    : coefficient{c}, vars{v} {}

Term operator+(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coefficient + b.coefficient, a.vars};
  }
  throw(std::runtime_error("can not add unlike terms"));
}

Term operator-(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coefficient - b.coefficient, a.vars};
  }
  throw(std::runtime_error("can not subtract unlike terms"));
}

Term operator*(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    std::set<Factor> factors;
    double c{a.coefficient * b.coefficient};

    for (auto it_a = a.vars.begin(), it_b = b.vars.begin();
         it_a != a.vars.end() || it_b != b.vars.end(); ++it_a, ++it_b) {
      factors.insert(Factor{it_a->variable, it_a->exponent + it_b->exponent});
    }

    return Term{c, factors};
  } else if (isConstant(a)) {
    return Term{a.coefficient * b.coefficient, b.vars};
  } else if (isConstant(b)) {
    return Term{a.coefficient * b.coefficient, a.vars};
  }
  throw(std::runtime_error("can not factor unlike terms - YET"));
}