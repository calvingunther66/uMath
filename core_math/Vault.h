#pragma once
#include <map>
#include <string>
#include <vector>

namespace uMath {

struct Formula {
  std::string name;
  std::string equation; // e.g., "a^2 + b^2"
  std::vector<std::string> required_vars;
};

class Vault {
public:
  void set_var(const std::string &name, float value) {
    variables[name] = value;
  }

  bool get_var(const std::string &name, float &out_value) const {
    auto it = variables.find(name);
    if (it != variables.end()) {
      out_value = it->second;
      return true;
    }
    return false;
  }

  void add_formula(const std::string &name, const std::string &eqn,
                   const std::vector<std::string> &vars) {
    formulas[name] = {name, eqn, vars};
  }

  const Formula *get_formula(const std::string &name) {
    auto it = formulas.find(name);
    return (it != formulas.end()) ? &it->second : nullptr;
  }

  const std::map<std::string, float> &get_all_vars() const { return variables; }

private:
  std::map<std::string, float> variables;
  std::map<std::string, Formula> formulas;
};

} // namespace uMath
