# computorv1 - Quadratic Equation Calculator
This is a project from the 42 network curriculum. 

## Project description
The goal is to write a program which can solve equations, up to the second degree.

## Personal learning objectives
Getting more familiar with c++ and in particular about the following concepts:

- Designing a grammar tailored to the subject's requirements;
- building a lexer and parser on top of the grammar;
- hooking the resulting Abstract Syntax Tree of the parser up to an interpreter;
- using std::variant and std::visit for runtime polymorphism;
- get more familiar with smart pointers;
- writing tests using googletest framework.

## Installation and usage
```
git clone git@github.com:ronaldheuts/computorv1.git computorv1
cd computorv1
cmake -S . -B build
cmake --build build
```

The subject requires equations to be written in the following form:
```
./computorv1 [equation]

example:
./computorv1 "42 * X^2 - 2 * X^1 + 4 * X^0 = 0"
```
ps. a constant must have a variable, but no exponent (in the above example "4 * X^0").