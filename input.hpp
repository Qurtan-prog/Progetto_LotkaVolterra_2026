/*lascio qui per chiarezza: visto che vogliamo testare read_positive_double/read_positive_int
con Doctest, usando std::istringstream al posto di std::cin serve questo file.
Se queste funzioni stessero dentro main.cpp, simulation_test.cpp non potrebbe includerle
senza portarsi dietro anche int main() di main.cpp — e avresti due main in conflitto
quando compili i test (uno vero, uno generato da Doctest).*/

#ifndef PARAMETERS_HPP
#define PARAMETERS_HPP

#include <istream>
#include <string>

namespace lotka_volterra
{

    // Legge da 'in' un numero (double) associato al parametro 'name'.
    // Lancia std::runtime_error se il valore non e' leggibile come double,
    // o se non e' strettamente positivo.
    double read_positive_double(std::istream &in, std::string const &name);

    // Come sopra, ma per un intero strettamente positivo.
    int read_positive_int(std::istream &in, std::string const &name);

} // namespace lotka_volterra

#endif