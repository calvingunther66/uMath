#include "Parser.h"

namespace uMath {

// OP Constants
constexpr char OP_ADD = '+';
constexpr char OP_SUB = '-';
constexpr char OP_MUL = '*';
constexpr char OP_DIV = '/';
constexpr char OP_POW = '^';
constexpr char OP_NEG = '~';

int Parser::get_precedence(char op) {
  if (op == OP_ADD || op == OP_SUB)
    return 1;
  if (op == OP_MUL || op == OP_DIV)
    return 2;
  if (op == OP_POW)
    return 3;
  if (op == OP_NEG)
    return 4;
  return 0;
}

bool Parser::is_operator(char c) {
  return c == '+' || c == '-' || c == '*' || c == '/' || c == '^';
}

NodeID Parser::parse(const char *expr) {
  FixedStack<NodeID, 64> values;
  FixedStack<NodeID, 64> ops;

  bool expect_unary = true;
  const char *p = expr;

  auto process_op = [&]() {
    if (ops.empty())
      return;

    NodeID opID = ops.pop();
    ASTNode &opNode = NodeArena::get(opID);

    // Unary
    if (opNode.op == OP_NEG && opNode.left == NULL_NODE &&
        opNode.right == NULL_NODE) {
      if (values.empty())
        return; // Error
      opNode.right = values.pop();
      opNode.left = NULL_NODE;
      values.push(opID);
      return;
    }

    // Binary
    if (values.size() < 2)
      return; // Error

    opNode.right = values.pop();
    opNode.left = values.pop();
    values.push(opID);
  };

  while (*p) {
    if (isspace(*p)) {
      p++;
      continue;
    }

    if (isdigit(*p) || *p == '.') {
      // Number
      char *end;
      float val = std::strtof(p, &end);
      p = end;

      NodeID id = NodeArena::alloc(NodeType::NUMBER);
      NodeArena::get(id).number = val;
      values.push(id);
      expect_unary = false;
    } else if (isalpha(*p)) {
      // Variable (assume short name < 6 chars)
      char name[6] = {0};
      int len = 0;
      while (isalnum(*p) && len < 5) {
        name[len++] = *p++;
      }
      // Skip excess
      while (isalnum(*p))
        p++;

      NodeID id = NodeArena::alloc(NodeType::VARIABLE);
      // safe copy
      for (int i = 0; i < 6; i++)
        NodeArena::get(id).var_name[i] = name[i];

      values.push(id);
      expect_unary = false;
    } else if (*p == '(') {
      NodeID id = NodeArena::alloc(NodeType::OPERATOR);
      NodeArena::get(id).op = '(';
      ops.push(id);
      expect_unary = true;
      p++;
    } else if (*p == ')') {
      while (!ops.empty() && NodeArena::get(ops.top()).op != '(') {
        process_op();
      }
      if (!ops.empty())
        ops.pop(); // Pop '('
      expect_unary = false;
      p++;
    } else if (is_operator(*p)) {
      char op = *p;
      if (op == '-' && expect_unary)
        op = OP_NEG;

      while (!ops.empty()) {
        NodeID topID = ops.top();
        char topChar = NodeArena::get(topID).op;

        if (topChar == '(')
          break;

        int currPrec = get_precedence(op);
        int topPrec = get_precedence(topChar);

        if ((op == OP_POW || op == OP_NEG) && currPrec >= topPrec)
          break;
        if (op != OP_POW && op != OP_NEG && currPrec > topPrec)
          break;

        process_op();
      }

      NodeID id = NodeArena::alloc(NodeType::OPERATOR);
      NodeArena::get(id).op = op;
      ops.push(id);
      expect_unary = true;
      p++;
    } else {
      // invalid char, skip
      p++;
    }
  }

  while (!ops.empty()) {
    process_op();
  }

  return values.empty() ? NULL_NODE : values.top();
}

} // namespace uMath
