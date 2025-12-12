#pragma once
#include "AST.h"
#include <cstdint>

namespace uMath {

enum class OperationType {
  NONE = 0,
  EVALUATE,
  SIMPLIFY,
  FACTOR_QUADRATIC,
  SOLVE_ROOTS,
  MATRIX_DET,
  MATRIX_TRANSPOSE
};

struct AnalysisResult {
  OperationType ops[8];
  int count = 0;

  void add(OperationType op) {
    if (count < 8)
      ops[count++] = op;
  }
};

// Callback type for logging steps
using LogCallback = void (*)(const char *);

class Kernel {
public:
  static void setLogger(LogCallback cb);

  static AnalysisResult analyze(NodeID root);

  // Solvers
  static float evaluate(NodeID root);
  static void solve_quadratic(const Poly &p);

private:
  static LogCallback logger;
  static void log(const char *msg);
  static bool is_quadratic_form(NodeID root);
};

} // namespace uMath
