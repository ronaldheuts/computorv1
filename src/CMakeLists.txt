cmake_minimum_required(VERSION 3.16)

project(computorv1)

target_sources(computorv1 PUBLIC 
  main.cpp
  lexer.cpp
  parser.cpp
  token.cpp
  term.cpp
  visitors.cpp
)
