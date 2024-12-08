#include "distance/distance.h"
#include "map/map.h"
#include "region/region.h"
#include <functional>

double combiner_addition(const double total, const double current)
{
  return total + current;
}

double combiner_maximum(const double max_value, const double current)
{
  return std::max(max_value, current);
}

double calculate_distance(
  const Map &map1,
  const Map &map2,
  const std::function<double(const Region &, const Region &)>
    &distance_calculator,
  const std::function<double(double, double)> &combiner)
{
  double distance = 0.0;
  for (auto &region1 : map1.get_regions()) {
    auto region2 = map2.find_matching_region(region1);
    distance = combiner(distance, distance_calculator(region1, region2));
  }
  return distance;
}
