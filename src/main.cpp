#include <fstream>
#include <iostream>

#include "../include/parser.h"

/*
1. check exceptions - are they the right ones
2. be specific with user errors - where did it go wrong
*/

int main(int argc, char *argv[]) try {
  Parser par;

  if (argc == 1) {
    par.stream(par.prompt());
  } else if (argc == 2) {
    par.stream(argv[1]);
  } else {
    throw(std::invalid_argument("usage: ./computorv1 [string]"));
  }
  par.parse();

  return 0;
} catch (std::exception &e) {
  std::cerr << e.what();
  return 1;
} catch (...) {
  std::cerr << std::string_view("unknown error occurred");
  return 2;
}
