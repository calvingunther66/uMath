#include "../core_math/FormulaBank.h"
#include "Serial.h"
#include "SolverUI.h" // New
#include <iostream>

using namespace uMath;

void dump_display(const DisplayBuffer &dbuff) {
  std::cout << "\n+--------------------------------+\n";
  for (int i = 0; i < DisplayBuffer::LINES; ++i) {
    std::cout << "| " << dbuff.getLine(i) << " |\n";
  }
  std::cout << "+--------------------------------+\n";
}

int main() {
  Serial::init();

  // Setup UI
  SolverUI solver;
  DisplayBuffer display;
  InputManager input; // Isolated input for this test

  std::cout << "=== uMath Phase 8: Solver UI ===\n";

  // 1. Load Ideal Gas Law
  std::cout << "[Setup] Loading 'Ideal Gas Law'...\n";
  const Formula *f = FormulaBank::findFormula("Ideal");
  if (f)
    solver.loadFormula(f);

  solver.render(display);
  dump_display(display);

  // 2. Simulate User Input:
  // P = 101 (Row 0)
  std::cout << "[Input] Typing '101' into P...\n";
  solver.onChar('1');
  solver.onChar('0');
  solver.onChar('1');
  solver.render(display);
  dump_display(display);

  // Move Down to V
  std::cout << "[Input] Moving Down to V...\n";
  input.pushEvent(EventType::BTN_DOWN);
  solver.update(input);

  // V = ? (Default) -> Leave it

  // Move Down to n
  std::cout << "[Input] Moving to n...\n";
  input.pushEvent(EventType::BTN_DOWN);
  solver.update(input);

  // n = 2.5
  std::cout << "[Input] Typing '2.5' into n...\n";
  solver.onChar('2');
  solver.onChar('.');
  solver.onChar('5');

  // Move Down to R (3) -> T (4) -> Submit (5)
  // Actually we just set Values.
  // Let's Skip to Solve basic flow
  // ...

  // 3. Trigger Solve
  // We need to navigate to bottom or hit OK multiple times?
  // Logic said: if at bottom, OK triggers.
  // Let's force scroll down to bottom.
  // Rows: P(0), V(1), n(2), R(3), T(4).
  // Currently at n(2).
  input.pushEvent(EventType::BTN_DOWN); // R
  input.pushEvent(EventType::BTN_DOWN); // T
  // input.pushEvent(EventType::BTN_DOWN); // End? No.
  solver.update(input);

  // Now at T(4). Press OK to finish/solve?
  std::cout << "[Input] Pressing OK (at T) to Solve...\n";
  input.pushEvent(EventType::BTN_OK); // T -> Solve? logic: if last row -> solve
  // Re-read logic: "if (selected_row < size-1) ++ else solve".
  // Size is 5. Max index 4. So at index 4, OK solves.
  solver.update(input);

  if (solver.isSolving()) {
    std::cout << "\n[Result] Command Generated:\n";
    std::cout << solver.getSolutionCommand() << "\n";
  } else {
    std::cout << "[Error] Solve not triggered.\n";
  }

  return 0;
}
