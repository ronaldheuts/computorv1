#include "../include/parser.h"

/*

A parser really has two jobs:

1. Given a valid sequence of tokens, produce a corresponding syntax tree.

2. Given an invalid sequence of tokens, detect any errors and tell the user
about their mistakes.

*/

// clang-format off
/*

https://gist.github.com/wolfsage/3996187
https://rosettacode.org/wiki/Parsing/RPN_calculator_algorithm#C++

<equation> ::= <expression> "=" <expression> | <expression>

<expression> ::= <expression> "-" <factor> | <expression> "+" <factor> | <factor>

<factor> ::= <factor> "*" <power> | <factor> "/" <power> | <power>

<power> ::= <unary> "^" <int> | <unary>

<unary> ::= "-" <primary> | <primary>

<primary> ::= "(" <expression> ")" | <num> | <var>

<num> ::= <int> | <float>

<float> ::= <int> "." <int> | <int> "."

<int> ::= <int> <digit> | <digit>

<var> ::= #'[a-z]'+ | #'[A-Z]'+

<digit> ::= #'[0-9]'+

*/
// clang-format on

/* Nodes */

BinaryExpr::BinaryExpr(Token::Kind k, std::unique_ptr<node_t>& l,
                       std::unique_ptr<node_t>& r)
    : oper{k}, left{std::move(l)}, right{std::move(r)} {}

UnaryExpr::UnaryExpr(Token::Kind k, std::unique_ptr<node_t>& c)
    : oper{k}, child{std::move(c)} {}

/* Tree */

void Tree::root(std::unique_ptr<node_t>& r) { m_root = std::move(r); }

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

double powerof(double base, int exponent) {
  double result{base};

  for (int i = 0; i < exponent - 1; ++i) {
    result *= base;
  }
  return result;
}

CalcVisitor::val_t CalcVisitor::operator()(const BinaryExpr& expr) const {
  val_t left = std::visit(*this, *(expr.left));
  val_t right = std::visit(*this, *(expr.right));

  if (std::holds_alternative<double>(left) &&
      std::holds_alternative<double>(right)) {
    switch (expr.oper) {
      case Token::Kind::PLUS:
        return std::get<double>(left) + std::get<double>(right);
      case Token::Kind::MINUS:
        return std::get<double>(left) - std::get<double>(right);
      case Token::Kind::ASTERISK:
        return std::get<double>(left) * std::get<double>(right);
      case Token::Kind::SLASH:
        return std::get<double>(left) / std::get<double>(right);
      case Token::Kind::CARET:
        return powerof(std::get<double>(left),
                       static_cast<int>(std::get<double>(right)));
    }
  }

  return val_t{};
}

CalcVisitor::val_t CalcVisitor::operator()(const UnaryExpr& expr) const {
  return -(std::get<double>(std::visit(*this, *(expr.child))));
}

CalcVisitor::val_t CalcVisitor::operator()(const Var& expr) const {
  return expr.value;
}

CalcVisitor::val_t CalcVisitor::operator()(const Num& expr) const {
  return expr.value;
}

/* Term */

Term::Term(const std::string& v) : coefficient{1}, variable{v}, exponent{1} {}

Term::Term(const double c, const std::string& v)
    : coefficient{c}, variable{v}, exponent{1} {}

Term::Term(const double c, const std::string v, const int e)
    : coefficient{c}, variable{v}, exponent{e} {}

bool likeTerms(const Term& a, const Term& b) {
  return a.variable == b.variable && a.exponent == b.exponent;
}

bool unlikeTerms(const Term& a, const Term& b) { return !(likeTerms(a, b)); }

bool isConstant(const Term& a) { return a.variable.empty(); }

/*
2 * X = 2X
2X * X = 2X^2
2X * 2X = 4X^2
2X * Y = 2XY
2X * 2Y = 4XY
*/

RpnVisitor::RpnVisitor(void) : operands{}, terms{} {}

Term operator+(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coefficient + b.coefficient, a.variable, a.exponent};
  }
  throw(std::runtime_error("can not add unlike terms"));
}

Term operator-(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coefficient - b.coefficient, a.variable, a.exponent};
  }
  throw(std::runtime_error("can not subtract unlike terms"));
}

Term operator*(const Term& a, const Term& b) {
  if (likeTerms(a, b)) {
    return Term{a.coefficient * b.coefficient, a.variable,
                a.exponent + b.exponent};
  } else if (isConstant(a)) {
    return Term{a.coefficient * b.coefficient, b.variable, b.exponent};
  } else if (isConstant(b)) {
    return Term{a.coefficient * b.coefficient, a.variable, a.exponent};
  }
  throw(std::runtime_error("can not factor unlike terms - YET"));
}

/* RpnVisitor */

// "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
// "5 * X^0 + 4 * X^1 = 4 * X^0"
// "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"

void RpnVisitor::evaluate(const BinaryExpr& expr) {
  Term a{terms.back()};
  terms.pop_back();
  Term b{terms.back()};
  terms.pop_back();

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
      terms.emplace_back(
          Term{b.coefficient, b.variable, static_cast<int>(a.coefficient)});
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
      terms.emplace_back(Term{-a.coefficient, a.variable, a.exponent});
      break;
    case Token::Kind::PLUS:
      terms.emplace_back(a);
      break;
  }
}

void printTerms(std::vector<Term>& terms) {
  std::cout << "number of terms: " << terms.size() << "\n";
  for (auto& x : terms) {
    std::cout << "[ " << x.coefficient;
    if (!x.variable.empty())
      std::cout << " * " << x.variable << " ^ " << x.exponent;
    std::cout << " ]\n";
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
  terms.emplace_back(Term{expr.value, {}, 1});
}

void RpnVisitor::operator()(const Var& expr) {
  terms.emplace_back(Term{1, expr.value, 1});
}

/* Parser */

Parser::Parser() : m_lexer{} {}

void Parser::stream(const std::string& s) { m_lexer.stream(s); }

Token Parser::peek(void) { return m_lexer.peek(); }

Token Parser::advance(void) { return m_lexer.get(); }

bool Parser::check(const Token& token, Token::Kind kind) {
  return token.kind == kind;
}

bool Parser::match(const Token& token, Token::Kind kind) {
  if (check(token, kind)) {
    advance();
    return true;
  }
  return false;
}

std::unique_ptr<Parser::node_t> Parser::primary(void) {
  switch (peek().kind) {
    case Token::Kind::NUMBER:
      return std::make_unique<node_t>(Num{std::get<double>(advance().value)});
    case Token::Kind::VARIABLE: {
      return std::make_unique<node_t>(
          Var{std::get<std::string>(advance().value)});
    }
    default:
      throw(std::runtime_error("invalid grammar"));
  }
}

std::unique_ptr<Parser::node_t> Parser::unary(void) {
  if (check(peek(), Token::Kind::MINUS)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> expr = unary();
    return std::make_unique<node_t>(UnaryExpr{current, expr});
  }
  return primary();
}

std::unique_ptr<Parser::node_t> Parser::power(void) {
  std::unique_ptr<node_t> expr = unary();

  while (check(peek(), Token::Kind::CARET)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = primary();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }

  return expr;
}

std::unique_ptr<Parser::node_t> Parser::factor(void) {
  std::unique_ptr<node_t> expr = power();

  while (check(peek(), Token::Kind::ASTERISK) ||
         check(peek(), Token::Kind::SLASH)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = power();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

std::unique_ptr<Parser::node_t> Parser::expression(void) {
  std::unique_ptr<node_t> expr = factor();

  while (check(peek(), Token::Kind::PLUS) ||
         check(peek(), Token::Kind::MINUS)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = factor();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

std::unique_ptr<Parser::node_t> Parser::equation(void) {
  std::unique_ptr<node_t> expr = expression();

  if (check(peek(), Token::Kind::EQUAL)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = expression();
    return std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  if (!check(peek(), Token::Kind::END)) {
    throw(std::runtime_error("invalid grammar"));
  }
  return expr;
}

bool solvable(const std::vector<Term>& terms) {
  constexpr int max_degree = 2;

  for (const auto& x : terms) {
    if (x.exponent > max_degree) {
      return false;
    }
  }
  return true;
}

void reduceTerms(const std::vector<Term>& terms) {
  std::map<std::pair<std::string, int>, Term> polynomial;
  for (auto& x : terms) {
    auto it = polynomial.find(std::make_pair(x.variable, x.exponent));
    if (it != polynomial.end()) {
      it->second.coefficient += x.coefficient;
    } else {
      polynomial.insert(
          std::make_pair(std::make_pair(x.variable, x.exponent), x));
    }
  }
  for (auto& x : polynomial) {
    std::cout << x.second.coefficient << " ";
    if (!isConstant(x.second))
      std::cout << "* " << x.second.variable << "^" << x.second.exponent << " ";
  }
  std::cout << "= 0\n";
}

void Parser::parse(void) {
  std::unique_ptr<node_t> root = equation();
  std::visit(PrintVisitor{}, *root);
  std::cout << '\n';

  RpnVisitor rpn{};
  std::visit(rpn, *root);

  printTerms(rpn.terms);
  if (!solvable(rpn.terms)) {
    throw(std::runtime_error(
        "The polynomial degree is strictly greater than 2, I can't solve."));
  }
  reduceTerms(rpn.terms);
}

std::string Parser::prompt(void) {
  std::string equation;
  constexpr std::string_view msg{
      "Enter a quadratic equation (a * X^2 + b * X^1 + c * X^0 = 0): "};

  /* error handling around cin */
  std::cout << msg;
  std::getline(std::cin, equation);

  return equation;
}