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

/* Parser */

Parser::Parser() : lexer{} {}

Parser::Parser(const std::string& s) { lexer.stream(s); }

void Parser::stream(const std::string& s) { lexer.stream(s); }

Token Parser::peek() { return lexer.peek(); }

Token Parser::advance() { return lexer.get(); }

bool Parser::check(const Token& token, Token::Kind kind) {
  return token.kind == kind;
}

/* "[num] * [char] ^ [num]" OR "0" AND end of equation */
std::unique_ptr<Parser::node_t> Parser::term(void) {
  Term expr{};

  if (check(peek().kind, Token::Kind::kNumber)) {
    expr.setCoe(std::get<double>(advance().value));
  } else {
    throw grammarError("missing number in term (ex. \"42\" * X^2)");
  }
  if (!expr.getCoe() && check(peek().kind, Token::Kind::kEnd)) {
    return std::make_unique<node_t>(expr);
  }
  if (check(peek().kind, Token::Kind::kAsterisk)) {
    advance();
  } else {
    throw grammarError("missing asterisk in term (ex. 42 \"*\" X^2)");
  }
  if (check(peek().kind, Token::Kind::kVariable)) {
    expr.setVar(std::get<char>(advance().value));
  } else {
    throw grammarError("missing variable in term (ex. 42 * \"X\"^2)");
  }
  if (check(peek().kind, Token::Kind::kCaret)) {
    advance();
  } else {
    throw grammarError("missing caret in term (ex. 42 * X\"^\"2)");
  }
  if (check(peek().kind, Token::Kind::kNumber)) {
    expr.setExp(std::get<double>(advance().value));
  } else {
    throw grammarError("missing exponent in term (ex. 42 * X^\"2\")");
  }
  return std::make_unique<node_t>(expr);
}

std::unique_ptr<Parser::node_t> Parser::unary(void) {
  if (check(peek(), Token::Kind::kMinus)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> expr = unary();
    return std::make_unique<node_t>(UnaryExpr{current, expr});
  }
  return term();
}

std::unique_ptr<Parser::node_t> Parser::power(void) {
  std::unique_ptr<node_t> expr = unary();

  while (check(peek(), Token::Kind::kCaret)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = term();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

std::unique_ptr<Parser::node_t> Parser::factor(void) {
  std::unique_ptr<node_t> expr = power();

  while (check(peek(), Token::Kind::kAsterisk) ||
         check(peek(), Token::Kind::kSlash)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = power();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

std::unique_ptr<Parser::node_t> Parser::expression(void) {
  std::unique_ptr<node_t> expr = factor();

  while (check(peek(), Token::Kind::kPlus) ||
         check(peek(), Token::Kind::kMinus)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = factor();
    expr = std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

std::unique_ptr<Parser::node_t> Parser::equation(void) {
  std::unique_ptr<node_t> expr = expression();

  if (check(peek(), Token::Kind::kEqual)) {
    Token::Kind current = peek().kind;
    advance();
    std::unique_ptr<node_t> rhs = expression();
    if (!check(peek(), Token::Kind::kEnd)) {
      throw grammarError("missing end of equation token");
    }
    return std::make_unique<node_t>(BinaryExpr{current, expr, rhs});
  }
  return expr;
}

/// @brief Consume tokens from lexer and build AST.
void Parser::parse() { tree.setRoot(equation()); }

Tree& Parser::getTree() { return tree; }

std::string Parser::prompt(void) {
  constexpr std::string_view msg{"Enter a quadratic equation or 'q' to quit: "};

  std::string equation;

  /* to do: error handling around cin */
  std::cout << msg;
  std::getline(std::cin, equation);

  return equation;
}
