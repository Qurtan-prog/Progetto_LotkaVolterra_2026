#include "input.hpp"

#include <stdexcept>

namespace lotka_volterra {

double read_positive_double(std::istream& in, std::string const& name)
{
  double value;
  if (!(in >> value)) {
    throw std::runtime_error("not accepted value for " + name
                              + ": write a number");
  }
  if (value <= 0.0) {
    throw std::runtime_error("not accepted value for " + name
                              + ": write positive number");
  }
  return value;
}

int read_positive_int(std::istream& in, std::string const& name)
{
  int value;
  if (!(in >> value)) {
    throw std::runtime_error("not accepted value for " + name
                              + ": write an integer number");
  }
  if (value <= 0) {
    throw std::runtime_error("not accepted value for " + name
                              + " write positive number");
  }
  return value;
}

} 