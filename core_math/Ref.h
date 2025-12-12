#pragma once
#include <cstdint>
#include <cstring>
#include <limits>
#include <string>
#include <vector>

namespace uMath {

using NodeID = uint16_t;
constexpr NodeID NULL_NODE = 0xFFFF;
constexpr size_t MAX_VAR_NAME = 6;

enum class NodeType : uint8_t {
  NONE = 0,
  NUMBER,   // 3.14, 5
  VARIABLE, // x, y, radius
  OPERATOR, // +, -, *, /, ^
  FUNCTION, // sin, cos, log, sqrt
  MATRIX    // Reference to a matrix store (not implemented in Node for size
            // reasons)
};

// Operator IDs
constexpr char OP_ADD = '+';
constexpr char OP_SUB = '-';
constexpr char OP_MUL = '*';
constexpr char OP_DIV = '/';
constexpr char OP_POW = '^';
constexpr char OP_NEG = '~'; // Unary minus

enum class FuncID : uint8_t {
  NONE = 0,
  SIN,
  COS,
  TAN,
  ASIN,
  ACOS,
  ATAN,
  LOG,
  LN,
  SQRT,
  ABS
};

struct Node {
  NodeType type;
  union {
    float number_value;          // For NUMBER
    char var_name[MAX_VAR_NAME]; // For VARIABLE (null terminated if < 6)
    char op_char;                // For OPERATOR
    FuncID func_id;              // For FUNCTION
  };
  NodeID left = NULL_NODE;
  NodeID right = NULL_NODE;

  // Helper constructor for easy cleared initialization
  Node() : type(NodeType::NONE), left(NULL_NODE), right(NULL_NODE) {
    number_value = 0.0f;
  }
};

} // namespace uMath
