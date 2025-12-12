#include "App.h" // To test InputManager queue integration directly
#include "Keyboard.h"
#include "Mouse.h" // New
#include "Serial.h"
#include "Shell.h"
#include <iostream>

// If mocked
#ifndef PICO_BOARD
#include "MockHardware.h"
#endif

using namespace uMath;

// Global Shell for callbacks
Shell *g_shell = nullptr;

void serial_logger(const char *msg) {
  Serial::print(msg);
  // Serial::print("\n"); // Appending newline might duplicate if shell already
  // does it? Shell adds \n during `onChar` echo, but Kernel logs are separate
  // lines. Let's assume Kernel logs don't have newlines. Serial::print("\n");
}

int main() {
  Serial::init();
  Kernel::setLogger(serial_logger);

  // We will test the unified InputManager used by the App state machine
  // (Navigation) AND the Shell (Typing). Note: In Phase 4 we used Shell
  // directly, bypassing App's state machine for typing. In Phase 3 we used App.
  // For this test, let's verify Mouse -> App Navigation.

  uMathApp app;
  MouseHandler mouse(app.input); // Hook mouse to App's InputManager

  std::cout << "=== uMath Phase 5: Composite HID ===\n";
  std::cout << "State: MENU\n";

  // 1. Simulate Mouse Scroll Down (Y > 10)
  // Send two packets to test accumulator
  hid_mouse_report_t mrep;
  mrep.buttons = 0;
  mrep.x = 0;
  mrep.wheel = 0;
  mrep.pan = 0;

  mrep.y = 6;
  std::cout << "[Mouse] Moving Y+6...\n";
  mouse.process_report(&mrep);
  app.update(); // Should do nothing (Threshold 10)

  mrep.y = 6; // Total 12
  std::cout << "[Mouse] Moving Y+6 (Total 12)...\n";
  mouse.process_report(&mrep);
  app.update(); // Should trigger DOWN -> Move selection
  app.render();

  std::cout << "\nDisplay should show selection moved down.\n";

  // 2. Simulate Left Click (OK) -> Enter Editor
  mrep.y = 0;
  mrep.buttons = 1; // Left Click
  std::cout << "\n[Mouse] Left Click (OK)...\n";
  mouse.process_report(&mrep);
  app.update(); // Trigger OK -> Editor
  app.render();

  // 3. Simulate Editor Nav (Move Right)
  std::cout << "\n[Mouse] Moving X+12...\n";
  mrep.buttons = 0;
  mrep.x = 12;
  mouse.process_report(&mrep);
  app.update(); // Move cursor Right
  app.render(); // Cursor should move

  return 0;
}
