#include "OrbitPlotter.hpp"
#include "PlotAxes.hpp"

#include <algorithm>

namespace lotka_volterra
{

void OrbitPlotter::drawOrbit(sf::RenderTarget& target, const sf::FloatRect& area,
                             const std::vector<double>& x_values,
                             const std::vector<double>& y_values,
                             const std::string& title,
                             const sf::Font& font, bool fontLoaded)
{
    double xMin = *std::min_element(x_values.begin(), x_values.end());
    double xMax = *std::max_element(x_values.begin(), x_values.end());
    double yMin = *std::min_element(y_values.begin(), y_values.end());
    double yMax = *std::max_element(y_values.begin(), y_values.end());

    if (xMax - xMin < 1e-9) xMax = xMin + 1.0;
    if (yMax - yMin < 1e-9) yMax = yMin + 1.0;
    const double xMargin = (xMax - xMin) * 0.05;
    const double yMargin = (yMax - yMin) * 0.05;
    xMin -= xMargin; xMax += xMargin;
    yMin -= yMargin; yMax += yMargin;

    drawAxes(target, area, xMin, xMax, yMin, yMax, font, fontLoaded);

    sf::VertexArray line(sf::LineStrip, x_values.size());
    for (std::size_t i = 0; i < x_values.size(); ++i) {
        const float fx = area.left + area.width *
                          static_cast<float>((x_values[i] - xMin) / (xMax - xMin));
        const float fy = area.top + area.height *
                          static_cast<float>(1.0 - (y_values[i] - yMin) / (yMax - yMin));
        line[i] = sf::Vertex(sf::Vector2f(fx, fy), sf::Color(30, 30, 200));
    }
    target.draw(line);

    auto drawMarker = [&](double xv, double yv, sf::Color color) {
        const float fx = area.left + area.width *
                          static_cast<float>((xv - xMin) / (xMax - xMin));
        const float fy = area.top + area.height *
                          static_cast<float>(1.0 - (yv - yMin) / (yMax - yMin));
        sf::CircleShape marker(4.f);
        marker.setFillColor(color);
        marker.setOrigin(4.f, 4.f);
        marker.setPosition(fx, fy);
        target.draw(marker);
    };
    drawMarker(x_values.front(), y_values.front(), sf::Color::Green);
    drawMarker(x_values.back(), y_values.back(), sf::Color::Red);

    if (fontLoaded) {
        sf::Text titleText(title, font, 16);
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(area.left, area.top - 25.f);
        target.draw(titleText);

        sf::Text xLabel("x (prede)", font, 13);
        xLabel.setFillColor(sf::Color::Black);
        xLabel.setPosition(area.left + area.width - 70.f, area.top + area.height + 22.f);
        target.draw(xLabel);

        sf::Text yLabel("y (predatori)", font, 13);
        yLabel.setFillColor(sf::Color::Black);
        yLabel.setPosition(area.left + 5.f, area.top - 5.f);
        target.draw(yLabel);
    }
}

void OrbitPlotter::show(const std::vector<double>& x_values,
                        const std::vector<double>& y_values)
{
    // Finestra e font creati e usati interamente su questo thread: il
    // contesto OpenGL non viene mai condiviso o scambiato con quello
    // della finestra principale, evitando i problemi di stabilita' che
    // si hanno alternando setActive() su due finestre sullo stesso thread.
    sf::RenderWindow window(sf::VideoMode(600, 600),
                             "Lotka-Volterra - Orbita (prede vs predatori)");
    window.setFramerateLimit(60);

    sf::Font font;
    bool const fontLoaded = loadAnyFont(font);

    const sf::FloatRect orbitArea(70.f, 60.f, 480.f, 480.f);

    while (window.isOpen()) {
        sf::Event event;
        while (window.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window.close();
        }

        window.clear(sf::Color(245, 245, 245));
        drawOrbit(window, orbitArea, x_values, y_values,
                  "Orbita nel piano (x, y)", font, fontLoaded);
        window.display();
    }
}

} // namespace lotka_volterra
