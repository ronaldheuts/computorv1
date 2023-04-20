#pragma once

#include <stdexcept>
#include <string>

class grammarError : public std::runtime_error {
 public:
  grammarError(const std::string& error) : std::runtime_error{error.c_str()} {}
};
