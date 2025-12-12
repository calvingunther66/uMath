#include "../core_math/FormulaBank.h"
#include "KeyboardMapper.h" // New
#include "SearchUI.h"       // New
#include "Serial.h"
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

  // Setup
  SearchUI search;
  DisplayBuffer display;
  InputManager input;

  std::cout << "=== uMath Phase 9: Search UI ===\n";

  // 1. Initial State (Show All?)
  search.render(display);
  dump_display(display);

  // 2. Simulate User Typing "Gas"
  // Use KeyboardMapper to convert fictional HID codes
  // 'g' = 0x0A (10), 'a' = 0x04 (4), 's' = 0x16 (22)
  // Shift is off

  std::cout << "[Input] Typing 'g'...\n";
  char c = KeyboardMapper::to_ascii(0, 0x0A); // 'g'
  search.onChar(c);

  std::cout << "[Input] Typing 'a'...\n";
  c = KeyboardMapper::to_ascii(0, 0x04); // 'a'
  search.onChar(c);

  std::cout << "[Input] Typing 's'...\n";
  c = KeyboardMapper::to_ascii(0, 0x16); // 's'
  search.onChar(c);

  search.render(display);
  dump_display(display);

  // 3. Select Result
  // Output shows "Ideal Gas Law" and "Combined Gas Law".
  // Let's select second one?
  std::cout << "[Input] Moving Down...\n";
  input.pushEvent(EventType::BTN_DOWN);
  search.update(input);
  search.render(display);
  dump_display(display);

  std::cout << "[Input] Pressing OK...\n";
  input.pushEvent(EventType::BTN_OK);
  search.update(input);

  const Formula *f = search.getSelectedFormula();
  if (f) {
    std::cout << "\n[Result] Selected: " << f->name << "\n";
  } else {
    std::cout << "[Error] No selection.\n";
  }

  // 4. Test Special Characters (Keyboard Map)
  // Shift + 6 = '^'
  std::cout << "\n[Mapper Test] Shift+6 -> ";
  // TinyUSB Shift Left is 0x02
  c = KeyboardMapper::to_ascii(0x02, 0x23); // 6 is 0x23 (35)
  std::cout << "'" << c << "'\n";

  // Shift + 8 = '*'
  std::cout << "[Mapper Test] Shift+8 -> ";
  c = KeyboardMapper::to_ascii(0x02, 0x25); // 8 is 0x25 (37)
  std::cout << "'" << c << "'\n";

  return 0;
}
