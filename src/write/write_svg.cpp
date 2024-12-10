#include "write/write_svg.hpp"
#include "map/map.hpp"
#include "region/region.hpp"
#include "write/svg.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <boost/geometry/geometries/polygon.hpp>
#include <boost/geometry/geometries/segment.hpp>
#include <string>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;
typedef boost::geometry::model::segment<Point> Segment;

void plot(const Point &point, SVG &svg, const std::string &color)
{
  svg.add_point(point.x(), point.y(), color);
}

void plot(const Segment &segment, SVG &svg, const std::string &color)
{
  svg.add_segment(
    segment.first.x(),
    segment.first.y(),
    segment.second.x(),
    segment.second.y(),
    color);
}

void plot_line(
  const std::vector<Point> &pts,
  SVG &svg,
  const std::string &color)
{
  for (unsigned int i = 0; i < pts.size(); ++i) {
    const auto &p1 = pts[i];
    const auto &p2 = pts[(i + 1) % pts.size()];
    plot(Segment(p1, p2), svg, color);
  }
}

void plot(const std::vector<Point> &pts, SVG &svg, const std::string &color)
{
  for (unsigned int i = 0; i < pts.size(); ++i) {
    plot(pts[i], svg, color);
  }
}

void plot(const Polygon_with_holes &pwh, SVG &svg, const std::string &color)
{
  plot_line(pwh.outer(), svg, color);
  for (const auto &hole : pwh.inners()) {
    plot_line(hole, svg, color);
  }
}

void plot(const Region &region, SVG &svg, const std::string &color)
{
  for (const auto &pwh : region.get_pwhs()) {
    plot(pwh, svg, color);
  }
}

void plot(const Map &map, SVG &svg, const std::string &color)
{
  for (const auto &region : map.get_regions()) {
    plot(region, svg, color);
  }
}

void plot_map_with_intersections(
  const Map &map,
  const std::vector<Point> &self_intersections,
  const std::vector<Point> &overlap_intersections)
{
  const std::string svg_filename = map.get_map_name() + "_intersections.svg";
  SVG svg(svg_filename, 512, 512);

  plot(map, svg, "black");
  plot(self_intersections, svg, "red");
  plot(overlap_intersections, svg, "blue");
}