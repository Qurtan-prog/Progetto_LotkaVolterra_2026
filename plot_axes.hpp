#ifndef PLOT_AXES_HPP
#define PLOT_AXES_HPP

#include <SFML/Graphics.hpp>

namespace lotka_volterra {
bool loadAnyFont(sf::Font &font);

void drawAxes(sf::RenderTarget &target, const sf::FloatRect &area, double tMin,
              double tMax, double vMin, double vMax, const sf::Font &font,
              bool fontLoaded);

}  // namespace lotka_volterra

#endif
