#include "simulation.hpp"

#include <cmath>
#include <stdexcept>

namespace lotka_volterra
{
  Parameters validated(Parameters const &params, double x0, double y0, double dt)
  {
    if (x0 <= 0.0 || y0 <= 0.0 || params.A <= 0.0 || params.B <= 0.0 || params.C <= 0.0 || params.D <= 0.0 || dt <= 0.0)
    {
      throw std::invalid_argument("Simulation All parameters have to be positive");
    }
    return params;
  }

  Simulation::Simulation(Parameters const &params, double x0, double y0, double dt)
      : params_(validated(params, x0, y0, dt)), dt_(dt),
        x_eq_(params.D / params.C), y_eq_(params.A / params.B)
  {
    double const x0_rel{x0 / x_eq_};
    double const y0_rel{y0 / y_eq_};

    x_rel_.push_back(x0_rel);
    y_rel_.push_back(y0_rel);
    H_.push_back(compute_H(x0, y0));
  }

  void Simulation::evolve()
  {
    double const x_prev{x_rel_.back()};
    double const y_prev{y_rel_.back()};

    double const y_new{y_prev + params_.D * (x_prev - 1.0) * y_prev * dt_};
    double const x_new{x_prev + params_.A * (1.0 - y_new) * x_prev * dt_};

    if (!(x_new > 0.0) || !(y_new > 0.0))
    {
      throw std::runtime_error(
          "Simulation::evolve: the simulation has diverged; next simulation try with a smaller dt !");
    }

    x_rel_.push_back(x_new);
    y_rel_.push_back(y_new);

    double const x_abs{x_new * x_eq_};
    double const y_abs{y_new * y_eq_};
    H_.push_back(compute_H(x_abs, y_abs));
  }

  std::size_t Simulation::size() const
  {
    return x_rel_.size();
  }

  State Simulation::state(std::size_t i) const
  {
    if (i >= x_rel_.size())
    {
      throw std::out_of_range("Simulation::state: index out of range");
    }
    return State{x_rel_[i] * x_eq_, y_rel_[i] * y_eq_, H_[i]};
  }

  Parameters const &Simulation::parameters() const
  {
    return params_;
  }

  double Simulation::compute_H(double x_abs, double y_abs) const
  {
    return -params_.D * std::log(x_abs) + params_.C * x_abs + params_.B * y_abs - params_.A * std::log(y_abs);
  }

}
