#include "parser.h"

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
*/

// clang-format on

/* Nodes */

BinaryExpr::BinaryExpr(Token::Kind k, std::unique_ptr<node_t>& l,
                       std::unique_ptr<node_t>& r)
    : oper{k}, left{std::move(l)}, right{std::move(r)} {}

UnaryExpr::UnaryExpr(Token::Kind k, std::unique_ptr<node_t>& c)
    : oper{k}, child{std::move(c)} {}

/* Tree */

void Tree::setRoot(std::unique_ptr<node_t>& r) { root = std::move(r); }

/* Parser */

Parser::Parser() : lexer{} {}

void Parser::stream(const std::string& s) { lexer.stream(s); }

Token Parser::peek() { return lexer.peek(); }

Token Parser::advance() { return lexer.get(); }

void Parser::putback(Token token) { lexer.putback(token); }

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

std::unique_ptr<Parser::node_t> Parser::term(void) {
  Term expr{};

  if (check(peek().kind, Token::Kind::NUMBER)) {
    expr.coe = std::get<double>(advance().value);
  }

  if (!expr.coe) {
    return std::make_unique<node_t>(expr);
  }

  if (check(peek().kind, Token::Kind::ASTERISK)) {
    advance();
  } else {
    throw(std::invalid_argument("missing asterisk"));
  }

  if (check(peek().kind, Token::Kind::VARIABLE)) {
    expr.var = std::get<char>(advance().value);
  } else {
    throw(std::invalid_argument("missing variable"));
  }

  if (check(peek().kind, Token::Kind::CARET)) {
    advance();
  } else {
    throw(std::invalid_argument("missing caret"));
  }

  if (check(peek().kind, Token::Kind::NUMBER)) {
    expr.exp = std::get<double>(advance().value);
  } else {
    throw(std::invalid_argument("missing exponent"));
  }

  return std::make_unique<node_t>(expr);
}

std::unique_ptr<Parser::node_t> Parser::unary(void) {
  if (check(peek(), Token::Kind::MINUS)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> expr = unary();
    return std::make_unique<node_t>(UnaryExpr{current, expr});
  }
  return term();
}

std::unique_ptr<Parser::node_t> Parser::power(void) {
  std::unique_ptr<node_t> expr = unary();

  while (check(peek(), Token::Kind::CARET)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = term();
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
    if (!check(peek(), Token::Kind::END)) {          // todo cleanup
      throw(std::runtime_error("invalid grammar"));  // todo cleanup
    }                                                // todo cleanup
    return std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  if (!check(peek(), Token::Kind::END)) {
    throw(std::runtime_error("invalid grammar"));
  }
  return expr;
}

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

void Parser::parse(void) {
  std::unique_ptr<node_t> root = equation();
  std::visit(PrintVisitor{}, *root);
  std::visit(PostFixPrintVisitor{}, *root);
  std::cout << '\n';

  RpnVisitor rpn{};
  std::visit(rpn, *root);
  printTerms(rpn.terms);

  std::cout << "reduced (size: " << rpn.reduced.size() << "):\n";
  for (auto& x : rpn.reduced) {
    std::cout << "[ " << x.second.coe;
    if (x.second.var) {
      std::cout << " * " << x.second.var << " ^ " << x.second.exp;
    }
    std::cout << " ]\n";
  }
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
