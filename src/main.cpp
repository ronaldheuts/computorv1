#include <fstream>
#include <iomanip>
#include <iostream>

#include "interpreter.h"
#include "parser.h"

/*
1. check exceptions - are they the right ones
2. be specific with user errors - where did it go wrong
*/

// "5 * X^0 + 4 * X^1 - 9.3 * X^2 = 1 * X^0"
// "5 * X^0 + 4 * X^1 = 4 * X^0"
// "8 * X^0 - 6 * X^1 + 0 * X^2 - 5.6 * X^3 = 3 * X^0"

int main(int argc, char *argv[]) try {
  Parser par;

  if (argc == 1) {
    par.stream(par.prompt());
  } else if (argc == 2) {
    par.stream(argv[1]);
  } else {
    throw(std::invalid_argument("usage: ./computorv1 [equation]"));
  }
  par.parse();

  Interpreter interp(par.getTree());

  interp.evaluate();
  return 0;
} catch (std::exception &e) {
  std::cerr << e.what();
  return 1;
} catch (...) {
  std::cerr << std::string_view("unknown error occurred");
  return 2;
}
