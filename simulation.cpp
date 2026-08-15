#include "simulation.hpp"

#include <cassert>
#include <cmath>

Simulation::Simulation(double x0, double y0,
                        double A, double B, double C, double D,
                        double dt)
  : A_(A), B_(B), C_(C), D_(D), dt_(dt),
    x_eq_(D / C), y_eq_(A / B)
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

  // Eulero simplettico: y aggiornata prima, usando ancora x_prev;
  // x aggiornata dopo, usando gia' il nuovo valore di y.
  double const y_new{y_prev + D_ * (x_prev - 1.0) * y_prev * dt_};
  double const x_new{x_prev + A_ * (1.0 - y_new) * x_prev * dt_};

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

double Simulation::x(std::size_t i) const
{
  assert(i < x_rel_.size());
  return x_rel_[i] * x_eq_;
}

double Simulation::y(std::size_t i) const
{
  assert(i < y_rel_.size());
  return y_rel_[i] * y_eq_;
}

double Simulation::H(std::size_t i) const
{
  assert(i < H_.size());
  return H_[i];
}

double Simulation::compute_H(double x_abs, double y_abs) const
{
  return -D_ * std::log(x_abs) + C_ * x_abs + B_ * y_abs - A_ * std::log(y_abs);
}