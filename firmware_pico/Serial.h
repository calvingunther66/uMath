#pragma once
#include <cstdio>

// Check if running on RP2040 SDK
#ifdef PICO_BOARD
#include "pico/stdlib.h"
#endif

namespace uMath {

class Serial {
public:
  static void init() {
#ifdef PICO_BOARD
    stdio_init_all();
#endif
  }

  static void print(const char *msg) {
#ifdef PICO_BOARD
    printf("%s\n", msg);
#else
    std::printf("%s\n", msg);
#endif
  }

  static void print_char(char c) {
#ifdef PICO_BOARD
    putchar(c);
#else
    std::putchar(c);
#endif
  }
};

} // namespace uMath
