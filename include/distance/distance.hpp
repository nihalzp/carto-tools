#pragma once

#include "map/map.hpp"
#include "region/region.hpp"
#include <functional>
#include <utility>

std::pair<int, double> calculate_distance(
  const Region &,
  const Region &,
  const std::function<
    double(const Polygon_with_holes &, const Polygon_with_holes &)> &);

double calculate_distance(
  const Map &,
  const Map &,
  const std::function<std::pair<int, double>(const Region &, const Region &)>
    &);