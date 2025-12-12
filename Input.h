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
    // TODO: Hook into Flipper GPIO/UART
    return EventType::NONE;
  }

  // Helper to simulate input (for testing)
  EventType last_simulated = EventType::NONE;

  void pushEvent(EventType e) { last_simulated = e; }

  EventType popSimulated() {
    EventType e = last_simulated;
    last_simulated = EventType::NONE;
    return e;
  }
};

} // namespace uMath
