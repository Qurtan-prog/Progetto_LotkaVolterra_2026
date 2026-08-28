#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

namespace lotka_volterra {
struct Parameters {
  double A;
  double B;
  double C;
  double D;

  bool operator==(Parameters const &) const = default;
};

struct State {
  double x;
  double y;
  double H;

  bool operator==(State const &) const = default;
};

class Simulation {
 public:
  Simulation(Parameters const &params, double x0, double y0, double dt);

  void evolve();

  std::size_t size() const;

  State state(std::size_t i) const;

  Parameters const &parameters() const;

 private:
  Parameters params_;
  double dt_;

  double x_eq_;
  double y_eq_;

  std::vector<double> x_rel_;
  std::vector<double> y_rel_;
  std::vector<double> H_;

  double compute_H(double x_abs, double y_abs) const;
};

}  // namespace lotka_volterra

#endif