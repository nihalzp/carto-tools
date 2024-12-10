#pragma once

#include "map/map.h"
#include "region/region.h"
#include <functional>

double combiner_addition(const double, const double);
double combiner_maximum(const double, const double);
double calculate_distance(
  const Map &,
  const Map &,
  const std::function<double(const Region &, const Region &)> &,
  const std::function<double(double, double)> &);