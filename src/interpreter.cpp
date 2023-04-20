#include "interpreter.h"

/* Helper functions */

void printTerms(const std::vector<Term>& terms) {
  std::cout << "number of terms: " << terms.size() << "\n";
  for (const auto& term : terms) {
    std::cout << "[ " << term.getCoe();
    if (term.getVar()) {
      std::cout << " * " << term.getVar() << " ^ " << term.getExp();
    }
    std::cout << " ]\n";
  }
}

int getDegree(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw std::invalid_argument("no terms provided");
  }
  int highest{0};

  for (const auto& term : terms) {
    if (term.second.getExp() > highest) highest = term.second.getExp();
  }
  return highest;
}

bool sameVars(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw std::invalid_argument("no terms provided");
  }
  const auto check = terms.rbegin()->second;

  if (!isConstant(check)) {
    for (const auto& term : terms) {
      if (!isConstant(term.second) && !sameVars(term.second, check)) {
        return false;
      }
    }
  }
  return true;
}

bool validDegree(const std::map<std::pair<char, int>, Term>& terms) {
  constexpr int max_degree = 2;
  constexpr int min_degree = 0;

  int degree = getDegree(terms);
  if (degree > max_degree || min_degree > degree) {
    return false;
  }
  return true;
}

bool solvable(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw std::invalid_argument("no terms provided");
  }
  if (!validDegree(terms)) {
    throw std::invalid_argument(
        "can not solve equation with a degree higher than 2");
  }
  if (!sameVars(terms)) {
    throw std::invalid_argument(
        "can not solve equation with different variables");
  }
  return true;
}

void printReducedForm(const std::map<std::pair<char, int>, Term>& terms) {
  if (terms.empty()) {
    throw std::invalid_argument("no terms provided");
  }

  std::cout << "Reduced form: ";
  for (auto it = terms.begin(); it != terms.end(); ++it) {
    if (it == terms.begin()) {
      std::cout << it->second << " ";
    } else if (it->second > 0) {
      std::cout << "+ " << it->second << " ";
    } else if (it->second < 0) {
      std::cout << "- " << -(it->second) << " ";
    }
  }
  std::cout << "= 0\n";
}

/* Interpreter */

Interpreter::Interpreter(Tree& t) : tree{} {
  tree.setRoot(std::move(t.getRoot()));
}

/// @brief move quantities from the right hand side of the equation across
void Interpreter::transpose() {
  if (!std::holds_alternative<BinaryExpr>(*tree.getRoot())) {
    throw std::invalid_argument("expression is not an equation");
  }
  std::visit(TransposeVisitor{}, *std::get<BinaryExpr>(*tree.getRoot()).right);
}

Interpreter::solutions_t Interpreter::getSolutions() const { return solutions; }

/// @brief if present, variable is at last element of the map (rbegin)
char Interpreter::findVar() const {
  const auto found = rpn.terms.rbegin();

  if (found != rpn.terms.rend()) {
    return found->second.getVar();
  }
  return 0;
}

/// @brief find the coefficient of the corresponding variable and exponent
double Interpreter::findCoef(const char var, const int exp) const {
  const auto found = rpn.terms.find(std::make_pair(var, exp));

  if (found != rpn.terms.end()) {
    return found->second.getCoe();
  }
  return 0;
}

/// @brief evaluate the equation
void Interpreter::evaluate() {
  constexpr int exponent_two = 2;
  constexpr int exponent_one = 1;
  constexpr int exponent_none = 0;

  transpose();
  if (!std::holds_alternative<BinaryExpr>(*tree.getRoot())) {
    throw grammarError("expression is not an equation (ex. 42 * X^2 = 0)");
  }

  rpn.evaluate((std::get<BinaryExpr>(*tree.getRoot())),
               std::visit(rpn, *tree.getRoot()));

  if (rpn.terms.empty()) {
    std::cout << "The solution is:\nAll real numbers\n";
    return;
  }

  printReducedForm(rpn.terms);
  std::cout << "Polynomial degree: " << getDegree(rpn.terms) << '\n';
  solvable(rpn.terms);

  char   var = findVar();
  double a = findCoef(var, exponent_two);
  double b = findCoef(var, exponent_one);
  double c = findCoef(var, exponent_none);

  if (!a) {
    solutions.emplace_back(utils::linear_equation_solver(b, c));
  } else {
    solutions = utils::quadratic_equation_solver(a, b, c);
  }
  if (solutions.empty()) {
    throw std::runtime_error("no solution available\n");
  } else if (solutions.size() == 1) {
    std::cout << "The solution is:\n";
  } else if (solutions.size() == 2) {
    std::cout << "The solutions are:\n";
  }
  for (int i = 0; i < solutions.size(); ++i) {
    std::visit(utils::ComplexVisitor{}, solutions.at(i));
  }
}
