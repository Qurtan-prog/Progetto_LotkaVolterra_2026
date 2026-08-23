#ifndef PLOT_AXES_HPP
#define PLOT_AXES_HPP

#include <SFML/Graphics.hpp>

namespace lotka_volterra
{

// Disegna assi, griglia e etichette numeriche in un riquadro `area`.
// E' una funzione libera (non legata a nessuna classe) cosi' puo'
// essere usata sia da Plotter (grafico nel tempo) sia da OrbitPlotter
// (grafico nel piano x,y): ognuna delle due passa il proprio font,
// caricato localmente, senza doverlo condividere tra thread diversi.
void drawAxes(sf::RenderTarget& target, const sf::FloatRect& area,
              double tMin, double tMax, double vMin, double vMax,
              const sf::Font& font, bool fontLoaded);

// Cerca di caricare un font di sistema tra alcuni percorsi noti.
// Ritorna true se un font e' stato caricato con successo.
bool loadAnyFont(sf::Font& font);

} // namespace lotka_volterra

#endif
