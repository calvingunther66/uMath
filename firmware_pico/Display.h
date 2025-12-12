#pragma once
#include <array>
#include <cstdio>
#include <cstring>

namespace uMath {

// Simple fixed-size text buffer simulating a screen
// 8 lines, 32 chars wide
class DisplayBuffer {
public:
  static constexpr int LINES = 8;
  static constexpr int COLS = 32;

  DisplayBuffer() { clear(); }

  void clear() {
    for (int i = 0; i < LINES; ++i) {
      std::memset(buffer[i], ' ', COLS); // Fill with spaces
      buffer[i][COLS] = '\0';            // Null terminate for debug printing
    }
  }

  void print(int line, int col, const char *text) {
    if (line < 0 || line >= LINES)
      return;
    if (col < 0 || col >= COLS)
      return;

    int len = std::strlen(text);
    for (int i = 0; i < len; ++i) {
      if (col + i >= COLS)
        break;
      buffer[line][col + i] = text[i];
    }
  }

  // For debugging: dump to stdout
  const char *getLine(int line) const {
    if (line < 0 || line >= LINES)
      return "";
    return buffer[line];
  }

  // Direct access for rendering engines
  char buffer[LINES][COLS + 1];
};

} // namespace uMath
