#ifndef PLOTTER_HPP
#define PLOTTER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace lotka_volterra
{

// Data series for rendering (label, values, and line color)
struct Series {
    std::string label;
    std::vector<double> values;
    sf::Color color;
};

// SFML Window class to render population dynamics and first integral H(t).
// Gestisce solo il grafico nel tempo: l'orbita nel piano (x, y) e' stata
// spostata nella classe OrbitPlotter, cosi' ognuna delle due apre e
// gestisce una sola finestra ed e' responsabile di un solo tipo di
// grafico.
class Plotter {
public:
    explicit Plotter(unsigned int width = 1100, unsigned int height = 750,
                     const std::string& title = "Lotka-Volterra - Andamento nel Tempo");

    void show(const std::vector<double>& time,
              const std::vector<Series>& populationSeries,
              const Series& hSeries);

private:
    sf::RenderWindow window_;
    sf::Font font_;
    bool fontLoaded_;

    void drawPlot(sf::RenderTarget& target,
                  const sf::FloatRect& area,
                  const std::vector<double>& time,
                  const std::vector<Series>& series,
                  const std::string& title);

    void drawLegend(sf::RenderTarget& target, const sf::Vector2f& pos,
                    const std::vector<Series>& series);
};

} // namespace lotka_volterra

#endif
