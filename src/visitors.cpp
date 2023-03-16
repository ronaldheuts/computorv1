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

/* PostFixPrintVisitor */

void PostFixPrintVisitor::operator()(const BinaryExpr& expr) {
  std::visit(*this, *expr.left);
  std::visit(*this, *expr.right);
  std::cout << static_cast<char>(expr.oper) << " ";
}

void PostFixPrintVisitor::operator()(const UnaryExpr& expr) {
  std::visit(*this, *expr.child);
  std::cout << static_cast<char>(expr.oper) << " ";
}

void PostFixPrintVisitor::operator()(const Term& expr) {
  std::cout << "[" << expr << "] ";
}

void PostFixPrintVisitor::operator()(const Var& expr) {
  std::cout << expr.value;
}

void PostFixPrintVisitor::operator()(const Num& expr) {
  std::cout << expr.value;
}

/* RpnVisitor */

// when possible, add or subtract terms
// when not possible, keep a record of operators

/// @brief post-order traversal of the abstract syntax tree;
RpnVisitor::RpnVisitor(void) : terms{}, reduced{} {}

void RpnVisitor::operator()(const BinaryExpr& expr) {
  std::visit(*this, *(expr.left));
  std::visit(*this, *(expr.right));
}

void RpnVisitor::operator()(const UnaryExpr& expr) {
  std::visit(*this, *(expr.child));
}

void RpnVisitor::operator()(const Term& expr) { terms.emplace_back(expr); }

void RpnVisitor::operator()(const Num& expr) {
  terms.emplace_back(Term{expr.value});
}

void RpnVisitor::operator()(const Var& expr) {
  terms.emplace_back(Term{1, expr.value});
}
