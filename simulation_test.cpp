#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "doctest.h"

#include "simulation.hpp"
#include "input.hpp"

#include <sstream>
#include <stdexcept>

using lotka_volterra::Parameters;
using lotka_volterra::read_positive_double;
using lotka_volterra::read_positive_int;
using lotka_volterra::Simulation;
using lotka_volterra::State;

TEST_CASE("read_positive_double: valore valido viene letto correttamente")
{
    std::istringstream in{"3.14"};
    CHECK(read_positive_double(in, "test") == doctest::Approx(3.14));
}

TEST_CASE("read_positive_double: valore non numerico lancia un'eccezione")
{
    std::istringstream in{"abc"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
}

TEST_CASE("read_positive_double: valore zero lancia un'eccezione")
{
    std::istringstream in{"0"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
}

TEST_CASE("read_positive_double: valore negativo lancia un'eccezione")
{
    std::istringstream in{"-2.5"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
}

TEST_CASE("read_positive_int: valore valido viene letto correttamente")
{
    std::istringstream in{"100"};
    CHECK(read_positive_int(in, "test") == 100);
}

TEST_CASE("read_positive_int: valore non intero lancia un'eccezione")
{
    std::istringstream in{"abc"};
    CHECK_THROWS_AS(read_positive_int(in, "test"), std::runtime_error);
}

TEST_CASE("read_positive_int: valore non positivo lancia un'eccezione")
{
    std::istringstream in{"-5"};
    CHECK_THROWS_AS(read_positive_int(in, "test"), std::runtime_error);
}

TEST_CASE("Simulation: costruttore con parametri non validi lancia")
{
    Parameters const valid{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};

    SUBCASE("A non positivo")
    {
        Parameters const p{.A = 0.0, .B = 0.00125, .C = 0.001, .D = 1.0};
        CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("B negativo")
    {
        Parameters const p{.A = 1.0, .B = -0.00125, .C = 0.001, .D = 1.0};
        CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("C negativo")
    {
        Parameters const p{.A = 1.0, .B = 0.00125, .C = -0.001, .D = 1.0};
        CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("D non positivo")
    {
        Parameters const p{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 0.0};
        CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("x0 non positivo")
    {
        CHECK_THROWS_AS(Simulation(valid, 0.0, 1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("y0 negativo")
    {
        CHECK_THROWS_AS(Simulation(valid, 1200.0, -1000.0, 0.001), std::invalid_argument);
    }
    SUBCASE("dt non positivo")
    {
        CHECK_THROWS_AS(Simulation(valid, 1200.0, 1000.0, 0.0), std::invalid_argument);
    }
}

TEST_CASE("Simulation: costruttore con parametri validi non lancia")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    CHECK_NOTHROW(Simulation(params, 1200.0, 1000.0, 0.001));
}

TEST_CASE("Simulation: parameters() restituisce i parametri passati al costruttore")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    Simulation sim{params, 1200.0, 1000.0, 0.001};
    CHECK(sim.parameters() == params);
}

TEST_CASE("Simulation: lo stato iniziale e' quello passato al costruttore")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    Simulation sim{params, 1200.0, 1000.0, 0.001};
    CHECK(sim.size() == 1);
    State const s0{sim.state(0)};
    CHECK(s0.x == doctest::Approx(1200.0));
    CHECK(s0.y == doctest::Approx(1000.0));
}

TEST_CASE("Simulation: evolve() aggiunge esattamente uno stato")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    Simulation sim{params, 1200.0, 1000.0, 0.001};
    sim.evolve();
    CHECK(sim.size() == 2);
}

TEST_CASE("Simulation: H si conserva durante l'evoluzione")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    Simulation sim{params, 1200.0, 1000.0, 0.001};
    double const H0{sim.state(0).H};
    for (int i{0}; i < 1000; ++i)
    {
        sim.evolve();
    }
    CHECK(sim.state(sim.size() - 1).H == doctest::Approx(H0));
}

TEST_CASE("Simulation: accesso con indice fuori intervallo lancia un'eccezione")
{
    Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    Simulation sim{params, 1200.0, 1000.0, 0.001};
    CHECK_THROWS_AS(sim.state(1), std::out_of_range);
}