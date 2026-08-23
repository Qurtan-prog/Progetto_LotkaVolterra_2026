#ifndef ORBIT_PLOTTER_HPP
#define ORBIT_PLOTTER_HPP

#include <SFML/Graphics.hpp>
#include <string>
#include <vector>

namespace lotka_volterra
{

// Disegna l'orbita nel piano delle fasi (x = prede, y = predatori) in
// una propria finestra SFML indipendente. show() e' pensata per girare
// su un thread dedicato (avviato da main con std::thread), quindi non
// condivide window_/font_ con la classe Plotter: crea e usa tutto
// localmente, cosi' da evitare i problemi di stabilita' che si hanno
// passando lo stesso contesto OpenGL tra finestre su thread diversi.
class OrbitPlotter {
public:
    // Apre la finestra e gira il ciclo eventi/disegno finche' l'utente
    // non la chiude. E' bloccante: da chiamare su un thread dedicato se
    // deve stare aperta insieme ad altre finestre. E' static perche'
    // non serve nessuno stato persistente tra una chiamata e l'altra:
    // finestra e font vengono creati e distrutti dentro la funzione.
    static void show(const std::vector<double>& x_values,
                      const std::vector<double>& y_values);

private:
    static void drawOrbit(sf::RenderTarget& target, const sf::FloatRect& area,
                          const std::vector<double>& x_values,
                          const std::vector<double>& y_values,
                          const std::string& title,
                          const sf::Font& font, bool fontLoaded);
};

} // namespace lotka_volterra

#endif
