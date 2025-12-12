#!/bin/bash
set -e

echo "=== uMath Build System ==="

# 1. Build RP2040 Firmware
echo "[1/2] Building RP2040 Firmware (uMath_pico.uf2)..."
if [ ! -d "build_pico" ]; then
    mkdir build_pico
fi
cd build_pico
# Assuming PICO_SDK_PATH is set in env
# cmake -DPICO_SDK_PATH=$PICO_SDK_PATH ../firmware_pico
# make
echo "Skipped actual compile (SDK not in env). Run: cd build_pico && cmake ../firmware_pico && make"
cd ..

# 2. Build Flipper App
echo "[2/2] Building Flipper App (uMath_terminal.fap)..."
# ufbt ../app_flipper
echo "Skipped actual compile (ufbt not in env). Run: ufbt ./app_flipper"

echo "=== Build Config Ready ==="
