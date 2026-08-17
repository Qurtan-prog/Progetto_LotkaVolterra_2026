#include "Plotter.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>

namespace lotka_volterra
{

namespace {
const std::vector<std::string> kFontPaths = {
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "C:/Windows/Fonts/arial.ttf"
};
} // namespace

Plotter::Plotter(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title), fontLoaded_(false)
{
    window_.setFramerateLimit(60);
    for (const auto& path : kFontPaths) {
        if (font_.loadFromFile(path)) {
            fontLoaded_ = true;
            break;
        }
    }
}

void Plotter::drawAxes(sf::RenderTarget& target, const sf::FloatRect& area,
                       double tMin, double tMax, double vMin, double vMax)
{
    sf::RectangleShape border({area.width, area.height});
    border.setPosition(area.left, area.top);
    border.setFillColor(sf::Color::White);
    border.setOutlineColor(sf::Color(60, 60, 60));
    border.setOutlineThickness(1.f);
    target.draw(border);

    constexpr int nGridLines = 5;

    // Horizontal grid lines and value labels
    for (int i = 0; i <= nGridLines; ++i) {
        const float fy = area.top + area.height * i / nGridLines;
        sf::Vertex hLine[] = {
            sf::Vertex(sf::Vector2f(area.left, fy), sf::Color(220, 220, 220)),
            sf::Vertex(sf::Vector2f(area.left + area.width, fy), sf::Color(220, 220, 220))
        };
        target.draw(hLine, 2, sf::Lines);

        if (fontLoaded_) {
            const double value = vMax - (vMax - vMin) * i / nGridLines;
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << value;
            sf::Text label(oss.str(), font_, 12);
            label.setFillColor(sf::Color::Black);
            label.setPosition(area.left - 55.f, fy - 8.f);
            target.draw(label);
        }
    }

    // Vertical grid lines and time labels
    for (int i = 0; i <= nGridLines; ++i) {
        const float fx = area.left + area.width * i / nGridLines;
        sf::Vertex vLine[] = {
            sf::Vertex(sf::Vector2f(fx, area.top), sf::Color(220, 220, 220)),
            sf::Vertex(sf::Vector2f(fx, area.top + area.height), sf::Color(220, 220, 220))
        };
        target.draw(vLine, 2, sf::Lines);

        if (fontLoaded_) {
            const double t = tMin + (tMax - tMin) * i / nGridLines;
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << t;
            sf::Text label(oss.str(), font_, 12);
            label.setFillColor(sf::Color::Black);
            label.setPosition(fx - 10.f, area.top + area.height + 5.f);
            target.draw(label);
        }
    }
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

    drawAxes(target, area, tMin, tMax, vMin, vMax);

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