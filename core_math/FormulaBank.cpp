#include "FormulaBank.h"
#include <cstring>

namespace uMath {

// Hard-coded database of formulas stored in Flash (rodata)
static const Formula formulas[] = {
    // === 1. Mathematics ===
    {"Pythagorean Theorem", "Right triangle side relation", "a^2 + b^2 = c^2",
     "c"},
    {"Quadratic Formula", "Roots of quadratic equation",
     "x = (-b + sqrt(b^2 - 4*a*c)) / (2*a)", "x"},
    {"Distance Formula", "Distance between two points",
     "d = sqrt((x2 - x1)^2 + (y2 - y1)^2)", "d"},
    {"Circle Area", "Area of a circle from radius", "A = 3.14159 * r^2", "A"},
    {"Sphere Volume", "Volume of a sphere", "V = (4/3) * 3.14159 * r^3", "V"},
    {"Trapezoid Area", "Area of a trapezoid", "A = ((a + b) / 2) * h", "A"},
    {"Law of Cosines", "General triangle side relation",
     "c^2 = a^2 + b^2 - 2*a*b*cos(C)", "c"},
    {"Slope Formula", "Slope between two points", "m = (y2 - y1) / (x2 - x1)",
     "m"},
    {"Arithmetic Series Sum", "Sum of arithmetic sequence",
     "S = (n / 2) * (2*a + (n - 1)*d)", "S"},
    {"Geometric Series Sum", "Sum of geometric sequence",
     "S = a * (1 - r^n) / (1 - r)", "S"},

    // === 2. Chemistry ===
    {"Ideal Gas Law", "State of hypothetical gas", "P * V = n * R * T", "P"},
    {"Combined Gas Law", "Ratio of PV/T", "(P1 * V1) / T1 = (P2 * V2) / T2",
     "V2"},
    {"Molarity", "Concentration of solution", "M = n / V", "M"},
    {"Dilution", "Concentration change", "M1 * V1 = M2 * V2", "M2"},
    {"Density", "Mass per unit volume", "rho = m / V", "rho"},
    {"pH Calculation", "Acidity level", "pH = -log(H)", "pH"},
    {"Gibbs Free Energy", "Thermodynamic potential", "dG = dH - T * dS", "dG"},
    {"Boyle's Law", "Pressure-Volume relation", "P1 * V1 = P2 * V2", "V2"},
    {"Charles's Law", "Volume-Temperature relation", "V1 / T1 = V2 / T2", "V2"},

    // === 3. Pharmacology & Medicine ===
    {"Creatinine Clearance", "Cockcroft-Gault Eq",
     "CrCl = ((140 - age) * weight) / (72 * SCr)", "CrCl"},
    {"Elimination Half-Life", "Time for 50% elimination", "t_half = 0.693 / k",
     "t_half"},
    {"Volume of Distribution", "Theoretical volume", "Vd = dose / C0", "Vd"},
    {"BMI", "Body Mass Index", "BMI = weight / (height^2)", "BMI"},
    {"Young's Rule", "Pediatric dosage based on age",
     "ChildDose = (Age / (Age + 12)) * AdultDose", "ChildDose"}};

const Formula *FormulaBank::getAll() { return formulas; }

size_t FormulaBank::getCount() { return sizeof(formulas) / sizeof(Formula); }

const Formula *FormulaBank::findFormula(const char *search_term) {
  if (!search_term)
    return nullptr;

  // Poor man's case-insensitive substring search could go here
  // For now, strict strstr
  size_t count = getCount();
  for (size_t i = 0; i < count; ++i) {
    // Search in Name
    if (strstr(formulas[i].name, search_term)) {
      return &formulas[i];
    }
  }
  return nullptr;
}

} // namespace uMath
