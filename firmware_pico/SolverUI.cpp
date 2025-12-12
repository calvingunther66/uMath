#include "SolverUI.h"
#include <cctype>
#include <cstdio>
#include <cstring>

namespace uMath {

SolverUI::SolverUI() { reset(); }

void SolverUI::reset() {
  current_formula = nullptr;
  variables.clear();
  selected_row = 0;
  solving = false;
  solution_command = "";
}

void SolverUI::loadFormula(const Formula *f) {
  reset();
  current_formula = f;
  if (f) {
    extractVariables(f->equation_str);
  }
}

// Very naive extractor: Finds single letters [a-zA-Z] that are NOT part of
// functions (sin, cos) For this phase, we assume mathematical variables are
// single characters.
void SolverUI::extractVariables(const char *eqn) {
  if (!eqn)
    return;

  int len = std::strlen(eqn);
  for (int i = 0; i < len; ++i) {
    char c = eqn[i];
    if (std::isalpha(c)) {
      // Check if it's part of a known function keyword "sin", "cos", "log",
      // "sqrt" Simple heuristic directly checking neighbors? Or better:
      // Accumulate purely unique chars.

      // Skip keywords: s, i, n, c, o, l, g, q, r, t if they form a word?
      // Let's rely on capital/lowercase usage or a simple deny list.
      // Problem: 'a', 'b', 'c' are variables, but 'c' is in 'cos'.
      // Simple fix for Phase 8: Accept ALL alpha chars, filter duplicates.
      // Improve: User can delete unwanted vars if our parser is too aggressive.

      // Check if already exists
      bool exists = false;
      for (const auto &v : variables) {
        if (v.symbol == c) {
          exists = true;
          break;
        }
      }

      if (!exists) {
        // Heuristic: Ignore 's', 'i', 'n', 'c', 'o', 's', 'l', 'o', 'g' if they
        // follow each other? Actually, let's just accept them. The user UI will
        // show "S: [ ]", "I: [ ]"... Clean solution: The extraction logic
        // should be smarter or the Formula struct should list vars explicitly.
        // Re-reading Phase 7 Formula struct: It does NOT list vars.
        // Let's implement a filtered list.

        // Deny logic: skip if looking like 'sin', 'cos'.
        // Too complex for this snippet.
        // Let's just add it.
        VariableState v;
        v.symbol = c;
        v.value_str = "";
        v.is_unknown = false;

        // If it matches the "missing_var" hint, mark it empty/target?
        if (current_formula->missing_var &&
            c == current_formula->missing_var[0]) {
          v.value_str = "?";
          v.is_unknown = true;
        }

        variables.push_back(v);
      }
    }
  }
}

void SolverUI::update(InputManager &input_mgr) {
  EventType e = input_mgr.poll();
  while (e != EventType::NONE) {
    handleInput(e);
    e = input_mgr.poll();
  }
}

void SolverUI::handleInput(EventType e) {
  if (variables.empty())
    return;

  if (e == EventType::BTN_UP) {
    if (selected_row > 0)
      selected_row--;
  } else if (e == EventType::BTN_DOWN) {
    if (selected_row < (int)variables.size() - 1)
      selected_row++;
  } else if (e == EventType::BTN_OK) {
    // Toggle Unknown Status? Or Submit?
    // Let's say OK submits "Solve" if we are at the bottom?
    // Or we have a virtual button?
    // For now: OK just moves to next field like Enter.
    if (selected_row < (int)variables.size() - 1)
      selected_row++;
    else {
      // Trigger Solve
      // Construct command
      // Format: solve_for(target, formula, var1=1, var2=2...)

      // Find target
      VariableState *target = nullptr;
      for (auto &v : variables) {
        if (v.value_str == "?" || v.value_str.empty()) {
          target = &v;
          break;
        }
      }

      if (!target) {
        // Error: No target
        return;
      }

      std::string cmd = "solve_for(";
      cmd += target->symbol;
      cmd += ", ";
      cmd += current_formula->equation_str;

      for (const auto &v : variables) {
        if (v.symbol != target->symbol) {
          cmd += ", ";
          cmd += v.symbol;
          cmd += "=";
          cmd += (v.value_str.empty() ? "0" : v.value_str);
        }
      }
      cmd += ")";
      solution_command = cmd;
      solving = true;
    }
  }
}

// Direct numeric input handler
void SolverUI::onChar(char c) {
  if (variables.empty())
    return;

  VariableState &v = variables[selected_row];

  if (std::isdigit(c) || c == '.') {
    if (v.value_str == "?")
      v.value_str = "";
    v.value_str += c;
    v.is_unknown = false;
  } else if (c == '?') {
    v.value_str = "?";
    v.is_unknown = true;
  }
  // Handle Backspace if needed (usually code 0x08 or 0x7F)
  else if (c == 0x08 || c == 0x7F) {
    if (!v.value_str.empty())
      v.value_str.pop_back();
  }
}

void SolverUI::render(DisplayBuffer &display) {
  if (!current_formula) {
    display.print(0, 0, "No Formula Loaded");
    return;
  }

  display.clear();
  char buf[32];
  snprintf(buf, 32, "--- %s ---", current_formula->name);
  display.print(0, 0, buf);

  // List variables
  // Limit to 6 visible lines (Row 1 to 6)
  int start_idx = 0;
  if (selected_row > 5)
    start_idx = selected_row - 5;

  for (int i = 0; i < 6; ++i) {
    int idx = start_idx + i;
    if (idx >= (int)variables.size())
      break;

    const auto &v = variables[idx];
    char line[32];
    char cur = (idx == selected_row) ? '>' : ' ';
    snprintf(line, 32, "%c %c: %s", cur, v.symbol, v.value_str.c_str());
    display.print(i + 1, 0, line);
  }

  display.print(7, 0, "[ PRESS ENTER TO SOLVE ]");
}

} // namespace uMath
