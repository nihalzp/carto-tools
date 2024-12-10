#include "error/max_relative_area_error.hpp"
#include "map/map.hpp"
#include "region/region.hpp"
#include <iostream>

double relative_area_error(const double actual_area, const double target_area)
{
  return abs(actual_area / target_area - 1);
}

double relative_area_error(const Region &region)
{
  const double actual_area = region.compute_area();
  const double target_area = region.get_target_area();
  return relative_area_error(actual_area, target_area);
}

void print_relative_area_error_region(const Region &region)
{
  for (auto [_, property] : region.get_properties()) {
    std::cout << property << ", ";
  }
  std::cout << ": " << relative_area_error(region) << std::endl;
}

double max_relative_area_error(const Map &map)
{
  double rel_area_error = 0.0;
  for (auto const &region : map.get_regions()) {
    rel_area_error = std::max(rel_area_error, relative_area_error(region));
  }
  return rel_area_error;
}

void print_max_relative_area_error_report(const Map &map)
{
  std::cout << "\n--------- Calculating Max Relative Area Error ---------"
            << std::endl;
  for (const auto &region : map.get_regions()) {
    print_relative_area_error_region(region);
  }
  std::cout << "Max Relative Area Error: " << max_relative_area_error(map)
            << std::endl
            << std::endl;
}