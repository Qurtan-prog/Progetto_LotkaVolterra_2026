#include "input.hpp"

#include <stdexcept>

namespace lotka_volterra {

double read_positive_double(std::istream &in, std::string const &name) {
  double value;
  if (!(in >> value)) {
    throw std::runtime_error("invalid value for " + name +
                             ": expected a number");
  }
  if (value <= 0.0) {
    throw std::runtime_error("invalid value for " + name +
                             ": must be greater than zero");
  }
  return value;
}

int read_positive_int(std::istream &in, std::string const &name) {
  int value;
  if (!(in >> value)) {
    throw std::runtime_error("invalid value for " + name +
                             ": expected an integer");
  }
  if (value <= 0) {
    throw std::runtime_error("invalid value for " + name +
                             ": must be greater than zero");
  }
  return value;
}

}  // namespace lotka_volterra