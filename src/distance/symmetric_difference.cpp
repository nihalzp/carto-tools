#include "distance/symmetric_difference.hpp"
#include "distance/distance.hpp"
#include "map/map.hpp"
#include "polygon_with_holes/polygon_with_holes.hpp"
#include "region/region.hpp"
#include <boost/geometry.hpp>
#include <boost/geometry/algorithms/area.hpp>
#include <boost/geometry/algorithms/sym_difference.hpp>

double calculate_symmetric_difference(
  const Region &region1,
  const Region &region2)
{
  double total_symmetric_difference = 0.0;

  for (unsigned int i = 0; i < region1.get_num_pwhs(); ++i) {
    const auto &pwh1 = region1.get_pwhs()[i];
    const auto &pwh2 = region2.get_pwhs()[i];

    std::vector<Polygon_with_holes> sym_difference_result;
    boost::geometry::sym_difference(pwh1, pwh2, sym_difference_result);

    for (const auto &diff_poly : sym_difference_result) {
      total_symmetric_difference += diff_poly.compute_area();
    }
  }

  return total_symmetric_difference;
}

double calculate_symmetric_difference(const Map &map1, const Map &map2)
{
  return calculate_distance(
    map1,
    map2,
    static_cast<double (*)(const Region &, const Region &)>(
      calculate_symmetric_difference),
    combiner_addition);
}
