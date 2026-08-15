#include "simulation.hpp"

#include <fstream>
#include <iostream>

using lotka_volterra::Simulation;

int main()
{
    double x0;
    double y0;
    double A;
    double B;
    double C;
    double D;
    double dt;
    int n_steps;

    std::cout << "Popolazione iniziale prede x0: ";
    std::cin >> x0;
    std::cout << "Popolazione iniziale predatori y0: ";
    std::cin >> y0;
    std::cout << "Parametro A (natalita' prede): ";
    std::cin >> A;
    std::cout << "Parametro B (mortalita' prede): ";
    std::cin >> B;
    std::cout << "Parametro C (natalita' predatori): ";
    std::cin >> C;
    std::cout << "Parametro D (mortalita' predatori): ";
    std::cin >> D;
    std::cout << "Passo di integrazione dt: ";
    std::cin >> dt;
    std::cout << "Numero di passi da simulare: ";
    std::cin >> n_steps;

    Simulation sim(x0, y0, A, B, C, D, dt);

    for (int i{0}; i < n_steps; ++i)
    {
        sim.evolve();
    }

    std::ofstream out{"output.txt"};
    for (std::size_t i{0}; i < sim.size(); ++i)
    {
        double const t{i * dt};
        out << t << '  ' << sim.x(i) << '  ' << sim.y(i) << '  ' << sim.H(i) << '\n';
    }

    std::cout << "Simulazione completata: " << sim.size()
              << " stati scritti in output.txt\n";
}