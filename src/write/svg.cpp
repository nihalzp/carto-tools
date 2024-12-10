#include "write/svg.h"
#include <string>

SVG::SVG(const std::string &filename, unsigned int w, unsigned int h)
    : width_(w), height_(h)
{
  svg_file_.open(filename);
  svg_file_ << "<svg xmlns='http://www.w3.org/2000/svg' "
            << "width='" << width_ << "' height='" << height_ << "' "
            << "viewBox='0 0 " << width_ << " " << height_ << "'>\n";
}

void SVG::add_segment(
  double x1,
  double y1,
  double x2,
  double y2,
  const std::string &color,
  double stroke_width)
{
  svg_file_ << "<line x1='" << x1 << "' y1='" << height_ - y1 << "' x2='" << x2
            << "' y2='" << height_ - y2 << "' style='stroke:" << color
            << ";stroke-width:" << stroke_width << "' />\n";
}

void SVG::add_point(
  double x,
  double y,
  const std::string &color,
  double radius)
{
  svg_file_ << "<circle cx='" << x << "' cy='" << height_ - y << "' r='"
            << radius << "' style='fill:" << color << "' />\n";
}

SVG::~SVG()
{
  svg_file_ << "</svg>\n";
  svg_file_.close();
}
