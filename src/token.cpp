#include "token.h"

Token::Token() : value{std::monostate{}}, kind{} {}

Token::Token(Token::Kind k, val_t t) : value{t}, kind{k} {}

Token::Token(Token::Kind k) : value{std::monostate{}}, kind{k} {}
