#include "write/svg.h"

SVG::SVG(const std::string &filename, int w, int h) : width(w), height(h)
{
  svg_file.open(filename);
  svg_file << "<svg xmlns='http://www.w3.org/2000/svg' "
           << "width='" << width << "' height='" << height << "' "
           << "viewBox='0 0 " << width << " " << height << "'>\n";
}

void SVG::add_segment(
  double x1,
  double y1,
  double x2,
  double y2,
  const std::string &color,
  double stroke_width)
{
  svg_file << "<line x1='" << x1 << "' y1='" << height - y1 << "' x2='" << x2
           << "' y2='" << height - y2 << "' style='stroke:" << color
           << ";stroke-width:" << stroke_width << "' />\n";
}

void SVG::add_point(
  double x,
  double y,
  const std::string &color,
  double radius)
{
  svg_file << "<circle cx='" << x << "' cy='" << height - y << "' r='"
           << radius << "' style='fill:" << color << "' />\n";
}

SVG::~SVG()
{
  svg_file << "</svg>\n";
  svg_file.close();
}
