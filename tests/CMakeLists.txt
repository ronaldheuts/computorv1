cmake_minimum_required(VERSION 3.16)

project(computorv1)

add_executable(computorv1_tests utils_tests.cpp)

target_sources(computorv1_tests PUBLIC
  ../src/lexer.cpp
  ../src/interpreter.cpp
  ../src/parser.cpp
  ../src/token.cpp
  ../src/tree.cpp
  ../src/term.cpp
  ../src/visitors.cpp
  ../src/utils.cpp)

include_directories(../include)

target_link_libraries(computorv1_tests compile_flags)

target_link_libraries(computorv1_tests GTest::gtest_main)

include(GoogleTest)

gtest_discover_tests(computorv1_tests)
