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
  expr.setCoe(expr.getCoe() > 0 ? -expr.getCoe() : expr.getCoe());
}

void TransposeVisitor::operator()(Var& expr) {}

void TransposeVisitor::operator()(Num& expr) {}

/* RpnVisitor */

/// @brief post-order traversal of the abstract syntax tree;
RpnVisitor::RpnVisitor(void) : terms{} {}

void RpnVisitor::evaluate(const BinaryExpr& expr, Term term) {
  if (expr.oper == Token::Kind::kMinus) {
    term = -term;
  }
  const auto [it, success] = terms.insert(
      std::make_pair(std::make_pair(term.getVar(), term.getExp()), term));
  if (!success) {
    it->second += term;
  }
  if (!it->second) {
    terms.erase(it);
  }
}

Term RpnVisitor::operator()(const BinaryExpr& expr) {
  Term lhs = std::visit(*this, *(expr.left));
  Term rhs = std::visit(*this, *(expr.right));

  if (expr.oper == Token::Kind::kMinus) {
    rhs = -rhs;
  }
  const auto [it, success] = terms.insert(
      std::make_pair(std::make_pair(rhs.getVar(), rhs.getExp()), rhs));

  if (!success) {
    it->second += rhs;
  }
  if (!it->second) {
    terms.erase(it);
  }
  return lhs;
}

Term RpnVisitor::operator()(const UnaryExpr& expr) {
  switch (expr.oper) {
    case Token::Kind::kMinus:
      return -std::visit(*this, *(expr.child));
    case Token::Kind::kPlus:  // maybe remove
      return std::visit(*this, *(expr.child));
  }
}

Term RpnVisitor::operator()(const Term& expr) { return expr; }

Term RpnVisitor::operator()(const Num& expr) { return Term{expr.value}; }

Term RpnVisitor::operator()(const Var& expr) { return Term{1, expr.value}; }
