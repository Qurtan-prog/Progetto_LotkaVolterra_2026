#ifndef INPUT_HPP
#define INPUT_HPP

#include <istream>
#include <string>

namespace lotka_volterra
{

    double read_positive_double(std::istream &in, std::string const &name);

    int read_positive_int(std::istream &in, std::string const &name);

}

#endif