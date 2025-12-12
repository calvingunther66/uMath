#pragma once
#include <cstdint>

// Mocks for TinyUSB types so we can compile tests on host
namespace uMath {
namespace mocks {

typedef struct {
  uint8_t modifiers;
  uint8_t reserved;
  uint8_t keycodes[6];
} hid_keyboard_report_t;

// Standard HID Modifiers
#define KEYBOARD_MODIFIER_LEFTSHIFT (1 << 1)
#define KEYBOARD_MODIFIER_RIGHTSHIFT (1 << 5)

// Standard HID Keycodes
#define HID_KEY_A 0x04
#define HID_KEY_1 0x1E
#define HID_KEY_2 0x1F
#define HID_KEY_3 0x20
#define HID_KEY_4 0x21
#define HID_KEY_5 0x22
#define HID_KEY_6 0x23
#define HID_KEY_7 0x24
#define HID_KEY_8 0x25
#define HID_KEY_9 0x26
#define HID_KEY_0 0x27

#define HID_KEY_ENTER 0x28
#define HID_KEY_SPACE 0x2C
#define HID_KEY_MINUS 0x2D
#define HID_KEY_EQUAL 0x2E
#define HID_KEY_BRACKET_LEFT 0x2F
#define HID_KEY_BRACKET_RIGHT 0x30
#define HID_KEY_BACKSLASH 0x31

typedef struct {
  uint8_t buttons;
  int8_t x;
  int8_t y;
  int8_t wheel;
  int8_t pan;
} hid_mouse_report_t;

} // namespace mocks
} // namespace uMath

// Use mocks if not actual TinyUSB
#ifndef PICO_BOARD
using hid_keyboard_report_t = uMath::mocks::hid_keyboard_report_t;
using hid_mouse_report_t = uMath::mocks::hid_mouse_report_t;
#endif
