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

void PrintVisitor::operator()(const Term& expr) {
  std::cout << std::string(height, ' ') << expr << '\n';
}

/* TransposeVisitor */

void TransposeVisitor::operator()(BinaryExpr& expr) {
  std::visit(*this, *expr.left);
  std::visit(*this, *expr.right);
}

void TransposeVisitor::operator()(UnaryExpr& expr) {
  std::visit(*this, *expr.child);
}

void TransposeVisitor::operator()(Term& expr) {
  expr.coe = expr.coe != 0 ? -(expr.coe) : 0;
}

void TransposeVisitor::operator()(Var& expr) {}

void TransposeVisitor::operator()(Num& expr) {}

/* RpnVisitor */

// when possible, add or subtract terms
// when not possible, keep a record of operators

/// @brief post-order traversal of the abstract syntax tree;
RpnVisitor::RpnVisitor(void) : terms{}, reduced{} {}

void RpnVisitor::evaluate(const BinaryExpr& expr, Term term) {
  if (expr.oper == Token::Kind::MINUS) term = -term;
  const auto [it, success] =
      reduced.insert(std::make_pair(std::make_pair(term.var, term.exp), term));
  if (!success) {
    it->second += term;
  }
}

Term RpnVisitor::operator()(const BinaryExpr& expr) {
  Term lhs = std::visit(*this, *(expr.left));
  Term rhs = std::visit(*this, *(expr.right));

  if (expr.oper == Token::Kind::MINUS) rhs = -rhs;
  const auto [it, success] =
      reduced.insert(std::make_pair(std::make_pair(rhs.var, rhs.exp), rhs));
  if (!success) {
    it->second += rhs;
  }
  return lhs;
}

Term RpnVisitor::operator()(const UnaryExpr& expr) {
  switch (expr.oper) {
    case Token::Kind::MINUS:
      return -(std::visit(*this, *(expr.child)));
    case Token::Kind::PLUS:  // maybe remove
      return std::visit(*this, *(expr.child));
  }
}

Term RpnVisitor::operator()(const Term& expr) { return expr; }

Term RpnVisitor::operator()(const Num& expr) { return Term{expr.value}; }

Term RpnVisitor::operator()(const Var& expr) { return Term{1, expr.value}; }
