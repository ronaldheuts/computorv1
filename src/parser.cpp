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
https://mdkrajnak.github.io/ebnftest/

<equation> ::= <expression> "=" <expression> | <expression>
<expression> ::= <expression> "-" <factor> | <expression> "+" <factor> | <factor>
<factor> ::= <factor> "*" <power> | <factor> "/" <power> | <power>
<power> ::= <power> "^" <int> | <unary>
<unary> ::= "-" <term> | <term>
<term> ::= <num> "*" <var> "^" <int> | <num>
<num> ::= <int> | <float>
<float> ::= <int> "." <int> | <int> "."
<int> ::= <int> <digit> | <digit>
<var> ::= #'[a-z]'+ | #'[A-Z]'+
<digit> ::= #'[0-9]'+

==========================

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

double powerof(double base, int exponent) {
  double result{base};

  for (int i = 0; i < exponent - 1; ++i) {
    result *= base;
  }
  return result;
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
      return std::make_unique<node_t>(Var{std::get<char>(advance().value)});
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

/* bool solvable(const std::vector<Term>& terms) {
  constexpr int max_degree = 2;

  for (const auto& x : terms) {
    if (x.exponent > max_degree) {
      return false;
    }
  }
  return true;
} */

/* void reduceTerms(const std::vector<Term>& terms) {
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
} */

void printTerms(std::vector<Term>& terms) {
  std::cout << "number of terms: " << terms.size() << "\n";
  for (auto& x : terms) {
    std::cout << "[ " << x.coefficient;
    if (!x.vars.empty()) {
      for (const auto& i : x.vars) {
        std::cout << " * " << i.variable << " ^ " << i.exponent << " ";
      }
    }
    std::cout << " ]\n";
  }
}

void Parser::parse(void) {
  std::unique_ptr<node_t> root = equation();
  std::visit(PrintVisitor{}, *root);
  std::cout << '\n';

  RpnVisitor rpn{};
  std::visit(rpn, *root);

  printTerms(rpn.terms);

  /*   if (!solvable(rpn.terms)) {
      throw(std::runtime_error(
          "The polynomial degree is strictly greater than 2, I can't solve."));
    } */
  // reduceTerms(rpn.terms);
}

std::string Parser::prompt(void) {
  std::string equation;
  constexpr std::string_view msg{
      "Enter a quadratic equation (a * X^2 + b * X^1 + c * X^0 = 0): "};

  /* to do: error handling around cin */
  std::cout << msg;
  std::getline(std::cin, equation);

  return equation;
}
