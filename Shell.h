#pragma once
#include "Kernel.h"
#include "Parser.h"
#include "Serial.h"
#include <string>
#include <vector>

namespace uMath {

class Shell {
public:
  Shell() {
    buffer.reserve(64);
    // Prompt
    Serial::print("uMath> ");
  }

  void onChar(char c) {
    if (c == '\n' || c == '\r') {
      processLine();
      Serial::print("uMath> ");
    } else if (c == 0x08 || c == 0x7F) { // Backspace
      if (!buffer.empty()) {
        buffer.pop_back();
        // Naive backspace echo handling (depends on terminal)
        // Serial::print("\b \b");
        Serial::print("[BS]");
      }
    } else {
      buffer += c;
      Serial::print_char(c); // Echo
    }
  }

private:
  std::string buffer;

  void processLine() {
    Serial::print("\n"); // Newline
    if (buffer.empty())
      return;

    // Reset arena
    NodeArena::reset();

    Serial::print("[CMD] Parsing...\n");
    NodeID root = Parser::parse(buffer.c_str());

    if (root == NULL_NODE) {
      Serial::print("[ERR] Parse failed.\n");
    } else {
      // Analyze
      auto res = Kernel::analyze(root);
      bool handled = false;

      // Eval
      float val = Kernel::evaluate(root);
      char buf[32];
      std::snprintf(buf, sizeof(buf), "[VAL] %.4f", val);
      Serial::print(buf);
      Serial::print("\n");

      // Check for solver ops
      for (int i = 0; i < res.count; i++) {
        if (res.ops[i] == OperationType::SOLVE_ROOTS) {
          Serial::print("[OP] Solving Quadratic...\n");
          // Mock Poly extraction for demo
          Poly p;
          p.setCoeff(2, 1);
          p.setCoeff(1, -5);
          p.setCoeff(0, 6);
          Kernel::solve_quadratic(p);
          handled = true;
        }
      }
    }

    buffer.clear();
  }
};

} // namespace uMath
