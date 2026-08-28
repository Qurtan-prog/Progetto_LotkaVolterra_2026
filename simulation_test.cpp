
#define DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN
#include "simulation.hpp"

#include <sstream>
#include <stdexcept>

#include "doctest.h"
#include "input.hpp"

using lotka_volterra::Parameters;
using lotka_volterra::read_positive_double;
using lotka_volterra::read_positive_int;
using lotka_volterra::Simulation;
using lotka_volterra::State;

TEST_CASE("read_positive_double") {
  SUBCASE("valid value is read correctly") {
    std::istringstream in{"7.3"};
    CHECK(read_positive_double(in, "test") == doctest::Approx(7.3));
  }
  SUBCASE("non-numeric value throws an exception") {
    std::istringstream in{"abc"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
  }
  SUBCASE("zero value throws an exception") {
    std::istringstream in{"0"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
  }
  SUBCASE("negative value throws an exception") {
    std::istringstream in{"-2.5"};
    CHECK_THROWS_AS(read_positive_double(in, "test"), std::runtime_error);
  }
}

TEST_CASE("read_positive_int") {
  SUBCASE("valid value is read correctly") {
    std::istringstream in{"100"};
    CHECK(read_positive_int(in, "test") == 100);
  }
  SUBCASE("non-integer value throws an exception") {
    std::istringstream in{"abc"};
    CHECK_THROWS_AS(read_positive_int(in, "test"), std::runtime_error);
  }
  SUBCASE("non-positive value throws an exception") {
    std::istringstream in{"-5"};
    CHECK_THROWS_AS(read_positive_int(in, "test"), std::runtime_error);
  }
}

TEST_CASE("Simulation: constructor with invalid parameters throws") {
  Parameters const valid{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};

  SUBCASE("A not positive") {
    Parameters const p{.A = 0.0, .B = 0.00125, .C = 0.001, .D = 1.0};
    CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("B negative") {
    Parameters const p{.A = 1.0, .B = -0.00125, .C = 0.001, .D = 1.0};
    CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("C negative") {
    Parameters const p{.A = 1.0, .B = 0.00125, .C = -0.001, .D = 1.0};
    CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("D not positive") {
    Parameters const p{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 0.0};
    CHECK_THROWS_AS(Simulation(p, 1200.0, 1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("x0 not positive") {
    CHECK_THROWS_AS(Simulation(valid, 0.0, 1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("y0 negative") {
    CHECK_THROWS_AS(Simulation(valid, 1200.0, -1000.0, 0.001),
                    std::invalid_argument);
  }
  SUBCASE("dt not positive") {
    CHECK_THROWS_AS(Simulation(valid, 1200.0, 1000.0, 0.0),
                    std::invalid_argument);
  }
}

TEST_CASE("Simulation: behaviour with valid parameters") {
  Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
  Simulation sim{params, 1200.0, 1000.0, 0.001};

  SUBCASE("parameters() returns the parameters passed to the constructor") {
    CHECK(sim.parameters() == params);
  }
  SUBCASE("the initial state matches what was passed to the constructor") {
    CHECK(sim.size() == 1);
    State const s0{sim.state(0)};
    CHECK(s0.x == doctest::Approx(1200.0));
    CHECK(s0.y == doctest::Approx(1000.0));
  }
  SUBCASE("evolve() adds exactly one state") {
    sim.evolve();
    CHECK(sim.size() == 2);
  }
  SUBCASE("H is conserved during evolution") {
    double const H0{sim.state(0).H};
    for (int i{0}; i < 1000; ++i) {
      sim.evolve();
    }
    CHECK(sim.state(sim.size() - 1).H == doctest::Approx(H0));
  }
  SUBCASE("accessing an out-of-range index throws an exception") {
    CHECK_THROWS_AS(sim.state(1), std::out_of_range);
  }
}

TEST_CASE("Simulation: the equilibrium point does not evolve") {
  Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
  // equilibrium point: (D/C, A/B) = (1000, 800)
  Simulation sim{params, 1000.0, 800.0, 0.001};

  for (int i{0}; i < 100; ++i) {
    sim.evolve();
  }

  State const s{sim.state(sim.size() - 1)};
  CHECK(s.x == doctest::Approx(1000.0));
  CHECK(s.y == doctest::Approx(800.0));
}

TEST_CASE(
    "Simulation: one evolve() step matches the symplectic Euler formula") {
  Parameters const params{.A = 1.0, .B = 0.00125, .C = 0.001, .D = 1.0};
  Simulation sim{params, 1200.0, 1000.0, 0.001};
  sim.evolve();

  // x_rel = 1.2, y_rel = 1.25
  // y_new = 1.25 + D*(1.2 - 1)*1.25*dt = 1.25025      -> 1000.2
  // x_new = 1.2  + A*(1 - y_new)*1.2*dt = 1.1996997   -> 1199.6997
  State const s{sim.state(1)};
  CHECK(s.y == doctest::Approx(1000.2));
  CHECK(s.x == doctest::Approx(1199.6997));

  SUBCASE("above the equilibrium prey count, predators increase") {
    Simulation sim{params, 1200.0, 800.0, 0.001};  // x > D/C, y = A/B
    sim.evolve();
    CHECK(sim.state(1).y > sim.state(0).y);
  }
}
