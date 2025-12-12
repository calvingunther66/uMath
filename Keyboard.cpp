#include "Keyboard.h"
#include <cstring>

namespace uMath {

// Simplified Lookup tables for HID Code -> ASCII
// Codes 0x04 (A) ...
// We will only populate the standard range to save space/time for this demo
// 0 means no char

// Map for HID 0x00 to 0x38 (roughly)
static const char hid_lower[] = {
    0,   0,    0,   0,   'a',  'b', 'c', 'd', 'e', 'f', 'g', 'h',
    'i', 'j',  'k', 'l', 'm',  'n', 'o', 'p', 'q', 'r', 's', 't',
    'u', 'v',  'w', 'x', 'y',  'z', '1', '2', '3', '4', '5', '6',
    '7', '8',  '9', '0', '\n', 0,   0,   0,   ' ', '-', '=', '[',
    ']', '\\', '#', ';', '\'', '`', ',', '.', '/'};

static const char hid_upper[] = {
    0,   0,   0,   0,   'A', 'B', 'C', 'D', 'E', 'F', 'G',  'H', 'I', 'J', 'K',
    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V',  'W', 'X', 'Y', 'Z',
    '!', '@', '#', '$', '%', '^', '&', '*', '(', ')', '\n', 0,   0,   0,   ' ',
    '_', '+', '{', '}', '|', '~', ':', '"', '~', '<', '>',  '?'};

char KeyboardHost::process_report(hid_keyboard_report_t const *report) {
  if (!report)
    return 0;

// Check modifiers
// Shift is bit 1 (Left) or bit 5 (Right)
#ifndef PICO_BOARD
// Mocks
#define MOD_LSHIFT (1 << 1)
#define MOD_RSHIFT (1 << 5)
#else
// TinyUSB defs usually match standard HID
#define MOD_LSHIFT KEYBOARD_MODIFIER_LEFTSHIFT
#define MOD_RSHIFT KEYBOARD_MODIFIER_RIGHTSHIFT
#endif

  bool shift =
      (report->modifiers & MOD_LSHIFT) || (report->modifiers & MOD_RSHIFT);

  // Check first keycode (simplification: only handle one key press at a time)
  // Rollover logic is not implemented here for "typing math".
  uint8_t key = report->keycodes[0];

  if (key == 0)
    return 0;

  if (key < sizeof(hid_lower)) {
    return shift ? hid_upper[key] : hid_lower[key];
  }

  return 0;
}

} // namespace uMath
