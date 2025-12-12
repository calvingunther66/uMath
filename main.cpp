#include "AST.h"
#include "Kernel.h"
#include "Parser.h"
#include <iostream>

using namespace uMath;

// logger callback
void my_logger(const char *msg) { std::cout << "[LOG] " << msg << std::endl; }

void print_tree(NodeID root, int depth = 0) {
  if (root == NULL_NODE)
    return;
  ASTNode &n = NodeArena::get(root);

  for (int i = 0; i < depth; ++i)
    std::cout << "  ";

  if (n.type == NodeType::NUMBER)
    std::cout << n.number << std::endl;
  else if (n.type == NodeType::VARIABLE)
    std::cout << n.var_name << std::endl;
  else if (n.type == NodeType::OPERATOR)
    std::cout << n.op << std::endl;

  print_tree(n.left, depth + 1);
  print_tree(n.right, depth + 1);
}

int main() {
  NodeArena::reset();
  Kernel::setLogger(my_logger);

  std::cout << "=== uMath Phase 2: Embedded Hardening ===" << std::endl;

  // 1. Test Static Parser
  const char *expr = "3 + 4 * 2";
  std::cout << "\nTest 1: " << expr << std::endl;
  NodeID root = Parser::parse(expr);
  print_tree(root);
  float res = Kernel::evaluate(root);
  std::cout << "Result: " << res << std::endl;

  // 2. Test Quadratic Detection
  const char *q_expr = "x^2 + 5*x + 6";
  std::cout << "\nTest 2: Parse " << q_expr << std::endl;
  NodeID q_root = Parser::parse(q_expr);
  // print_tree(q_root);

  AnalysisResult analysis = Kernel::analyze(q_root);
  std::cout << "Analysis found " << analysis.count << " ops." << std::endl;
  for (int i = 0; i < analysis.count; ++i) {
    if (analysis.ops[i] == OperationType::FACTOR_QUADRATIC)
      std::cout << "- Factor Quadratic" << std::endl;
    if (analysis.ops[i] == OperationType::SOLVE_ROOTS)
      std::cout << "- Solve Roots" << std::endl;
  }

  // 3. Test Poly Solver (Manual Poly creation since extraction isn't built yet)
  std::cout << "\nTest 3: Solving Poly x^2 - 5x + 6" << std::endl;
  Poly p;
  p.setCoeff(2, 1.0f);  // 1x^2
  p.setCoeff(1, -5.0f); // -5x
  p.setCoeff(0, 6.0f);  // +6

  Kernel::solve_quadratic(p);

  std::cout << "\nMemory Usage: " << NodeArena::usage() << " nodes."
            << std::endl;

  return 0;
}
