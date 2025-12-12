# uMath: Embedded Computer Algebra System
**For Raspberry Pi Pico (RP2040) & Flipper Zero**

uMath (formerly "Antigravity") is a highly optimized, memory-safe Computer Algebra System (CAS) designed to run on the **RP2040** microcontroller (Video Game Module) while using the **Flipper Zero** as a display and controller.

## 🚀 Key Features

### 🧠 Core Engine (`core_math`)
*   **Zero-Heap Architecture**: Uses a static `NodeArena` and fixed-size buffers (`TinyMatrix`, `Poly`) to prevent memory fragmentation on the 264KB RAM RP2040.
*   **Custom Parser**: A Shunting-Yard implementation optimized for C-strings, avoiding heavy STL containers (`std::string`, `std::vector`) in hot paths.
*   **Symbolic Kernel**: Capable of parsing, analyzing, and solving algebraic expressions (e.g., Step-by-Step Quadratic Solver).

### 🎮 Hardware Integration (`firmware_pico`)
*   **Composite HID Host**: Supports plugging in a **Wireless Keyboard & Mouse** via USB OTG.
    *   **Keyboard**: Full typing support with correct Shift-modifier mapping for math symbols (`^`, `*`, `+`).
    *   **Mouse-to-Macro**: Innovative control scheme converting mouse physics to macros:
        *   **Scroll / Y-Axis**: Acts as `UP/DOWN` for menu navigation.
        *   **X-Axis**: Acts as `LEFT/RIGHT` for cursor movement.
        *   **Left Click**: `ENTER/OK`.
        *   **Right Click**: `BACK/CANCEL`.
*   **Smooth Navigation**: Implements an "Accumulator Threshold" algorithm to filter mouse sensor noise.

### 📟 Display (`app_flipper`)
*   Uses the Flipper Zero as a Serial Terminal to display the UI rendered by the RP2040.

## 📂 Project Structure

| Directory | Description |
| :--- | :--- |
| **`core_math/`** | Platform-agnostic C++ logic (AST, Parser, Solver). |
| **`firmware_pico/`** | RP2040 implementation. Handles USB Host, UART, and the App State Machine. |
| **`app_flipper/`** | Flipper Zero application metadata (`.fam`) for the companion app. |

## 🛠 Building the Project

The system requires two build steps: one for the RP2040 firmware and one for the Flipper app.

### Prerequisites
*   **Pico SDK** (for RP2040)
*   **ufbt** (for Flipper Zero)
*   `cmake`, `make`, `arm-none-eabi-gcc`

### Build Command
We provide a helper script:
```bash
./build_all.sh
```

**Manual Steps:**

1.  **Build Firmware (`.uf2`)**:
    ```bash
    cd build_pico
    cmake -DPICO_SDK_PATH=/path/to/pico-sdk ../firmware_pico
    make
    # Flash uMath_pico.uf2 to the Video Game Module
    ```

2.  **Build App (`.fap`)**:
    ```bash
    ufbt ./app_flipper
    # Install the resulting .fap to the Flipper Zero
    ```

## 🕹 Usage

1.  **Connect**: Plug the Video Game Module into the Flipper Zero.
2.  **Input**: Plug a USB Keyboard/Mouse dongle into the Module's USB-C port.
3.  **Run**: Launch "uMath Terminal" on the Flipper.
4.  **Math**:
    *   Type equations like `x^2 - 5x + 6`.
    *   Press **Enter** (or Left Click) to solve.
    *   Use **Up/Down** (or Scroll) to view step-by-step results.
