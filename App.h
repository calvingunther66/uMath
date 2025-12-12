#pragma once
#include "Display.h"
#include "Input.h"
#include "Kernel.h"
#include "Parser.h"
#include <string>
#include <vector>

namespace uMath {

enum class AppState { MENU, EDITOR, SOLVER };

class uMathApp {
public:
  uMathApp();

  void update(); // Main loop step
  void render(); // Draw to display buffer

  // Simulate hardware input (for testing)
  InputManager input;
  DisplayBuffer display;

private:
  AppState state = AppState::MENU;

  // --- Menu State ---
  int menu_idx = 0;
  const char *menu_options[2] = {"Calculator", "About"};

  // --- Editor State ---
  // "Strip Editor"
  // Characters available to cycle through
  static constexpr const char *CHAR_SET = "0123456789+-*/^=.xyz() ";
  char editor_buffer[32]; // 32 chars max
  int cursor_pos = 0;

  void handle_editor_input(EventType e);
  char get_char_at_cursor();
  void set_char_at_cursor(char c);
  void cycle_char(int direction); // +1 or -1

  // --- Solver State ---
  NodeID current_ast_root = NULL_NODE;
  AnalysisResult current_analysis;
  // We store solution steps as simple strings for now
  std::vector<std::string> solution_steps;
  int scroll_offset = 0;

  // Kernel callback to capture steps
  static void kernel_log_callback(const char *msg);
  // Static pointer to instance for callback to access
  static uMathApp *instance;
  void add_solution_step(const char *step);
};

} // namespace uMath
