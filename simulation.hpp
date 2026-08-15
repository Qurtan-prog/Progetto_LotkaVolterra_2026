#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

namespace lotka_volterra
{
  // Simula il sistema preda-predatore di Lotka-Volterra tramite il metodo di Eulero simplettico
  class Simulation
  {
  private:
    double A_;
    double B_;
    double C_;
    double D_;
    double dt_;

    // punto di equilibrio (D/C, A/B)
    double x_eq_;
    double y_eq_;

    // storia degli stati: x_rel_/y_rel_ in forma relativa, H_ assoluta
    std::vector<double> x_rel_;
    std::vector<double> y_rel_;
    std::vector<double> H_;

    // Calcola H in valore assoluto per lo stato i-esimo
    double compute_H(double x_abs, double y_abs) const;

  public:
    Simulation(double x0, double y0,
               double A, double B, double C, double D,
               double dt);

    void evolve();

    // Numero di stati calcolati finora (incluso quello iniziale).
    std::size_t size() const;

    // stato i-esimo, in valori assoluti
    double x(std::size_t i) const;
    double y(std::size_t i) const;
    double H(std::size_t i) const;
  };
}

#endif