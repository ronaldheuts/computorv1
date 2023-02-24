#include "../include/visitors.h"

/* Visitors */

void PrintVisitor::operator()(const BinaryExpr& expr) const {
  std::visit(*this, *(expr.left));
  std::visit(*this, *(expr.right));
  std::cout << static_cast<char>(expr.oper);
}

void PrintVisitor::operator()(const UnaryExpr& expr) const {
  std::visit(*this, *(expr.child));
  std::cout << static_cast<char>(expr.oper);
}

void PrintVisitor::operator()(const Var& expr) const {
  std::cout << expr.value;
}

void PrintVisitor::operator()(const Num& expr) const {
  std::cout << expr.value;
}

/* RpnVisitor */

// "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
// "5 * X^0 + 4 * X^1 = 4 * X^0"
// "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"

RpnVisitor::RpnVisitor(void) : operands{}, terms{} {}

void RpnVisitor::evaluate(const BinaryExpr& expr) {
  Term a{terms.back()};
  terms.pop_back();
  Term b{terms.back()};
  terms.pop_back();

  // val_t a{buffer.back()};
  // buffer.pop_back();
  // val_t b{buffer.back()};
  // buffer.pop_back();

  switch (expr.oper) {
    case Token::Kind::PLUS:
      if (likeTerms(a, b)) {
        terms.emplace_back(a + b);
      } else {
        terms.emplace_back(a);
        terms.emplace_back(b);
      }
      break;
    case Token::Kind::MINUS:
      if (likeTerms(a, b)) {
        terms.emplace_back(a - b);
      } else {
        a.coefficient *= -1;
        terms.emplace_back(a);
        terms.emplace_back(b);
      }
      break;
    case Token::Kind::ASTERISK:
      if (likeTerms(a, b) || isConstant(a) || isConstant(b)) {
        terms.emplace_back(a * b);
      } else {
        terms.emplace_back(a);
        terms.emplace_back(b);
      }
      break;
    case Token::Kind::CARET:
      // terms.emplace_back(Term{})

      // Term{b.coefficient, b.variable, static_cast<int>(a.coefficient)});
      break;
    case Token::Kind::EQUAL:
      a.coefficient *= -1;
      terms.emplace_back(a);
      terms.emplace_back(b);
      break;
  }
}

void RpnVisitor::evaluate(const UnaryExpr& expr) {
  Term a{terms.back()};
  terms.pop_back();

  switch (expr.oper) {
    case Token::Kind::MINUS:
      terms.emplace_back(Term{-a.coefficient, a.vars});
      break;
    case Token::Kind::PLUS:  // maybe refuse this
      terms.emplace_back(a);
      break;
  }
}

void RpnVisitor::operator()(const BinaryExpr& expr) {
  std::visit(*this, *(expr.left));
  std::visit(*this, *(expr.right));
  evaluate(expr);
}

void RpnVisitor::operator()(const UnaryExpr& expr) {
  std::visit(*this, *(expr.child));
  evaluate(expr);
}

void RpnVisitor::operator()(const Num& expr) {
  terms.emplace_back(Term{expr.value, std::set<Factor>{}});
  // buffer.emplace_back(val_t{expr.value});
}

void RpnVisitor::operator()(const Var& expr) {
  terms.emplace_back(Term{1, std::set<Factor>{expr.value}});
  // buffer.emplace_back(val_t{expr.value});
}