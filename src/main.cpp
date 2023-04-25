#include <cmath>

#include "interpreter.h"
#include "parser.h"

int main(int argc, char *argv[]) try {
  Parser par;

  std::cerr << std::fixed << (std::pow(2, 62)) << '\n';
  double lol = utils::exponentiation(2, 63);

  if (argc == 1) {
    par.stream(par.prompt());
  } else if (argc == 2) {
    par.stream(argv[1]);
  } else {
    throw(std::invalid_argument("usage: ./computorv1 [equation]"));
  }
  if (!par.parse()) return 0;

  Interpreter interp(par.getTree());
  interp.evaluate();
  return 0;
} catch (std::exception &e) {
  std::cerr << e.what();
  return 1;
} catch (...) {
  std::cerr << "unexpected error";
  return 2;
}
