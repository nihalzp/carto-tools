#pragma once

#include "map/map.hpp"
#include "region/region.hpp"
#include <functional>

double calculate_distance(
  const Map &,
  const Map &,
  const std::function<double(const Region &, const Region &)> &,
  const std::function<double(const double, const double)> &);
double combiner_addition(const double, const double);
double combiner_maximum(const double, const double);