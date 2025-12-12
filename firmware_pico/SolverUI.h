#pragma once
#include "../core_math/FormulaBank.h"
#include "Display.h"
#include "Input.h"
#include <string>
#include <vector>

namespace uMath {

struct VariableState {
  char symbol;
  std::string value_str;
  bool is_unknown;
};

class SolverUI {
public:
  SolverUI();

  // Setup for a specific formula
  void loadFormula(const Formula *f);

  // Main Loop hooks
  void update(InputManager &input_mgr);
  void render(DisplayBuffer &display);

  // Helpers
  bool isSolving() const { return solving; }
  std::string getSolutionCommand() const { return solution_command; }
  void reset();

private:
  const Formula *current_formula = nullptr;
  std::vector<VariableState> variables;
  int selected_row = 0; // Index in variables list
  bool solving = false;
  std::string solution_command;

  // Logic
  void extractVariables(const char *eqn);
  void handleInput(EventType e);
  // Helper to inject char into current field
  void handleCharInput(char c);

public:
  // Allow main to inject keyboard chars directly for this phase
  void onChar(char c);
};

} // namespace uMath
