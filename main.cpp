#ifdef __linux__
    XInitThreads();
#endif


#include "simulation.hpp"
#include "input.hpp"
#include "Plotter.hpp"
#include "OrbitPlotter.hpp"

#include <cstdlib>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <thread>
#include <vector>

using lotka_volterra::OrbitPlotter;
using lotka_volterra::Parameters;
using lotka_volterra::Plotter;
using lotka_volterra::read_positive_double;
using lotka_volterra::read_positive_int;
using lotka_volterra::Series;
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

        // Dati da passare sia al file di output sia ai Plotter
        std::vector<double> time;
        std::vector<double> x_values;
        std::vector<double> y_values;
        std::vector<double> h_values;
        time.reserve(sim.size());
        x_values.reserve(sim.size());
        y_values.reserve(sim.size());
        h_values.reserve(sim.size());

        for (std::size_t i{0}; i < sim.size(); ++i)
        {
            double const t{i * dt};
            State const s{sim.state(i)};
            out << t << ' ' << s.x << ' ' << s.y << ' ' << s.H << '\n';

            time.push_back(t);
            x_values.push_back(s.x);
            y_values.push_back(s.y);
            h_values.push_back(s.H);
        }

        std::cout << "Simulazione completata: " << sim.size()
                  << " stati scritti in output.txt\n";

        // Costruzione delle serie e apertura delle finestre grafiche.
        std::vector<Series> populationSeries{
            Series{"Prede x(t)", x_values, sf::Color::Blue},
            Series{"Predatori y(t)", y_values, sf::Color::Red}};
        Series const hSeries{"H(t)", h_values, sf::Color(0, 150, 0)};

        // L'orbita gira su un thread dedicato, con la propria finestra e
        // il proprio contesto OpenGL creati al suo interno (vedi
        // OrbitPlotter::show). x_values e y_values sono catturati per
        // riferimento: e' sicuro perche' aspettiamo la fine del thread
        // (join) prima di uscire da main, quindi i vettori restano
        // validi per tutta la vita del thread.
#ifdef __linux__
#include <X11/Xlib.h>
#endif


        std::thread orbitThread(&OrbitPlotter::show, std::cref(x_values), std::cref(y_values));

        // La finestra principale (popolazioni + H) gira sul thread
        // chiamante.
        Plotter plotter;
        plotter.show(time, populationSeries, hSeries);

        // A questo punto la finestra principale e' stata chiusa, ma
        // quella dell'orbita potrebbe essere ancora aperta: aspettiamo
        // che l'utente la chiuda prima di terminare il programma.
        orbitThread.join();
    }
    catch (std::exception const &error)
    {
        std::cerr << "Errore: " << error.what() << '\n';
        return EXIT_FAILURE;
    }
}
