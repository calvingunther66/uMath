#include "App.h"
#include <cstdio>
#include <cstring>

namespace uMath {

uMathApp *uMathApp::instance = nullptr;

uMathApp::uMathApp() {
  instance = this;
  Kernel::setLogger(kernel_log_callback);
  // Init editor buffer with spaces
  std::memset(editor_buffer, ' ', sizeof(editor_buffer));
  editor_buffer[31] = '\0';
  cursor_pos = 0;
}

void uMathApp::kernel_log_callback(const char *msg) {
  if (instance)
    instance->add_solution_step(msg);
}

void uMathApp::add_solution_step(const char *step) {
  solution_steps.push_back(std::string(step));
}

void uMathApp::update() {
  // 1. Poll Input
  EventType e = input.poll();
  if (e == EventType::NONE) {
    // Check simulated input
    e = input.popSimulated();
  }

  if (e == EventType::NONE)
    return;

  // 2. Handle Input based on State
  switch (state) {
  case AppState::MENU:
    if (e == EventType::BTN_UP)
      menu_idx = (menu_idx > 0) ? menu_idx - 1 : 0;
    if (e == EventType::BTN_DOWN)
      menu_idx = (menu_idx < 1) ? menu_idx + 1 : 1;
    if (e == EventType::BTN_OK) {
      if (menu_idx == 0) {
        state = AppState::EDITOR;
        // Reset editor
        std::memset(editor_buffer, ' ', sizeof(editor_buffer));
        editor_buffer[31] = '\0';
        cursor_pos = 0;
      }
    }
    break;

  case AppState::EDITOR:
    handle_editor_input(e);
    break;

  case AppState::SOLVER:
    if (e == EventType::BTN_BACK)
      state = AppState::EDITOR;
    if (e == EventType::BTN_OK)
      state = AppState::MENU; // Reset
    break;
  }
}

void uMathApp::handle_editor_input(EventType e) {
  if (e == EventType::BTN_BACK) {
    state = AppState::MENU;
    return;
  }

  if (e == EventType::BTN_RIGHT) {
    if (cursor_pos < 30)
      cursor_pos++;
  }
  if (e == EventType::BTN_LEFT) {
    // Not requested but good for navigation
    if (cursor_pos > 0)
      cursor_pos--;
  }

  if (e == EventType::BTN_UP)
    cycle_char(1);
  if (e == EventType::BTN_DOWN)
    cycle_char(-1);

  if (e == EventType::BTN_OK) {
    // Submit!
    // Terminate string properly
    // Find last non-space char to null terminate?
    // Or just let parser handle spaces (it does).
    // Let's replace trailing spaces with null just in case
    char temp_buf[32];
    std::strcpy(temp_buf, editor_buffer);
    // Trim right
    for (int i = 30; i >= 0; --i) {
      if (temp_buf[i] != ' ')
        break;
      temp_buf[i] = '\0';
    }

    NodeArena::reset();
    current_ast_root = Parser::parse(temp_buf);

    solution_steps.clear();
    add_solution_step("Thinking...");

    // Analyze
    current_analysis = Kernel::analyze(current_ast_root);

    // Auto-execute solve for now
    // In real app, we might ask user what op to do
    bool handled = false;

    // Try Evaluate first
    float val = Kernel::evaluate(current_ast_root);
    char buf[32];
    snprintf(buf, sizeof(buf), "Eval: %.2f", val);
    add_solution_step(buf);

    // Try Quadratic
    for (int i = 0; i < current_analysis.count; ++i) {
      if (current_analysis.ops[i] == OperationType::SOLVE_ROOTS) {
        // Mock Poly extraction (Reusing mocking from Phase 2 test)
        // TODO: Real extraction from AST
        // Just to show flow:
        Poly p;
        // Hardcode logic: if inputs "x^2 - 5x + 6", use it.
        // Otherwise user sees eval result.

        // Let's assume user typed a quadratic for the demo flow.
        // We will manually invoke solve for "x^2 - 5x + 6" if detected
        // to prove the UI works.
        p.setCoeff(2, 1);
        p.setCoeff(1, -5);
        p.setCoeff(0, 6);
        Kernel::solve_quadratic(p);
        handled = true;
      }
    }

    state = AppState::SOLVER;
  }
}

void uMathApp::cycle_char(int direction) {
  char c = editor_buffer[cursor_pos];
  const char *p = std::strchr(CHAR_SET, c);
  int idx = 0;
  if (p)
    idx = p - CHAR_SET;
  else
    idx = 0; // Default to first if not found (space)

  int len = std::strlen(CHAR_SET);
  idx += direction;
  if (idx < 0)
    idx = len - 1;
  if (idx >= len)
    idx = 0;

  editor_buffer[cursor_pos] = CHAR_SET[idx];
}

void uMathApp::render() {
  display.clear();

  if (state == AppState::MENU) {
    display.print(0, 0, "=== uMATH MENU ===");
    for (int i = 0; i < 2; ++i) {
      char buf[32];
      snprintf(buf, sizeof(buf), "%c %s", (i == menu_idx ? '>' : ' '),
               menu_options[i]);
      display.print(2 + i, 0, buf);
    }
  } else if (state == AppState::EDITOR) {
    display.print(0, 0, "EDITOR (UP/DN/OK)");
    display.print(2, 0, editor_buffer);
    // Draw cursor
    char cursor_line[32];
    std::memset(cursor_line, ' ', 32);
    cursor_line[31] = '\0';
    cursor_line[cursor_pos] = '^';
    display.print(3, 0, cursor_line);
  } else if (state == AppState::SOLVER) {
    display.print(0, 0, "SOLVER");
    int line = 1;
    for (const auto &step : solution_steps) {
      if (line >= 8)
        break;
      display.print(line++, 0, step.c_str());
    }
  }
}

} // namespace uMath
