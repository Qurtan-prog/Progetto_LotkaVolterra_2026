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

// SFML Window class to render population dynamics and first integral H(t)
class Plotter {
public:
    explicit Plotter(unsigned int width = 1100, unsigned int height = 750,
                     const std::string& title = "Lotka-Volterra - Andamento nel Tempo");

    void show(const std::vector<double>& time,
              const std::vector<Series>& populationSeries,
              const Series& hSeries);

    // Come show(), ma apre anche una seconda finestra indipendente con
    // l'orbita nel piano (x, y): preda sull'asse orizzontale, predatore
    // su quello verticale. La finestra dell'orbita gira su un thread
    // separato con il proprio contesto OpenGL, cosi' da evitare i
    // problemi di stabilita' che si hanno passando lo stesso contesto
    // avanti e indietro tra due finestre sullo stesso thread. Chiudendo
    // una finestra l'altra resta aperta; il metodo ritorna solo quando
    // entrambe sono state chiuse.
    void showWithOrbit(const std::vector<double>& time,
                        const std::vector<Series>& populationSeries,
                        const Series& hSeries,
                        const std::vector<double>& x_values,
                        const std::vector<double>& y_values);

private:
    sf::RenderWindow window_;
    sf::Font font_;
    bool fontLoaded_;

    void drawPlot(sf::RenderTarget& target,
                  const sf::FloatRect& area,
                  const std::vector<double>& time,
                  const std::vector<Series>& series,
                  const std::string& title);

    void drawAxes(sf::RenderTarget& target, const sf::FloatRect& area,
                  double tMin, double tMax, double vMin, double vMax);

    void drawLegend(sf::RenderTarget& target, const sf::Vector2f& pos,
                    const std::vector<Series>& series);

    // Versioni "statiche" (non dipendono da this) usate anche dal
    // thread dedicato alla finestra dell'orbita, che ha il proprio
    // font caricato localmente per evitare di condividere font_ tra
    // due thread/contesti diversi.
    static void drawAxesStatic(sf::RenderTarget& target, const sf::FloatRect& area,
                               double tMin, double tMax, double vMin, double vMax,
                               const sf::Font& font, bool fontLoaded);

    static void drawOrbitStatic(sf::RenderTarget& target, const sf::FloatRect& area,
                                const std::vector<double>& x_values,
                                const std::vector<double>& y_values,
                                const std::string& title,
                                const sf::Font& font, bool fontLoaded);

    // Funzione eseguita sul thread dedicato: crea la propria finestra
    // e il proprio font, poi gira il ciclo eventi/disegno finche' non
    // viene chiusa.
    static void runOrbitWindow(const std::vector<double>& x_values,
                               const std::vector<double>& y_values);
};

} // namespace lotka_volterra

#endif
