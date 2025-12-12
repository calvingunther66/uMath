#pragma once
#include "Ref.h"
#include <vector>

namespace Antigravity {

// Global or scoped Arena to manage nodes without malloc overhead per node
class Arena {
public:
  // Reserve specific capacity to prevent reallocations if possible
  explicit Arena(size_t capacity = 4096) {
    nodes.reserve(capacity);
    // Index 0 is often essentially 'null' or reserved, but since we use 0xFFFF
    // for NULL, 0 is a valid index. However, let's keep it simple.
  }

  void reset() { nodes.clear(); }

  NodeID alloc() {
    if (nodes.size() >= 0xFFFE) {
      // Handle overflow - in embedded, this might be a fatal error or reset
      return NULL_NODE;
    }
    nodes.emplace_back();
    return static_cast<NodeID>(nodes.size() - 1);
  }

  NodeID alloc(NodeType type) {
    NodeID id = alloc();
    if (id != NULL_NODE) {
      nodes[id].type = type;
    }
    return id;
  }

  Node &get(NodeID id) {
    // In production, might remove bounds check for speed if verified safe
    if (id >= nodes.size()) {
      static Node dummy; // Dangerous but prevents crash in bad logic
      return dummy;
    }
    return nodes[id];
  }

  const Node &get(NodeID id) const {
    if (id >= nodes.size()) {
      static Node dummy;
      return dummy;
    }
    return nodes[id];
  }

  size_t size() const { return nodes.size(); }

private:
  std::vector<Node> nodes;
};

} // namespace Antigravity
