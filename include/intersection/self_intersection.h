#pragma once

#include "map/map.h"
#include <boost/geometry.hpp>
#include <boost/geometry/geometries/point_xy.hpp>
#include <vector>

typedef boost::geometry::model::d2::point_xy<double> Point;

std::vector<Point> get_self_intersections(const Map &);