#include "token.h"

Token::Token() : value{}, kind{} {}

Token::Token(Token::Kind k, val_t t) : value{t}, kind{k} {}

Token::Token(Token::Kind k) : kind{k} {}
