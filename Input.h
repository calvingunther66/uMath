#pragma once
#include <cstdint>

namespace uMath {

enum class EventType {
  NONE = 0,
  BTN_UP,
  BTN_DOWN,
  BTN_LEFT,
  BTN_RIGHT,
  BTN_OK,
  BTN_BACK
};

class InputManager {
public:
  // Polls hardware. For now, returns NONE.
  // In a real implementation, this would read GPIOs or UART buffer.
  EventType poll() {
    if (event_count > 0) {
      EventType e = events[head];
      head = (head + 1) % MAX_EVENTS;
      event_count--;
      return e;
    }
    return EventType::NONE;
  }

  // Helper to simulate input (for testing)
  // Now a proper FIFO queue
  static constexpr int MAX_EVENTS = 16;
  EventType events[MAX_EVENTS];
  int head = 0;
  int tail = 0;
  int event_count = 0;

  void pushEvent(EventType e) {
    if (event_count < MAX_EVENTS) {
      events[tail] = e;
      tail = (tail + 1) % MAX_EVENTS;
      event_count++;
    }
  }

  // Removed popSimulated, poll() handles it now.
  EventType popSimulated() { return poll(); }
};

} // namespace uMath
