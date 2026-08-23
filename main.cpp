// PER COMPIPLARE: g++ -std=c++20 -Wall -Wextra simulation.cpp input.cpp main.cpp -o lotka_volterra

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
  try {
    std::cout << "Initial prey population x0: ";
    double const x0{read_positive_double(std::cin, "x0")};
    std::cout << "Initial predator population y0: ";
    double const y0{read_positive_double(std::cin, "y0")};
    std::cout << "Parameter A (prey growth rate): ";
    double const A{read_positive_double(std::cin, "A")};
    std::cout << "Parameter B (prey mortality rate): ";
    double const B{read_positive_double(std::cin, "B")};
    std::cout << "Parameter C (predator growth rate): ";
    double const C{read_positive_double(std::cin, "C")};
    std::cout << "Parameter D (predator mortality rate): ";
    double const D{read_positive_double(std::cin, "D")};
    std::cout << "Integration step dt: ";
    double const dt{read_positive_double(std::cin, "dt")};
    std::cout << "Number of steps to simulate: ";
    int const n_steps{read_positive_int(std::cin, "number of steps")};

    Parameters const params{.A = A, .B = B, .C = C, .D = D};
    Simulation sim(params, x0, y0, dt);

    for (int i{0}; i < n_steps; ++i) {
      sim.evolve();
    }

    std::ofstream out{"output.txt"};
    for (std::size_t i{0}; i < sim.size(); ++i) {
      double const t{i * dt};
      State const s{sim.state(i)};
      out << t << ' ' << s.x << ' ' << s.y << ' ' << s.H << '\n';
    }

    std::cout << "Simulation completed: " << sim.size()
               << " states written to output.txt\n";

  } catch (std::exception const& error) {
    std::cerr << "Error: " << error.what() << '\n';
    return EXIT_FAILURE;
  }
}