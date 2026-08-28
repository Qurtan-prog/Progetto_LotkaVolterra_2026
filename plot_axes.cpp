#include "plot_axes.hpp"

#include <algorithm>
#include <cmath>
#include <iomanip>
#include <sstream>
#include <string>
#include <vector>

namespace lotka_volterra {
namespace {
const std::vector<std::string> kFontPaths = {
    "/usr/share/fonts/truetype/dejavu/DejaVuSans.ttf",
    "/usr/share/fonts/truetype/liberation/LiberationSans-Regular.ttf",
    "/System/Library/Fonts/Supplemental/Arial.ttf",
    "C:/Windows/Fonts/arial.ttf"};

// this function is needed to set the precision fo the vertical axes values,
// since the values for the two graphs have different magnitude
int decimalsFor(double range, int nDivisions) {
  if (!(range > 0.0)) {
    return 1;
  }
  double const step = range / nDivisions;
  int const d = static_cast<int>(std::ceil(-std::log10(step))) + 1;
  return std::clamp(d, 0, 9);
}
}  // namespace

bool loadAnyFont(sf::Font &font) {
  for (const auto &path : kFontPaths) {
    if (font.loadFromFile(path)) {
      return true;
    }
  }
  return false;
}

void drawAxes(sf::RenderTarget &target, const sf::FloatRect &area, double tMin,
              double tMax, double vMin, double vMax, const sf::Font &font,
              bool fontLoaded) {
  sf::RectangleShape border({area.width, area.height});
  border.setPosition(area.left, area.top);
  border.setFillColor(sf::Color::White);
  border.setOutlineColor(sf::Color(60, 60, 60));
  border.setOutlineThickness(1.f);
  target.draw(border);

  constexpr int nGridLines = 5;

  int const vDecimals = decimalsFor(vMax - vMin, nGridLines);
  int const tDecimals = decimalsFor(tMax - tMin, nGridLines);

  for (int i = 0; i <= nGridLines; ++i) {
    const float fy = area.top + area.height * i / nGridLines;
    sf::Vertex hLine[] = {
        sf::Vertex(sf::Vector2f(area.left, fy), sf::Color(220, 220, 220)),
        sf::Vertex(sf::Vector2f(area.left + area.width, fy),
                   sf::Color(220, 220, 220))};
    target.draw(hLine, 2, sf::Lines);

    if (fontLoaded) {
      const double v = vMax - (vMax - vMin) * i / nGridLines;  // value
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(vDecimals) << v;
      sf::Text label(oss.str(), font, 12);
      label.setFillColor(sf::Color::Black);
      // These two lines align the numeric values on the vertical axis so they
      // don't overflow into the plot area.
      sf::FloatRect const b{label.getLocalBounds()};
      label.setPosition(area.left - 10.f - (b.left + b.width), fy - 8.f);
      target.draw(label);
    }
  }

  for (int i = 0; i <= nGridLines; ++i) {
    const float fx = area.left + area.width * i / nGridLines;
    sf::Vertex vLine[] = {
        sf::Vertex(sf::Vector2f(fx, area.top), sf::Color(220, 220, 220)),
        sf::Vertex(sf::Vector2f(fx, area.top + area.height),
                   sf::Color(220, 220, 220))};
    target.draw(vLine, 2, sf::Lines);

    if (fontLoaded) {
      const double t = tMin + (tMax - tMin) * i / nGridLines;  // time
      std::ostringstream oss;
      oss << std::fixed << std::setprecision(1) << t;
      sf::Text label(oss.str(), font, 12);
      label.setFillColor(sf::Color::Black);
      // These two lines align the numeric values on the vertical axis so they
      // don't overflow into the plot area.
      sf::FloatRect const b{label.getLocalBounds()};
      label.setPosition(fx - (b.left + b.width) / 2.f,
                        area.top + area.height + 5.f);
      target.draw(label);
    }
  }
}

}  // namespace lotka_volterra
