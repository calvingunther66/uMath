#include "KeyboardMapper.h"

namespace uMath {

// HID Usage Tables (0x00 - 0x38 roughly covers standard keys)
// 0 means no printable char

// Lowercase / Unshifted
static const char key_map_lower[] = {
    0,      0,   0,    0,   'a',  'b', 'c', 'd', 'e', 'f', 'g', 'h', 'i', 'j',
    'k',    'l', 'm',  'n', 'o',  'p', 'q', 'r', 's', 't', 'u', 'v', 'w', 'x',
    'y',    'z', '1',  '2', '3',  '4', '5', '6', '7', '8', '9', '0',
    '\n',                                  // Enter (0x28)
    '\x1B',                                // Escape (0x29)
    '\b',                                  // Backspace (0x2A)
    '\t',                                  // Tab (0x2B)
    ' ',                                   // Space (0x2C)
    '-',    '=', '[',  ']', '\\',          // 0x2D - 0x31
    '#',    ';', '\'', '`', ',',  '.', '/' // 0x32 - 0x38
};

// Uppercase / Shifted
static const char key_map_upper[] = {
    0,      0,   0,   0,   'A', 'B', 'C', 'D', 'E', 'F', 'G', 'H', 'I', 'J',
    'K',    'L', 'M', 'N', 'O', 'P', 'Q', 'R', 'S', 'T', 'U', 'V', 'W', 'X',
    'Y',    'Z', '!', '@', '#', '$', '%', '^', '&', '*', '(', ')',
    '\n',                                // Enter
    '\x1B',                              // Escape
    '\b',                                // Backspace
    '\t',                                // Tab
    ' ',                                 // Space
    '_',    '+', '{', '}', '|',          // 0x2D - 0x31
    '~',    ':', '"', '~', '<', '>', '?' // 0x32 - 0x38
};

// TinyUSB / Standard HID Modifiers
static const uint8_t KEYBOARD_MODIFIER_LEFTSHIFT = 0x02;
static const uint8_t KEYBOARD_MODIFIER_RIGHTSHIFT = 0x20;

char KeyboardMapper::to_ascii(uint8_t modifier, uint8_t keycode) {
  if (keycode >= sizeof(key_map_lower))
    return 0;

  bool is_shift =
      (modifier & (KEYBOARD_MODIFIER_LEFTSHIFT | KEYBOARD_MODIFIER_RIGHTSHIFT));

  if (is_shift) {
    return key_map_upper[keycode];
  } else {
    return key_map_lower[keycode];
  }
}

} // namespace uMath
