#pragma once
#include "../core_math/FormulaBank.h"
#include "Display.h"
#include "Input.h"
#include <string>
#include <vector>

namespace uMath {

class SearchUI {
public:
  SearchUI();

  void update(InputManager &input_mgr);
  void render(DisplayBuffer &display);

  // Process single char from Keyboard
  void onChar(char c);

  // Check if user selected something
  const Formula *getSelectedFormula() const { return selected_formula; }
  void reset();

private:
  char query_buffer[32];
  std::vector<int> filtered_indices;
  const Formula *selected_formula = nullptr;
  int selection_index = -1; // Index within the filtered list

  void filter_results();
};

} // namespace uMath
