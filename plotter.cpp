#include "plotter.hpp"
#include "plot_axes.hpp"

#include <algorithm>

namespace lotka_volterra
{

Plotter::Plotter(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title), fontLoaded_(false)
{
    window_.setFramerateLimit(60);
    fontLoaded_ = loadAnyFont(font_);
}

void Plotter::drawLegend(sf::RenderTarget& target, const sf::Vector2f& pos,
                         const std::vector<Series>& series)
{
    float y = pos.y;
    for (const auto& s : series) {
        sf::RectangleShape swatch({14.f, 4.f});
        swatch.setPosition(pos.x, y + 6.f);
        swatch.setFillColor(s.color);
        target.draw(swatch);

        if (fontLoaded_) {
            sf::Text label(s.label, font_, 13);
            label.setFillColor(sf::Color::Black);
            label.setPosition(pos.x + 20.f, y);
            target.draw(label);
        }
        y += 20.f;
    }
}

void Plotter::drawPlot(sf::RenderTarget& target, const sf::FloatRect& area,
                       const std::vector<double>& time,
                       const std::vector<Series>& series,
                       const std::string& title)
{
    const double tMin = time.front();
    const double tMax = time.back();

    double vMin = series.front().values.front();
    double vMax = vMin;
    for (const auto& s : series) {
        for (double v : s.values) {
            vMin = std::min(vMin, v);
            vMax = std::max(vMax, v);
        }
    }
    if (vMax - vMin < 1e-9) vMax = vMin + 1.0;
    const double margin = (vMax - vMin) * 0.05;
    vMin -= margin;
    vMax += margin;

    drawAxes(target, area, tMin, tMax, vMin, vMax, font_, fontLoaded_);

    for (const auto& s : series) {
        sf::VertexArray line(sf::LineStrip, time.size());
        for (std::size_t i = 0; i < time.size(); ++i) {
            const float fx = area.left + area.width *
                              static_cast<float>((time[i] - tMin) / (tMax - tMin));
            const float fy = area.top + area.height *
                              static_cast<float>(1.0 - (s.values[i] - vMin) / (vMax - vMin));
            line[i] = sf::Vertex(sf::Vector2f(fx, fy), s.color);
        }
        target.draw(line);
    }

    if (fontLoaded_) {
        sf::Text titleText(title, font_, 16);
        titleText.setStyle(sf::Text::Bold);
        titleText.setFillColor(sf::Color::Black);
        titleText.setPosition(area.left, area.top - 25.f);
        target.draw(titleText);
    }

    drawLegend(target, sf::Vector2f(area.left + area.width - 160.f, area.top + 10.f), series);
}

void Plotter::show(const std::vector<double>& time,
                   const std::vector<Series>& populationSeries,
                   const Series& hSeries)
{
    const sf::FloatRect topArea(90.f, 60.f, 900.f, 280.f);
    const sf::FloatRect bottomArea(90.f, 430.f, 900.f, 240.f);

    while (window_.isOpen()) {
        sf::Event event;
        while (window_.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                window_.close();
        }

        window_.clear(sf::Color(245, 245, 245));
        drawPlot(window_, topArea, time, populationSeries, "Popolazioni nel tempo");
        drawPlot(window_, bottomArea, time, {hSeries}, "Integrale primo H(t)");
        window_.display();
    }
}

} // namespace lotka_volterra
