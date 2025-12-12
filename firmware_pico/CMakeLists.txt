cmake_minimum_required(VERSION 3.13)

include(pico_sdk_import.cmake)

project(uMath_pico C CXX ASM)
set(CMAKE_C_STANDARD 11)
set(CMAKE_CXX_STANDARD 17)

pico_sdk_init()

# --- TinyUSB Host ---
# Fix RP2040 USB Host Enumeration Issue
add_definitions(-DPICO_RP2040_USB_DEVICE_ENUMERATION_FIX=1)

# UART Definitions (Matches Video Game Module Hardware)
add_definitions(-DUART_TX_PIN=0)
add_definitions(-DUART_RX_PIN=1)

# --- uMath Sources ---
# Core Math Library
set(CORE_SOURCES
    ../core_math/AST.cpp
    ../core_math/Parser.cpp
    ../core_math/Kernel.cpp
)

# Firmware Implementation
set(FIRMWARE_SOURCES
    main.cpp
    App.cpp
    Keyboard.cpp
    # Mouse is header only currently but include if cpp added later
)

add_executable(uMath_pico
    ${FIRMWARE_SOURCES}
    ${CORE_SOURCES}
)

# Include Paths
target_include_directories(uMath_pico PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}
    ${CMAKE_CURRENT_SOURCE_DIR}/../core_math
)

# TinyUSB Config
target_include_directories(uMath_pico PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})

# Link Libraries
target_link_libraries(uMath_pico 
    pico_stdlib 
    tinyusb_host
    tinyusb_board
)

# Enable USB Output (for UART mainly but USB can also print)
pico_enable_stdio_usb(uMath_pico 0) 
pico_enable_stdio_uart(uMath_pico 1)

pico_add_extra_outputs(uMath_pico)
