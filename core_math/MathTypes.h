#pragma once
#include <algorithm>
#include <array>
#include <cstdint>
#include <initializer_list>

namespace uMath {

// --- Fixed-Size Matrix ---
// Allocation: Stack or Static. No heap.
template <typename T, size_t Rows, size_t Cols> struct TinyMatrix {
  std::array<T, Rows * Cols> data;

  TinyMatrix() { data.fill(0); }

  // Initializer list constructor
  TinyMatrix(std::initializer_list<T> list) {
    size_t count = 0;
    for (auto val : list) {
      if (count < Rows * Cols) {
        data[count++] = val;
      }
    }
    // Fill remainder with 0
    while (count < Rows * Cols)
      data[count++] = 0;
  }

  T &at(size_t r, size_t c) {
    // No exceptions in hot path, trust caller or clamp?
    // For speed, trust caller or use modulo.
    return data[r * Cols + c];
  }

  const T &at(size_t r, size_t c) const { return data[r * Cols + c]; }

  constexpr size_t rows() const { return Rows; }
  constexpr size_t cols() const { return Cols; }
};

// --- Fixed-Size Polynomial ---
// Max degree 20 (handling up to x^20).
// Usage: coeffs[0] is constant term, coeffs[1] is x, etc.
constexpr size_t MAX_POLY_DEGREE = 20;

struct Poly {
  float coeffs[MAX_POLY_DEGREE + 1]; // +1 for constant term (x^0) to x^MAX
  uint8_t degree;                    // Current actual highest degree

  Poly() { reset(); }

  void reset() {
    for (int i = 0; i <= MAX_POLY_DEGREE; ++i)
      coeffs[i] = 0.0f;
    degree = 0;
  }

  void setCoeff(uint8_t power, float value) {
    if (power > MAX_POLY_DEGREE)
      return;
    coeffs[power] = value;
    if (power > degree)
      degree = power;
    // Optimization: if we zero out the high term, we should reduce degree,
    // but that requires scanning down.
  }

  float getCoeff(uint8_t power) const {
    if (power > MAX_POLY_DEGREE)
      return 0.0f;
    return coeffs[power];
  }
};

} // namespace uMath
