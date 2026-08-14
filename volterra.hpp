#ifndef VOLTERRA_HPP
#define VOLTERRA_HPP

#include <cmath>
#include <fstream>
#include <iostream>
#include <stdexcept>
#include <string>
#include <vector>

namespace definition
{
    struct Parameters
    {
        double a; // prey birth rate
        double b; // prey death rate
        double c; // predator birth rate
        double d; // predator death rate
    };

    struct State
    {
        double x; // prey
        double y; // predator
        double H; // Hamiltonian
    };

    class Simulation
    {
    }
}

#endif