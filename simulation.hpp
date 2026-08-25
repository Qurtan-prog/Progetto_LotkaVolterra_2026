#include <vector>

#ifndef SIMULATION_HPP
#define SIMULATION_HPP

namespace lotka_volterra
{

  // I quattro parametri del modello, raggruppati per evitare di passarli
  // come singoli double (facili da scambiare tra loro per posizione,
  // come ci e' gia' capitato con C e D).
  struct Parameters
  {
    double A;
    double B;
    double C;
    double D;

  /*avremmo potuto mettere cosi al posto che con default alla fine ma almeno lo 
  fa il compilatorre direttamente il confronto e non dobbiamo scrivere noi di 
  confrontare A con A, B con B... e se aggiungeremo altri parametri in futuro li 
  cofronta automaticamente:

  bool operator==(Parameters const& other) const
{
  return A == other.A && B == other.B && C == other.C && D == other.D;
}*/

    bool operator==(Parameters const &) const = default; //l'intera riga insieme dice "genera un confronto di uguaglianza membro-a-membro, che non modifica nulla, tra questo oggetto e un altro passato per riferimento costante"
  };

  // Lo stato del sistema in un dato istante, in valori assoluti.
  struct State
  {
    double x;
    double y;
    double H;

    bool operator==(State const &) const = default; 
  };

  // Sistema preda-predatore di Lotka-Volterra, integrato con Eulero
  // simplettico. Lo stato interno e' salvato in coordinate relative
  // rispetto al punto di equilibrio, per stabilita' numerica.
  class Simulation
  {
  public:
    Simulation(Parameters const &params, double x0, double y0, double dt);

    // avanza la simulazione di un passo dt
    void evolve();

    std::size_t size() const;

    // stato i-esimo, in valori assoluti
    State state(std::size_t i) const;

    Parameters const &parameters() const;

  private:
    Parameters params_;
    double dt_;

    // punto di equilibrio (D/C, A/B), usato per convertire tra
    // valori assoluti e relativi
    double x_eq_;
    double y_eq_;

    // storia degli stati: x_rel_/y_rel_ in forma relativa, H_ assoluta
    std::vector<double> x_rel_;
    std::vector<double> y_rel_;
    std::vector<double> H_;

    double compute_H(double x_abs, double y_abs) const;
  };

} // namespace lotka_volterra

#endif