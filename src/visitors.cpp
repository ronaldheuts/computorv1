#include "visitors.h"

PrettyPrint::PrettyPrint(int levels)
    : levels(levels), width(utils::exponentiation(2, levels + 1)), level(1) {}

void PrettyPrint::operator()(const BinaryExpr& expr) {
  int offset = width / utils::exponentiation(2, level);

  std::visit(*this, *expr.left);
  std::visit(*this, *expr.right);
}

void PrettyPrint::operator()(const UnaryExpr& expr) {}

void PrettyPrint::operator()(const Term& expr) {}

void PrettyPrint::buildQueue() {
  while (!q.empty()) {
    QueueElem el = q.front();
  }
}

/* Visitors */

PrintVisitor::PrintVisitor() : height{0}, prefix{}, levels{0} {}

void PrintVisitor::operator()(const BinaryExpr& expr) {
  height += 1;
  std::visit(*this, *(expr.left));
  height -= 1;
  std::cout << std::string(height, ' ') << static_cast<char>(expr.oper) << '\n';
  height += 1;
  std::visit(*this, *(expr.right));
  height -= 1;
}

void PrintVisitor::operator()(const UnaryExpr& expr) {
  std::cout << std::string(height, ' ') << static_cast<char>(expr.oper) << '\n';
  height += 1;
  std::visit(*this, *(expr.child));
  height -= 1;
}

void PrintVisitor::operator()(const Term& expr) {
  levels = std::max(levels, height);
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
  expr.setCoe(expr > 0 ? -expr.getCoe() : expr.getCoe());
}

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
