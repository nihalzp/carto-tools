#pragma once

#include "map/map.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;

std::vector<Point> get_overlap_intersections(const Map &);