/*lascio qui per chiarezza: visto che vogliamo testare read_positive_double/read_positive_int
con Doctest, usando std::istringstream al posto di std::cin serve questo file.
Se queste funzioni stessero dentro main.cpp, simulation_test.cpp non potrebbe includerle
senza portarsi dietro anche int main() di main.cpp — e avresti due main in conflitto
quando compili i test (uno vero, uno generato da Doctest).*/

#ifndef INPUT_HPP
#define INPUT_HPP

#include <istream>
#include <string>

namespace lotka_volterra {

// Reads a double from 'in', associated with the parameter 'name'.
// Throws std::runtime_error if the value cannot be read as a double,
// or if it is not strictly positive.
double read_positive_double(std::istream& in, std::string const& name);

// Same as above, but for a strictly positive integer.
int read_positive_int(std::istream& in, std::string const& name);

} // namespace lotka_volterra

#endif