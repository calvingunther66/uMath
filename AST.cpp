#include "AST.h"

namespace uMath {

// Definition of static members
constexpr size_t NodeArena::POOL_SIZE;
ASTNode NodeArena::pool[NodeArena::POOL_SIZE];
uint16_t NodeArena::ptr = 0;

void NodeArena::reset() { ptr = 0; }

NodeID NodeArena::alloc(NodeType type) {
  if (ptr >= POOL_SIZE) {
    return NULL_NODE; // Out of memory
  }

  NodeID id = ptr++;
  ASTNode &node = pool[id];

  // Init defaults
  node.type = type;
  node.left = NULL_NODE;
  node.right = NULL_NODE;
  node.number = 0.0f; // Clear union (simplest way covering 4 bytes)

  return id;
}

ASTNode &NodeArena::get(NodeID id) {
  if (id >= POOL_SIZE) {
    // Return the 0th node as dummy or handle error?
    // In embedded, we might prefer a panic or safe fallback.
    // For now, return a safe reference to index 0, but this is dangerous
    // logic-wise.
    return pool[0];
  }
  return pool[id];
}

size_t NodeArena::usage() { return ptr; }

} // namespace uMath
