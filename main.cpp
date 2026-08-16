#include "simulation.hpp"
#include "input.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>

using lotka_volterra::Parameters;
using lotka_volterra::read_positive_double;
using lotka_volterra::read_positive_int;
using lotka_volterra::Simulation;
using lotka_volterra::State;

int main()
{
    try
    {
        std::cout << "Popolazione iniziale prede x0: ";
        double const x0{read_positive_double(std::cin, "x0")};
        std::cout << "Popolazione iniziale predatori y0: ";
        double const y0{read_positive_double(std::cin, "y0")};
        std::cout << "Parametro A (natalita' prede): ";
        double const A{read_positive_double(std::cin, "A")};
        std::cout << "Parametro B (mortalita' prede): ";
        double const B{read_positive_double(std::cin, "B")};
        std::cout << "Parametro C (natalita' predatori): ";
        double const C{read_positive_double(std::cin, "C")};
        std::cout << "Parametro D (mortalita' predatori): ";
        double const D{read_positive_double(std::cin, "D")};
        std::cout << "Passo di integrazione dt: ";
        double const dt{read_positive_double(std::cin, "dt")};
        std::cout << "Numero di passi da simulare: ";
        int const n_steps{read_positive_int(std::cin, "numero di passi")};

        Parameters const params{.A = A, .B = B, .C = C, .D = D};
        Simulation sim(params, x0, y0, dt);

        for (int i{0}; i < n_steps; ++i)
        {
            sim.evolve();
        }

        std::ofstream out{"output.txt"};
        for (std::size_t i{0}; i < sim.size(); ++i)
        {
            double const t{i * dt};
            State const s{sim.state(i)};
            out << t << ' ' << s.x << ' ' << s.y << ' ' << s.H << '\n';
        }

        std::cout << "Simulazione completata: " << sim.size()
                  << " stati scritti in output.txt\n";
    }
    catch (std::exception const &error)
    {
        std::cerr << "Errore: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}