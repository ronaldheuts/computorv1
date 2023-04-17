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

/// @brief try to insert term into a map, if a liketerm is known, evaluate.
/// @param term to store
void RpnVisitor::addTerm(std::pair<std::pair<char, int>, Term> term) {
  const auto [it, success] = terms.insert(term);
  if (!success) {
    it->second += term.second;
  }
  if (!it->second) {
    terms.erase(it);
  }
}

void RpnVisitor::evaluate(const BinaryExpr& expr, Term term) {
  if (expr.oper == Token::Kind::kMinus) {
    term = -term;
  }
  addTerm(std::make_pair(std::make_pair(term.getVar(), term.getExp()), term));
}

Term RpnVisitor::operator()(const BinaryExpr& expr) {
  Term lhs = std::visit(*this, *(expr.left));
  Term rhs = std::visit(*this, *(expr.right));

  if (expr.oper == Token::Kind::kMinus) {
    rhs = -rhs;
  }
  addTerm(std::make_pair(std::make_pair(rhs.getVar(), rhs.getExp()), rhs));
  return lhs;
}

Term RpnVisitor::operator()(const UnaryExpr& expr) {
  switch (expr.oper) {
    case Token::Kind::kMinus:
      return -std::visit(*this, *(expr.child));
    case Token::Kind::kPlus:
      return std::visit(*this, *(expr.child));
    case Token::Kind::kEnd:
    case Token::Kind::kNumber:
    case Token::Kind::kVariable:
    case Token::Kind::kAsterisk:
    case Token::Kind::kSlash:
    case Token::Kind::kCaret:
    case Token::Kind::kEqual:
      throw std::invalid_argument("Unexpected token error");
  }
}

Term RpnVisitor::operator()(const Term& expr) { return expr; }
