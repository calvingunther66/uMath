#pragma once
#include "AST.h"
#include <cctype>
#include <cstdlib>
#include <cstring>

namespace uMath {

// Simple fixed-size stack to avoid std::stack/std::vector
template <typename T, size_t Capacity> class FixedStack {
  T data[Capacity];
  size_t ptr = 0;

public:
  void push(T val) {
    if (ptr < Capacity)
      data[ptr++] = val;
  }
  T pop() { return (ptr > 0) ? data[--ptr] : T{}; }
  T top() const { return (ptr > 0) ? data[ptr - 1] : T{}; }
  bool empty() const { return ptr == 0; }
  size_t size() const { return ptr; }
};

class Parser {
public:
  // Parse C-string into AST using NodeArena
  static NodeID parse(const char *expression);

private:
  static int get_precedence(char op);
  static bool is_operator(char c);
};

} // namespace uMath
