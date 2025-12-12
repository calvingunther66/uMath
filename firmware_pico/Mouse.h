#pragma once
#include "Input.h"
#include <cmath>
#include <cstdint>

// If mocked
#ifndef PICO_BOARD
#include "MockHardware.h"
#else
#include "tusb.h"
#endif

namespace uMath {

class MouseHandler {
public:
  static constexpr int MOVE_THRESHOLD = 10;

  MouseHandler(InputManager &input_ref) : input(input_ref) {}

  void process_report(hid_mouse_report_t const *report) {
    if (!report)
      return;

    // 1. Buttons
    // Bit 0 = Left (OK), Bit 1 = Right (BACK)
    bool left = report->buttons & 0x01;
    bool right = report->buttons & 0x02;

    // Simple edge detection (naive)
    if (left && !prev_left)
      input.pushEvent(EventType::BTN_OK);
    if (right && !prev_right)
      input.pushEvent(EventType::BTN_BACK);

    prev_left = left;
    prev_right = right;

    // 2. Movement Accumulator
    acc_x += report->x;
    acc_y += report->y;
    acc_wheel += report->wheel; // Wheel usually has small values or steps

    // Check X Threshold
    if (acc_x > MOVE_THRESHOLD) {
      input.pushEvent(EventType::BTN_RIGHT);
      acc_x = 0; // Reset or subtract? Reset prevents machine-gunning too fast?
      // Subtracting allows smoother "flow" but reset is safer for discrete menu
      // steps. Let's reset.
    } else if (acc_x < -MOVE_THRESHOLD) {
      input.pushEvent(EventType::BTN_LEFT);
      acc_x = 0;
    }

    // Check Y Threshold
    if (acc_y > MOVE_THRESHOLD) {
      input.pushEvent(EventType::BTN_DOWN);
      acc_y = 0;
    } else if (acc_y < -MOVE_THRESHOLD) {
      input.pushEvent(EventType::BTN_UP);
      acc_y = 0;
    }

    // Check Wheel (Sensitive)
    if (report->wheel != 0) {
      if (report->wheel > 0)
        input.pushEvent(EventType::BTN_DOWN);
      else
        input.pushEvent(EventType::BTN_UP);
    }
  }

private:
  InputManager &input;
  int16_t acc_x = 0;
  int16_t acc_y = 0;
  int16_t acc_wheel = 0;

  bool prev_left = false;
  bool prev_right = false;
};

} // namespace uMath
