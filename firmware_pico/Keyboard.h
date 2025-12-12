#pragma once
#include <cstdint>

// If mocked, include mocks, otherwise TinyUSB
#ifndef PICO_BOARD
#include "MockHardware.h"
#else
#include "tusb.h"
#endif

namespace uMath {

class KeyboardHost {
public:
  // Process a raw HID report and return the ASCII character
  // Returns 0 if no printable character (e.g. just a modifier change)
  static char process_report(hid_keyboard_report_t const *report);

private:
  // Basic HID map for standard US Layout
  // Index 4 (A) to 39 (0)
  static const char keymap_lower[128];
  static const char keymap_upper[128];
};

} // namespace uMath
