#include "Plotter.hpp"

#include <algorithm>
#include <iomanip>
#include <sstream>
#include <thread>

namespace lotka_volterra
{

namespace {
const std::vector<std::string> kFontPaths = {
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "C:/Windows/Fonts/arial.ttf"
};

bool loadAnyFont(sf::Font& font)
{
    for (const auto& path : kFontPaths) {
        if (font.loadFromFile(path)) {
            return true;
        }
    }
    return false;
}
} // namespace

Plotter::Plotter(unsigned int width, unsigned int height, const std::string& title)
    : window_(sf::VideoMode(width, height), title), fontLoaded_(false)
{
    window_.setFramerateLimit(60);
    fontLoaded_ = loadAnyFont(font_);
}

void Plotter::drawAxesStatic(sf::RenderTarget& target, const sf::FloatRect& area,
                             double tMin, double tMax, double vMin, double vMax,
                             const sf::Font& font, bool fontLoaded)
{
    sf::RectangleShape border({area.width, area.height});
    border.setPosition(area.left, area.top);
    border.setFillColor(sf::Color::White);
    border.setOutlineColor(sf::Color(60, 60, 60));
    border.setOutlineThickness(1.f);
    target.draw(border);

    constexpr int nGridLines = 5;

    for (int i = 0; i <= nGridLines; ++i) {
        const float fy = area.top + area.height * i / nGridLines;
        sf::Vertex hLine[] = {
            sf::Vertex(sf::Vector2f(area.left, fy), sf::Color(220, 220, 220)),
            sf::Vertex(sf::Vector2f(area.left + area.width, fy), sf::Color(220, 220, 220))
        };
        target.draw(hLine, 2, sf::Lines);

        if (fontLoaded) {
            const double value = vMax - (vMax - vMin) * i / nGridLines;
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << value;
            sf::Text label(oss.str(), font, 12);
            label.setFillColor(sf::Color::Black);
            label.setPosition(area.left - 55.f, fy - 8.f);
            target.draw(label);
        }
    }

    for (int i = 0; i <= nGridLines; ++i) {
        const float fx = area.left + area.width * i / nGridLines;
        sf::Vertex vLine[] = {
            sf::Vertex(sf::Vector2f(fx, area.top), sf::Color(220, 220, 220)),
            sf::Vertex(sf::Vector2f(fx, area.top + area.height), sf::Color(220, 220, 220))
        };
        target.draw(vLine, 2, sf::Lines);

        if (fontLoaded) {
            const double t = tMin + (tMax - tMin) * i / nGridLines;
            std::ostringstream oss;
            oss << std::fixed << std::setprecision(1) << t;
            sf::Text label(oss.str(), font, 12);
            label.setFillColor(sf::Color::Black);
            label.setPosition(fx - 10.f, area.top + area.height + 5.f);
            target.draw(label);
        }
    }
}

void Plotter::drawAxes(sf::RenderTarget& target, const sf::FloatRect& area,
                       double tMin, double tMax, double vMin, double vMax)
{
    drawAxesStatic(target, area, tMin, tMax, vMin, vMax, font_, fontLoaded_);
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

void Plotter::drawOrbitStatic(sf::RenderTarget& target, const sf::FloatRect& area,
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

    drawAxesStatic(target, area, xMin, xMax, yMin, yMax, font, fontLoaded);

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

void Plotter::runOrbitWindow(const std::vector<double>& x_values,
                             const std::vector<double>& y_values)
{
    // Finestra e font creati e usati interamente su questo thread: il
    // contesto OpenGL non viene mai condiviso o scambiato con quello
    // della finestra principale, evitando i problemi di stabilita' che
    // si hanno alternando setActive() su due finestre sullo stesso thread.
    sf::RenderWindow orbitWindow(sf::VideoMode(600, 600),
                                  "Lotka-Volterra - Orbita (prede vs predatori)");
    orbitWindow.setFramerateLimit(60);

    sf::Font font;
    bool const fontLoaded = loadAnyFont(font);

    const sf::FloatRect orbitArea(70.f, 60.f, 480.f, 480.f);

    while (orbitWindow.isOpen()) {
        sf::Event event;
        while (orbitWindow.pollEvent(event)) {
            if (event.type == sf::Event::Closed)
                orbitWindow.close();
        }

        orbitWindow.clear(sf::Color(245, 245, 245));
        drawOrbitStatic(orbitWindow, orbitArea, x_values, y_values,
                        "Orbita nel piano (x, y)", font, fontLoaded);
        orbitWindow.display();
    }
}

void Plotter::showWithOrbit(const std::vector<double>& time,
                            const std::vector<Series>& populationSeries,
                            const Series& hSeries,
                            const std::vector<double>& x_values,
                            const std::vector<double>& y_values)
{
    // La finestra dell'orbita gira su un thread dedicato, con la
    // propria finestra/contesto creati al suo interno. x_values e
    // y_values sono catturati per riferimento: e' sicuro perche' questo
    // metodo fa join del thread prima di ritornare, quindi i vettori
    // restano validi per tutta la vita del thread.
    std::thread orbitThread(&Plotter::runOrbitWindow, std::cref(x_values), std::cref(y_values));

    // La finestra principale gira sul thread chiamante, come in show().
    show(time, populationSeries, hSeries);

    // A questo punto la finestra principale e' stata chiusa, ma quella
    // dell'orbita potrebbe essere ancora aperta: aspettiamo che l'utente
    // la chiuda prima di terminare il programma.
    orbitThread.join();
}

} // namespace lotka_volterra
