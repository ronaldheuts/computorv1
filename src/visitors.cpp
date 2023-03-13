#include "visitors.h"

/* Visitors */

PrintVisitor::PrintVisitor() : height{0}, prefix{} {}

void PrintVisitor::operator()(const BinaryExpr& expr) {
  height += 2;
  std::visit(*this, *(expr.left));
  height -= 2;
  std::cout << std::string(height, ' ') << static_cast<char>(expr.oper) << '\n';
  height += 2;
  std::visit(*this, *(expr.right));
  height -= 2;
}

void PrintVisitor::operator()(const UnaryExpr& expr) {
  std::cout << std::string(height, ' ') << static_cast<char>(expr.oper) << '\n';
  height += 2;
  std::visit(*this, *(expr.child));
}

void PrintVisitor::operator()(const Var& expr) {
  std::cout << std::string(height, ' ') << expr.value << '\n';
}

void PrintVisitor::operator()(const Num& expr) {
  std::cout << std::string(height, ' ') << expr.value << '\n';
}

/* RpnVisitor */

// when possible, add or subtract terms
// when not possible, keep a record of operators

/// @brief post-order traversal of the abstract syntax tree;
/// constructs terms.
RpnVisitor::RpnVisitor(void) : terms{}, reduced{} {}

void RpnVisitor::evaluate(const BinaryExpr& expr) {
  assert(terms.size() >= 2);
  Term rhs{terms.back()};
  terms.pop_back();
  Term lhs{terms.back()};
  terms.pop_back();

  switch (expr.oper) {
    case Token::Kind::MINUS:
    case Token::Kind::EQUAL:
      rhs.coe *= -1;
    case Token::Kind::PLUS:
      terms.emplace_back(rhs);
      terms.emplace_back(lhs);
      break;
    case Token::Kind::ASTERISK:
      terms.emplace_back(lhs * rhs);
      break;
    case Token::Kind::SLASH:
      terms.emplace_back(lhs / rhs);
      break;
    case Token::Kind::CARET:
      if (isConstant(lhs)) {
        terms.emplace_back(Term{utils::exponentiation(lhs.coe, rhs.coe)});
      } else {
        terms.emplace_back(Term{lhs.coe, lhs.var, static_cast<int>(rhs.coe)});
      }
      break;
  }
}

void RpnVisitor::evaluate(const UnaryExpr& expr) {
  Term rhs{terms.back()};
  terms.pop_back();

  switch (expr.oper) {
    case Token::Kind::MINUS:
      terms.emplace_back(Term{(rhs.coe * -1), rhs.var, rhs.exp});
      break;
    case Token::Kind::PLUS:  // maybe refuse this
      terms.emplace_back(rhs);
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
  terms.emplace_back(Term{expr.value});
}

void RpnVisitor::operator()(const Var& expr) {
  terms.emplace_back(Term{1, expr.value});
}
