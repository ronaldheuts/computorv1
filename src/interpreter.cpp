#include "interpreter.h"

/* Helper functions */

void printTerms(const std::vector<Term>& terms) {
  std::cout << "number of terms: " << terms.size() << "\n";
  for (auto& x : terms) {
    std::cout << "[ " << x.coe;
    if (x.var) {
      std::cout << " * " << x.var << " ^ " << x.exp;
    }
    std::cout << " ]\n";
  }
}

int getDegree(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw(std::invalid_argument("no terms provided"));
  }
  int highest{0};

  for (const auto& term : terms) {
    if (term.second.exp > highest) highest = term.second.exp;
  }
  return highest;
}

bool sameVars(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw(std::invalid_argument("no terms provided"));
  }
  const auto check = terms.rbegin()->second;

  if (!isConstant(check)) {
    for (const auto& x : terms) {
      if (!isConstant(x.second) && !likeVars(x.second, check)) {
        return false;
      }
    }
  }
  return true;
}

bool solvable(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw(std::invalid_argument("no terms provided"));
  }
  constexpr int max_degree = 2;
  constexpr int min_degree = 0;

  int degree = getDegree(terms);
  if (degree > max_degree || min_degree > degree) {
    throw(
        std::invalid_argument("can not solve quadratic equation with degree: " +
                              std::to_string(degree)));
  }
  if (!sameVars(terms)) {
    throw(std::invalid_argument(
        "can not solve quadratic equation with unlike variables"));
  }
  return true;
}

void printReducedForm(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw(std::invalid_argument("no terms provided"));
  }

  for (auto it = terms.rbegin(); it != terms.rend(); ++it) {
    if (it == terms.rbegin()) {
      std::cout << it->second << " ";
    } else if (it->second > 0) {
      std::cout << "+ " << it->second << " ";
    } else if (it->second < 0) {
      std::cout << "- " << -(it->second) << " ";
    }
  }
  std::cout << "= 0\n";
}

bool isEquation(Interpreter::node_t& expr) {
  auto result = std::get_if<BinaryExpr>(&expr);

  if (result && result->oper == Token::Kind::kEqual) {
    return true;
  }
  return false;
}

/* Interpreter */

Interpreter::Interpreter(Tree& t) : tree{} { tree.setRoot(std::move(t.root)); }

void Interpreter::transpose() {
  if (!isEquation(tree.getRoot())) {
    throw(std::invalid_argument("not an equation"));
  }
  std::visit(TransposeVisitor{}, *std::get<BinaryExpr>(tree.getRoot()).right);
}

void Interpreter::evaluate() {
  std::visit(PrintVisitor{}, tree.getRoot());

  RpnVisitor rpn{};

  rpn.evaluate((std::get<BinaryExpr>(tree.getRoot())),
               std::visit(rpn, tree.getRoot()));

  std::cout << '\n';

  transpose();

  printReducedForm(rpn.terms);
  std::cout << "degree: " << getDegree(rpn.terms) << '\n';
  std::cout << "terms (size: " << rpn.terms.size() << "):\n";
  for (auto& x : rpn.terms) {
    std::cout << "[ " << x.second << " ]\n";
  }

  std::cout << "solvable: " << std::boolalpha << solvable(rpn.terms) << '\n';

  int a = rpn.terms.find(std::make_pair('X', 2))->second.coe;
  int b = rpn.terms.find(std::make_pair('X', 1))->second.coe;
  int c = rpn.terms.find(std::make_pair('X', 0))->second.coe;

  std::optional<std::vector<double>> solutions =
      utils::quadratic_equation_solver(a, b, c);

  if (!solutions) {
    std::cout << "complex solutions not supported\n";
  } else {
    std::cout << "solutions size: " << solutions->size() << '\n';
    for (const auto& x : *solutions) {
      std::cout << "X = " << x << '\n';
    }
  }
}
