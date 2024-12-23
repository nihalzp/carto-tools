#include "distance/distance.hpp"
#include "constants.hpp"
#include "map/map.hpp"
#include "region/region.hpp"
#include <functional>
#include <utility>

std::pair<int, double> merge_comparison_data(
  const std::pair<int, double> total,
  const std::pair<int, double> current)
{
  return {total.first + current.first, total.second + current.second};
}

std::pair<int, double> calculate_distance(
  const Region &region1,
  const Region &region2,
  const std::function<
    double(const Polygon_with_holes &, const Polygon_with_holes &)>
    &distance_calculator)
{
  std::pair<int, double> pwh_comparison_data = {
    0,
    0.0};  // Holds {number of comparisons, total distance}

  const unsigned int possible_num_pwhs_to_compare = std::min(
    {region1.get_num_pwhs(), region2.get_num_pwhs(), NUM_PWH_TO_COMPARE});

  for (unsigned int i = 0; i < possible_num_pwhs_to_compare; ++i) {
    const Polygon_with_holes &pwh1 = region1.get_pwhs()[i];
    const Polygon_with_holes &pwh2 = region2.get_pwhs()[i];

    const double distance = distance_calculator(pwh1, pwh2);
    pwh_comparison_data =
      merge_comparison_data(pwh_comparison_data, {1, distance});
  }

  return pwh_comparison_data;
}

double calculate_distance(
  const Map &map1,
  const Map &map2,
  const std::function<std::pair<int, double>(const Region &, const Region &)>
    &distance_calculator)
{
  std::pair<int, double> pwh_comparison_data = {
    0,
    0.0};  // Holds {number of comparisons, total distance}

  for (const auto &region1 : map1.get_regions()) {
    const Region &region2 = map2.find_matching_region(region1);
    const std::pair<int, double> region_pwh_comparison_data =
      distance_calculator(region1, region2);
    pwh_comparison_data =
      merge_comparison_data(pwh_comparison_data, region_pwh_comparison_data);
  }

  return pwh_comparison_data.second / pwh_comparison_data.first;
}
