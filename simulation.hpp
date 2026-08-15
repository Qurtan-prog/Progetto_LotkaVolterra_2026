#ifndef SIMULATION_HPP
#define SIMULATION_HPP

#include <vector>

// Simula il sistema preda-predatore di Lotka-Volterra tramite il metodo
// di Eulero simplettico, in forma "relativa" rispetto al punto di
// equilibrio e2 = (D/C, A/B), per maggiore stabilita' numerica.
class Simulation
{
public:
  // Costruisce la simulazione a partire dallo stato iniziale (x0, y0)
  // e dai parametri A, B, C, D del modello, con passo di integrazione dt.
  Simulation(double x0, double y0,
             double A, double B, double C, double D,
             double dt);

  // Fa avanzare la simulazione di una singola unita' dt,
  // calcolando il nuovo stato (x_rel, y_rel) e il corrispondente H.
  void evolve();

  // Numero di stati calcolati finora (incluso quello iniziale).
  std::size_t size() const;

  // Valori assoluti dello stato i-esimo, per stampe/analisi.
  double x(std::size_t i) const;
  double y(std::size_t i) const;
  double H(std::size_t i) const;

private:
  // Parametri del modello.
  double A_;
  double B_;
  double C_;
  double D_;
  double dt_;

  // Coordinate del punto di equilibrio e2 = (D/C, A/B),
  // calcolate una volta nel costruttore e riusate per le conversioni.
  double x_eq_;
  double y_eq_;

  // Storia degli stati, in forma relativa (x_rel_[i], y_rel_[i])
  // e il corrispondente integrale primo H_[i], in forma assoluta.
  std::vector<double> x_rel_;
  std::vector<double> y_rel_;
  std::vector<double> H_;

  // Calcola H in valore assoluto per lo stato i-esimo (usata internamente
  // sia dal costruttore per lo stato iniziale, sia da evolve()).
  double compute_H(double x_abs, double y_abs) const;
};

#endif