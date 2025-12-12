#pragma once

#include <cstddef>
#include <cstring>

namespace uMath {

struct Formula {
  const char *name;
  const char *description;
  const char *equation_str; // e.g. "a^2 + b^2 = c^2"
  const char *missing_var;  // The default variable to solve for
};

class FormulaBank {
public:
  // Returns pointer to formula if found (substring match on name), or nullptr
  static const Formula *findFormula(const char *search_term);

  // Direct access to array (for listing all)
  static const Formula *getAll();
  static size_t getCount();
};

} // namespace uMath
