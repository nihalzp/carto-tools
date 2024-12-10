#pragma once

#include "map/map.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;

void plot_map_with_intersections(
  const Map &,
  const std::vector<Point> &,
  const std::vector<Point> &);