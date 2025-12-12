#pragma once
#include <cstdint>

namespace uMath {

class KeyboardMapper {
public:
  // Convert HID modifier + keycode to ASCII
  // Returns 0 if not a printable char
  static char to_ascii(uint8_t modifier, uint8_t keycode);
};

} // namespace uMath
