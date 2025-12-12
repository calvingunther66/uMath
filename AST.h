#pragma once
#include "MathTypes.h"
#include <cstdint>

namespace uMath {

// Node Indices are 16-bit to save space
using NodeID = uint16_t;
constexpr NodeID NULL_NODE = 0xFFFF;

enum class NodeType : uint8_t {
  NONE = 0,
  NUMBER,    // 3.14
  VARIABLE,  // x, y
  OPERATOR,  // +, *, ^
  FUNCTION,  // sin, cos
  MATRIX,    // Reference to matrix
  POLYNOMIAL // Reference to poly
};

struct ASTNode {
  NodeType type;
  union {
    float number;       // 4 bytes ~ 32bit
    char op;            // OPERATOR
    char var_name[6];   // VARIABLE strings short (null term if < 6)
    uint16_t matrix_id; // ID for a Matrix pool (if we implement one)
    uint16_t poly_id;   // ID for a Poly pool
    // Note: Union size will be sizeof(largest member) = 6 bytes (var_name)
  };

  // Tree structure
  NodeID left = NULL_NODE;
  NodeID right = NULL_NODE;

  // Total Size: ~1 byte (Type) + 6 bytes (Union) + 4 bytes (Kids) + Padding =
  // ~12-16 bytes.
};

// Static Arena - No Malloc
// Lives in BSS section
class NodeArena {
public:
  static void reset();
  static NodeID alloc(NodeType type);
  static ASTNode &get(NodeID id);
  static size_t usage();

private:
  static constexpr size_t POOL_SIZE = 4096;
  static ASTNode pool[POOL_SIZE];
  static uint16_t ptr;
};

} // namespace uMath
