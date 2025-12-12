#include "Kernel.h"
#include <cmath>
#include <cstdio> // for snprintf

namespace uMath {

LogCallback Kernel::logger = nullptr;

void Kernel::setLogger(LogCallback cb) { logger = cb; }

void Kernel::log(const char *msg) {
  if (logger)
    logger(msg);
}

AnalysisResult Kernel::analyze(NodeID root) {
  AnalysisResult res;
  if (root == NULL_NODE)
    return res;

  res.add(OperationType::EVALUATE);

  // Check for quadratic
  // Simplified check: does it contain 'x' and '^' and '2'?
  // In a real system we'd build the Poly struct here.
  if (is_quadratic_form(root)) {
    res.add(OperationType::FACTOR_QUADRATIC);
    res.add(OperationType::SOLVE_ROOTS);
  }

  return res;
}

bool Kernel::is_quadratic_form(NodeID root) {
  if (root == NULL_NODE)
    return false;
  ASTNode &n = NodeArena::get(root);

  if (n.type == NodeType::OPERATOR && n.op == '^') {
    // Check power 2
    ASTNode &right = NodeArena::get(n.right);
    if (right.type == NodeType::NUMBER &&
        std::abs(right.number - 2.0f) < 0.001f) {
      return true;
    }
  }
  return is_quadratic_form(n.left) || is_quadratic_form(n.right);
}

float Kernel::evaluate(NodeID root) {
  if (root == NULL_NODE)
    return 0.0f;
  ASTNode &n = NodeArena::get(root);

  if (n.type == NodeType::NUMBER)
    return n.number;

  if (n.type == NodeType::OPERATOR) {
    float l = evaluate(n.left);
    float r = evaluate(n.right);

    switch (n.op) {
    case '+':
      return l + r;
    case '-':
      return l - r;
    case '*':
      return l * r;
    case '/':
      return (r != 0) ? l / r : 0;
    case '^':
      return std::pow(l, r);
    case '~':
      return -r;
    }
  }
  return 0.0f;
}

void Kernel::solve_quadratic(const Poly &p) {
  // Assuming p.coeffs[0]=c, [1]=b, [2]=a for ax^2+bx+c
  float a = p.getCoeff(2);
  float b = p.getCoeff(1);
  float c = p.getCoeff(0);

  char buf[64];
  snprintf(buf, sizeof(buf), "Coefficients: a=%.2f, b=%.2f, c=%.2f", a, b, c);
  log(buf);

  if (std::abs(a) < 0.0001f) {
    log("Not a quadratic (a=0)");
    return;
  }

  float D = b * b - 4 * a * c;
  snprintf(buf, sizeof(buf), "Discriminant D = b^2 - 4ac = %.2f", D);
  log(buf);

  if (D >= 0) {
    float r1 = (-b + std::sqrt(D)) / (2 * a);
    float r2 = (-b - std::sqrt(D)) / (2 * a);
    log("Two real roots found."); // Found is better than just "Two real roots"
                                  // implies existence

    snprintf(buf, sizeof(buf), "x1 = %.4f", r1);
    log(buf);
    snprintf(buf, sizeof(buf), "x2 = %.4f", r2);
    log(buf);
  } else {
    log("Discriminant < 0. Complete roots required.");
  }
}

} // namespace uMath
