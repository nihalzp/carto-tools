#include "error/area_weighted_mean_error.hpp"
#include "map/map.hpp"
#include "region/region.hpp"
#include <iostream>

double relative_error(const double actual_area, const double target_area)
{
  return abs(target_area - actual_area) / (target_area + actual_area);
}

double relative_error(const Region &region)
{
  const double actual_area = region.compute_area();
  const double target_area = region.get_target_area();
  return relative_error(actual_area, target_area);
}

double area_weighted_mean_error(const Map &map)
{
  double area_wgt_mean_error = 0.0;
  for (const auto &region : map.get_regions()) {
    area_wgt_mean_error += relative_error(region) * region.get_target_area();
  }
  return area_wgt_mean_error;
}

void print_area_weighted_mean_error(const Map &map)
{
  std::cout << "\n--------- Calculating Area Weighted Mean Error ---------"
            << std::endl;
  std::cout << "Area Weighted Mean Error: " << area_weighted_mean_error(map)
            << std::endl
            << std::endl;
}